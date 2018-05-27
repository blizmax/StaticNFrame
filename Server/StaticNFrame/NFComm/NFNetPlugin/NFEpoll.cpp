// -------------------------------------------------------------------------
//    @FileName         :    NFEpoll.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFEpoll.h"
#include <NFComm/NFPluginModule/NFLogMgr.h>
#include "NFComm/NFCore/NFLikely.h"

#include <errno.h>

#if NF_PLATFORM == NF_PLATFORM_LINUX

NFEpoll::NFEpoll()
{
	max_fd_ = 0;
	read_fd_ = -1;
	write_fd_ = -1;
	poll_type_ = POLL_TYPE_NULL;
	poll_index_ = -1;
	poll_counts_ = 0;
	read_events_ = NULL;
	write_events_ = NULL;
}

NFEpoll::~NFEpoll()
{}

bool NFEpoll::Init(int max_sock)
{
	bool result = false;

	max_fd_ = max_sock;
	read_fd_ = epoll_create(max_fd_);
	if (read_fd_ < 0)
		goto Exit0;

	write_fd_ = epoll_create(max_fd_);
	if (write_fd_ < 0)
		goto Exit0;

	read_events_ = new epoll_event[max_fd_];
	if (read_events_ == NULL)
		goto Exit0;

	write_events_ = new epoll_event[max_fd_];
	if (write_events_ == NULL)
		goto Exit0;

	result = true;
Exit0:
	if (!result)
	{
		_Release();
	}
	return result;
}

void NFEpoll::UnInit()
{
	_Release();
}

void NFEpoll::_Release()
{
	if (write_events_)
	{
		delete[] write_events_;
		write_events_ = NULL;
	}
	if (read_events_)
	{
		delete[] read_events_;
		read_events_ = NULL;
	}
	if (write_fd_ != -1)
	{
		close(write_fd_);
		write_fd_ = -1;
	}
	if (read_fd_ != -1)
	{
		close(read_fd_);
		read_fd_ = -1;
	}
	max_fd_ = 0;
}

bool NFEpoll::Poll(bool poolWrite, bool poolRead, uint64_t timeout_ms)
{
	if (poolWrite)
	{
		_PollWrite(timeout_ms);
	}
	if (poolRead)
	{
		_PollRead(timeout_ms);
	}
	return true;
}

bool NFEpoll::AddEvent(SOCKET sock, EventFlag flag, EventData* ptr)
{
	// read poll event
	uint32_t event_type = (EPOLLHUP | EPOLLERR);
	if (flag & EVENT_READ)
	{
		event_type |= EPOLLIN;
	}
	if (!_CtlPollEvent(read_fd_, sock, ptr, EPOLL_CTL_ADD, event_type))
	{
		return false;
	}
	// write poll event
	event_type = (EPOLLHUP | EPOLLERR);
	if (flag & EVENT_WRITE)
	{
		event_type |= EPOLLOUT;
	}
	if (!_CtlPollEvent(write_fd_, sock, ptr, EPOLL_CTL_ADD, event_type))
	{
		return false;
	}
	EventData* data = reinterpret_cast<EventData*>(ptr);
	data->event_flag = flag;
	return true;
}

bool NFEpoll::ModEvent(SOCKET sock, EventFlag flag, EventData* ptr)
{
	EventData* data = reinterpret_cast<EventData*>(ptr);
	if (data->event_flag == flag)
	{
		return true;
	}
	// read poll event
	uint32_t event_type = (EPOLLHUP | EPOLLERR);
	if (flag & EVENT_READ)
	{
		event_type |= EPOLLIN;
	}
	if (!_CtlPollEvent(read_fd_, sock, ptr, EPOLL_CTL_MOD, event_type))
	{
		return false;
	}
	// write poll event
	event_type = (EPOLLHUP | EPOLLERR);
	if (flag & EVENT_WRITE)
	{
		event_type |= EPOLLOUT;
	}
	if (!_CtlPollEvent(write_fd_, sock, ptr, EPOLL_CTL_MOD, event_type))
	{
		return false;
	}
	data->event_flag = flag;
	return true;
}

bool NFEpoll::DelEvent(SOCKET sock, EventData* ptr)
{
	_DelPollEvent(ptr);
	if (!_CtlPollEvent(read_fd_, sock, ptr, EPOLL_CTL_DEL, 0))
	{
		return false;
	}
	if (!_CtlPollEvent(write_fd_, sock, ptr, EPOLL_CTL_DEL, 0))
	{
		return false;
	}
	return true;
}

