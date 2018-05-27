// -------------------------------------------------------------------------
//    @FileName         :    NFEventWatcher.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFEventWatcher.h"

#include "NFLibEvent.h"
#include <iostream>
#include "NFEventLoop.h"
#include "NFComm/NFCore/NFDuration.h"


NFEventWatcher::NFEventWatcher(struct event_base* evbase, const Handler& handler)
	: evbase_(evbase), attached_(false), handler_(handler)
{
	event_ = new event;
	memset(event_, 0, sizeof(struct event));
}

NFEventWatcher::NFEventWatcher(struct event_base* evbase, Handler&& handler)
	: evbase_(evbase), attached_(false), handler_(std::move(handler))
{
	event_ = new event;
	memset(event_, 0, sizeof(struct event));
}

NFEventWatcher::~NFEventWatcher()
{
	FreeEvent();
	Close();
}

bool NFEventWatcher::Init()
{
	if (!DoInit())
	{
		goto failed;
	}

	::event_base_set(evbase_, event_);
	return true;

failed:
	Close();
	return false;
}


void NFEventWatcher::Close()
{
	DoClose();
}

bool NFEventWatcher::Watch(NFDuration timeout)
{
	struct timeval tv;
	struct timeval* timeoutval = nullptr;
	if (timeout.Nanoseconds() > 0)
	{
		timeout.To(&tv);
		timeoutval = &tv;
	}

	if (attached_)
	{
		// When InvokerTimer::periodic_ == true, EventWatcher::Watch will be called many times
		// so we need to remove it from event_base before we add it into event_base
		if (event_del(event_) != 0)
		{
			std::cerr << "event_del failed. fd=" << this->event_->ev_fd << " event_=" << event_ << std::endl;
			// TODO how to deal with it when failed?
		}
		attached_ = false;
	}

	assert(!attached_);
	if (event_add(event_, timeoutval) != 0)
	{
		std::cerr << "event_add failed. fd=" << this->event_->ev_fd << " event_=" << event_ << std::endl;
		return false;
	}
	attached_ = true;
	return true;
}

void NFEventWatcher::FreeEvent()
{
	if (event_)
	{
		if (attached_)
		{
			event_del(event_);
			attached_ = false;
		}

		delete (event_);
		event_ = nullptr;
	}
}

void NFEventWatcher::Cancel()
{
	assert(event_);
	FreeEvent();

	if (cancel_callback_)
	{
		cancel_callback_();
		cancel_callback_ = Handler();
	}
}

void NFEventWatcher::SetCancelCallback(const Handler& cb)
{
	cancel_callback_ = cb;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

NFPipeEventWatcher::NFPipeEventWatcher(NFEventLoop* loop,
                                       const Handler& handler)
	: NFEventWatcher(loop->event_base(), handler)
{
	memset(pipe_, 0, sizeof(pipe_[0] * 2));
}

NFPipeEventWatcher::NFPipeEventWatcher(NFEventLoop* loop,
                                       Handler&& h)
	: NFEventWatcher(loop->event_base(), std::move(h))
{
	memset(pipe_, 0, sizeof(pipe_[0] * 2));
}


NFPipeEventWatcher::~NFPipeEventWatcher()
{
	Close();
}

bool NFPipeEventWatcher::DoInit()
{
	assert(pipe_[0] == 0);

	if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_) < 0)
	{
		int err = errno;
		std::cerr << "create socketpair ERROR errno=" << err << " " << strerror(err) << std::endl;
		goto failed;
	}

	if (evutil_make_socket_nonblocking(pipe_[0]) < 0 ||
		evutil_make_socket_nonblocking(pipe_[1]) < 0)
	{
		goto failed;
	}

	::event_set(event_, pipe_[1], EV_READ | EV_PERSIST,
	            &NFPipeEventWatcher::HandlerFn, this);
	return true;
failed:
	Close();
	return false;
}

