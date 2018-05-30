// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoop.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFEventLoop.h"
#include "NFLibEvent.h"
#include <iostream>
#include "NFEventWatcher.h"

NFEventLoop::NFEventLoop()
	: create_evbase_myself_(true), notified_(false), pending_functor_count_(0)
{
#if LIBEVENT_VERSION_NUMBER >= 0x02001500
	struct event_config* cfg = event_config_new();
	if (cfg)
	{
		// Does not cache time to get a preciser timer
		event_config_set_flag(cfg, EVENT_BASE_FLAG_NO_CACHE_TIME);
		evbase_ = event_base_new_with_config(cfg);
		event_config_free(cfg);
	}
#else
	evbase_ = event_base_new();
#endif
	Init();
}

NFEventLoop::NFEventLoop(struct event_base* base)
	: evbase_(base), create_evbase_myself_(false), notified_(false), pending_functor_count_(0)
{
	Init();

	// When we build an EventLoop instance from an existing event_base
	// object, we will never call EventLoop::Run() method.
	// So we need to watch the task queue here.
	bool rc = watcher_->AsyncWait();
	assert(rc);
	if (!rc)
	{
		std::cerr << "PipeEventWatcher init failed." << std::endl;
	}
	mStatus.store(kRunning);
}

NFEventLoop::~NFEventLoop()
{
	watcher_.reset();

	if (evbase_ != nullptr && create_evbase_myself_)
	{
		event_base_free(evbase_);
		evbase_ = nullptr;
	}

	delete pending_functors_;
	pending_functors_ = nullptr;
}

void NFEventLoop::Init()
{
	mStatus.store(kInitializing);

	this->pending_functors_ = new std::vector<Functor>();

	tid_ = std::this_thread::get_id(); // The default thread id

	InitNotifyPipeWatcher();

	mStatus.store(kInitialized);
}

void NFEventLoop::InitNotifyPipeWatcher()
{
	// Initialized task queue notify pipe watcher
	watcher_.reset(new NFPipeEventWatcher(this, std::bind(&NFEventLoop::DoPendingFunctors, this)));
	int rc = watcher_->Init();
	assert(rc);
	if (!rc)
	{
		std::cerr << "PipeEventWatcher init failed." << std::endl;
	}
}

void NFEventLoop::Run()
{
	mStatus.store(kStarting);
	tid_ = std::this_thread::get_id(); // The actual thread id

	int rc = watcher_->AsyncWait();
	assert(rc);
	if (!rc)
	{
		std::cerr << "PipeEventWatcher init failed." << std::endl;
	}

	// After everything have initialized, we set the status to kRunning
	mStatus.store(kRunning);

	rc = event_base_dispatch(evbase_);
	if (rc == 1)
	{
		std::cerr << "event_base_dispatch error: no event registered" << std::endl;
	}
	else if (rc == -1)
	{
		int serrno = errno;
		std::cerr << "event_base_dispatch error " << serrno << " " << strerror(serrno) << std::endl;
	}

	// Make sure watcher_ does construct, initialize and destruct in the same thread.
	watcher_.reset();
	std::cout << "EventLoop stopped, tid=" << std::this_thread::get_id() << std::endl;

	mStatus.store(kStopped);
}

void NFEventLoop::Stop()
{
	assert(mStatus.load() == kRunning);
	mStatus.store(kStopping);
	std::cout << "EventLoop::Stop" << std::endl;
	QueueInLoop(std::bind(&NFEventLoop::StopInLoop, this));
}

void NFEventLoop::StopInLoop()
{
	std::cerr << "EventLoop is stopping now, tid=" << std::this_thread::get_id() << std::endl;
	assert(mStatus.load() == kStopping);

	auto f = [this]()
		{
			for (int i = 0;; i++)
			{
				std::cout << "calling DoPendingFunctors index=" << i << std::endl;
				DoPendingFunctors();
				if (IsPendingQueueEmpty())
				{
					break;
				}
			}
		};

	std::cout << "before event_base_loopexit, we invoke DoPendingFunctors" << std::endl;

	f();

	std::cout << "start event_base_loopexit";
	event_base_loopexit(evbase_, nullptr);
	std::cout << "after event_base_loopexit, we invoke DoPendingFunctors" << std::endl;

	f();

	std::cout << "end of StopInLoop" << std::endl;
}

void NFEventLoop::RunInLoop(const Functor& functor)
{
	if (IsRunning() && IsInLoopThread())
	{
		functor();
	}
	else
	{
		QueueInLoop(functor);
	}
}

void NFEventLoop::RunInLoop(Functor&& functor)
{
	if (IsRunning() && IsInLoopThread())
	{
		functor();
	}
	else
	{
		QueueInLoop(std::move(functor));
	}
}

void NFEventLoop::QueueInLoop(const Functor& cb)
{
	std::cout << "pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		pending_functors_->emplace_back(cb);
	}
	++pending_functor_count_;
	std::cout << "queued a new Functor. pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
	if (!notified_.load())
	{
		std::cout << "call watcher_->Nofity() notified_.store(true)" << std::endl;

		// We must set notified_ to true before calling `watcher_->Nodify()`
		// otherwise there is a change that:
		//  1. We called watcher_- > Nodify() on thread1
		//  2. On thread2 we watched this event, so wakeup the CPU changed to run this EventLoop on thread2 and executed all the pending task
		//  3. Then the CPU changed to run on thread1 and set notified_ to true
		//  4. Then, some thread except thread2 call this QueueInLoop to push a task into the queue, and find notified_ is true, so there is no change to wakeup thread2 to execute this task
		notified_.store(true);

		// Sometimes one thread invoke EventLoop::QueueInLoop(...), but anther
		// thread is invoking EventLoop::Stop() to stop this loop. At this moment
		// this loop maybe is stopping and the watcher_ object maybe has been
		// released already.
		if (watcher_)
		{
			watcher_->Notify();
		}
		else
		{
			std::cout << "status=" << StatusToString() << std::endl;
			assert(!IsRunning());
		}
	}
	else
	{
		std::cout << "No need to call watcher_->Nofity()" << std::endl;
	}
}

void NFEventLoop::QueueInLoop(Functor&& cb)
{
	std::cout << "pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		pending_functors_->emplace_back(std::move(cb));
	}
	++pending_functor_count_;
	std::cout << "queued a new Functor. pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
	if (!notified_.load())
	{
		std::cout << "call watcher_->Nofity() notified_.store(true)" << std::endl;
		notified_.store(true);
		if (watcher_)
		{
			watcher_->Notify();
		}
		else
		{
			std::cout << "watcher_ is empty, maybe we call EventLoop::QueueInLoop on a stopped EventLoop. status=" << StatusToString() << std::endl;
			assert(!IsRunning());
		}
	}
	else
	{
		std::cout << "No need to call watcher_->Nofity()" << std::endl;
	}
}

event_base* NFEventLoop::event_base()
{
	return evbase_;
}

void NFEventLoop::DoPendingFunctors()
{
	std::cout << "pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;

	std::vector<Functor> functors;
	{
		std::lock_guard<std::mutex> lock(mutex_);
		notified_.store(false);
		pending_functors_->swap(functors);
		std::cout << "pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
	}
	std::cout << "pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
		--pending_functor_count_;
	}
	std::cout << "pending_functor_count_=" << pending_functor_count_ << " PendingQueueSize=" << GetPendingQueueSize() << " notified_=" << notified_.load() << std::endl;
}

size_t NFEventLoop::GetPendingQueueSize()
{
	return pending_functors_->size();
}

bool NFEventLoop::IsPendingQueueEmpty()
{
	return pending_functors_->empty();
}