bool NFEpoll::_DelPollEvent(EventData* ptr)
{
	if (poll_counts_ <= 0)
		return false;

	assert(poll_index_ < poll_counts_);

	if (poll_type_ == POLL_TYPE_READ)
	{
		_DelPollEvent(ptr, read_events_);
	}
	else if (poll_type_ == POLL_TYPE_WRITE)
	{
		_DelPollEvent(ptr, write_events_);
	}
	else
	{
		assert(false);
	}
	return true;
}

void NFEpoll::_DelPollEvent(EventData* ptr, struct epoll_event* events)
{
	if (events[poll_index_].data.ptr == ptr)
	{
		return;
	}
	for (int i = poll_index_ + 1; i < poll_counts_; ++i)
	{
		if (events[i].data.ptr == ptr)
		{
			events[i].data.ptr = NULL;
			return;
		}
	}
	return;
}

bool NFEpoll::_CtlPollEvent(int poll_fd, SOCKET sock, EventData* ptr, int opt, uint32_t flag)
{
	struct epoll_event epev = { flag,{ 0 } };
	epev.data.ptr = ptr;
	int ret = epoll_ctl(poll_fd, opt, sock, &epev);
	if (UNLIKELY(ret == -1))
	{
		NFLogError("[Net] epoll_ctl fd(%d) err(%d):%s", sock, ERRNO, ERRSTR);
		return false;
	}
	else
	{
		return true;
	}
}

bool NFEpoll::_Poll(uint64_t timeout_ms)
{
	bool       result = false;
	EventData* data = NULL;
	EventHandle*    handle = NULL;
	int        poll_fd = -1;
	struct epoll_event* poll_events = NULL;

	if (poll_type_ == POLL_TYPE_READ)
	{
		poll_fd = read_fd_;
		poll_events = read_events_;
	}
	else
	{
		poll_fd = write_fd_;
		poll_events = write_events_;
	}

	poll_counts_ = epoll_wait(poll_fd, poll_events, max_fd_, timeout_ms);
	if (poll_counts_ == -1)
	{
		if (ERRNO == ERR_INTER)
		{
			goto Exit1;
		}
		else
		{
			NFLogError("[Net] epoll_wait() err(%d):%s"
				, ERRNO, ERRSTR);
			goto Exit0;
		}
	}

	// handle read
	if (poll_type_ == POLL_TYPE_READ)
	{
		for (poll_index_ = 0; poll_index_ < poll_counts_; ++poll_index_)
		{
			data = reinterpret_cast<EventData*>(poll_events[poll_index_].data.ptr);
			if (data == NULL)
			{  // delete by other int net pack handle
				continue;
			}

			if (poll_events[poll_index_].events & EPOLLERR)
			{
				data->handle.mErrorHandler(data);
				continue;
			}
			//if shutdown SHUTDOWN_SEND sock will recv epollin event and epollhup event
			//because socket can read
			if (poll_events[poll_index_].events & (EPOLLIN | EPOLLHUP))
			{
				data->handle.mReadHandler(data);
			}
		}
	}
	else if (poll_type_ == POLL_TYPE_WRITE)
	{  // handle write
		for (poll_index_ = 0; poll_index_ < poll_counts_; ++poll_index_)
		{
			data = reinterpret_cast<EventData*>(poll_events[poll_index_].data.ptr);
			if (data == NULL)
			{  // delete by other int net pack handle
				continue;
			}
			if (poll_events[poll_index_].events & EPOLLERR)
			{
				data->handle.mErrorHandler(data);
				continue;
			}
			//can not handle event EPOLLHUP because socket can not write
			if (poll_events[poll_index_].events & EPOLLOUT)
			{
				//std::cout << poll_events[poll_index_].events << std::endl;
				data->handle.mWriteHandler(data);
			}
		}
	}
	else
	{
		assert(false);
	}

Exit1:
	result = true;
Exit0:
	poll_index_ = -1;
	poll_counts_ = 0;
	return result;
}

bool NFEpoll::_PollRead(uint64_t timeout_ms)
{
	bool       result = false;

	poll_type_ = POLL_TYPE_READ;
	if (!_Poll(timeout_ms))
	{
		goto Exit0;
	}
	result = true;
Exit0:
	poll_type_ = POLL_TYPE_NULL;
	return result;
}

bool NFEpoll::_PollWrite(uint64_t timeout_ms)
{
	bool       result = false;

	poll_type_ = POLL_TYPE_WRITE;
	if (!_Poll(timeout_ms))
	{
		goto Exit0;
	}
	result = true;
Exit0:
	poll_type_ = POLL_TYPE_NULL;
	return result;
}

#endif