void NFPipeEventWatcher::DoClose()
{
	if (pipe_[0] > 0)
	{
		EVUTIL_CLOSESOCKET(pipe_[0]);
		EVUTIL_CLOSESOCKET(pipe_[1]);
		memset(pipe_, 0, sizeof(pipe_[0]) * 2);
	}
}

void NFPipeEventWatcher::HandlerFn(evutil_socket_t fd, short /*which*/, void* v)
{
	std::cout << "PipeEventWatcher::HandlerFn fd=" << fd << " v=" << v << std::endl;
	NFPipeEventWatcher* e = (NFPipeEventWatcher*)v;
#ifdef H_BENCHMARK_TESTING
	// Every time we only read 1 byte for testing the IO event performance.
	// We use it in the benchmark test program 
	//  1. evpp/benchmark/ioevent/evpp/
	//  1. evpp/benchmark/ioevent/fd_channel_vs_pipe_event_watcher/
		char buf[1];
#else
	char buf[128];
#endif
	int n = 0;

	if ((n = ::recv(e->pipe_[1], buf, sizeof(buf), 0)) > 0)
	{
		e->handler_();
	}
}

bool NFPipeEventWatcher::AsyncWait()
{
	return Watch(NFDuration());
}

void NFPipeEventWatcher::Notify()
{
	char buf[1] = {};

	if (::send(pipe_[0], buf, sizeof(buf), 0) < 0)
	{
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
NFTimerEventWatcher::NFTimerEventWatcher(NFEventLoop* loop,
                                         const Handler& handler,
                                         NFDuration timeout)
	: NFEventWatcher(loop->event_base(), handler)
	  , timeout_(timeout)
{
}

NFTimerEventWatcher::NFTimerEventWatcher(NFEventLoop* loop,
                                         Handler&& h,
                                         NFDuration timeout)
	: NFEventWatcher(loop->event_base(), std::move(h))
	  , timeout_(timeout)
{
}

NFTimerEventWatcher::NFTimerEventWatcher(struct event_base* loop,
                                         const Handler& handler,
                                         NFDuration timeout)
	: NFEventWatcher(loop, handler)
	  , timeout_(timeout)
{
}

NFTimerEventWatcher::NFTimerEventWatcher(struct event_base* loop,
                                         Handler&& h,
                                         NFDuration timeout)
	: NFEventWatcher(loop, std::move(h))
	  , timeout_(timeout)
{
}

bool NFTimerEventWatcher::DoInit()
{
	::event_set(event_, -1, 0, NFTimerEventWatcher::HandlerFn, this);
	return true;
}

void NFTimerEventWatcher::HandlerFn(evutil_socket_t /*fd*/, short /*which*/, void* v)
{
	NFTimerEventWatcher* h = (NFTimerEventWatcher*)v;
	h->handler_();
}

bool NFTimerEventWatcher::AsyncWait()
{
	return Watch(timeout_);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
NFSignalEventWatcher::NFSignalEventWatcher(evutil_socket_t signo, NFEventLoop* loop,
                                                          const Handler& handler)
	: NFEventWatcher(loop->event_base(), handler)
	  , signo_(signo)
{
	assert(signo_);
}

NFSignalEventWatcher::NFSignalEventWatcher(evutil_socket_t signo, NFEventLoop* loop,
                                                          Handler&& h)
	: NFEventWatcher(loop->event_base(), std::move(h))
	  , signo_(signo)
{
	assert(signo_);
}

bool NFSignalEventWatcher::DoInit()
{
	assert(signo_);
	signal_set(event_, signo_, NFSignalEventWatcher::HandlerFn, this);
	return true;
}

void NFSignalEventWatcher::HandlerFn(evutil_socket_t /*sn*/, short /*which*/, void* v)
{
	NFSignalEventWatcher* h = (NFSignalEventWatcher*)v;
	h->handler_();
}

bool NFSignalEventWatcher::AsyncWait()
{
	return Watch(NFDuration());
}

