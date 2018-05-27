// -------------------------------------------------------------------------
//    @FileName         :    NFSelect.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFSelect.h"
#include "NFSocket.h"

#include <errno.h>
#include <NFComm/NFPluginModule/NFLogMgr.h>

Select::Select()
	: max_sock_(0),
	timeout_()
{
	FD_ZERO(&recv_set_);
	FD_ZERO(&send_set_);
	FD_ZERO(&error_set_);
	FD_ZERO(&loop_recv_);
	FD_ZERO(&loop_send_);
	FD_ZERO(&loop_error_);
}

Select::~Select()
{}

bool Select::Init(int max_sock)
{
	return true;
}

void Select::UnInit()
{
	max_sock_ = 0;
	FD_ZERO(&recv_set_);
	FD_ZERO(&send_set_);
	FD_ZERO(&error_set_);
	FD_ZERO(&loop_recv_);
	FD_ZERO(&loop_send_);
	FD_ZERO(&loop_error_);
}

bool Select::Poll(bool poolWrite, bool poolRead, uint64_t timeout_ms)
{
	if (poolWrite)
	{
		_EventLoop(timeout_ms, POLL_TYPE_WRITE);
	}
	if (poolRead)
	{
		_EventLoop(timeout_ms, POLL_TYPE_READ);
	}
	return true;
}

bool  Select::_EventLoop(uint64_t timeout_ms, PollType poll_type)
{
	int ret = -1;
	timeout_.tv_sec = (long)timeout_ms / 1000;
	timeout_.tv_usec = (long)(timeout_ms % 1000) * 1000;

#if NF_PLATFORM == NF_PLATFORM_WIN
	if (recv_set_.fd_count == 0
		&& send_set_.fd_count == 0
		&& error_set_.fd_count == 0)
	{
		return true;
	}
#endif
	if (poll_type == POLL_TYPE_READ)
	{
		memcpy(&loop_recv_, &recv_set_, sizeof(loop_recv_));
	}
	else if (poll_type == POLL_TYPE_WRITE)
	{
		memcpy(&loop_send_, &send_set_, sizeof(loop_send_));
	}
	else
	{
		assert(false);
	}
	memcpy(&loop_error_, &error_set_, sizeof(loop_error_));

	int max_fd = 0;
#if NF_PLATFORM == NF_PLATFORM_LINUX
	max_fd = max_sock_ + 1;
#endif  // OS_LINUX

	ret = select(max_fd,
		(poll_type == POLL_TYPE_READ) ? &loop_recv_ : NULL,
		(poll_type == POLL_TYPE_WRITE) ? &loop_send_ : NULL,
		&loop_error_,
		&timeout_);

	if (ret == -1)
	{
		if (ERRNO == ERR_INTER)
		{
			return true;
		}
		else
		{
			NFLogError("[Net] select() err(%d):%s", ERRNO, ERRSTR);
			return false;
		}
	}

	SOCKET     sock = INVALID_SOCKET;
	EventData* data = NULL;
	Handle*    handle = NULL;

	// todo: 遍历map比较低效
	EVENT_MAP::iterator it = event_map_.begin();
	EVENT_MAP::iterator it_use = it;

	// handle read
	if (poll_type == POLL_TYPE_READ)
	{
		for (; it != event_map_.end();)
		{
			it_use = it++;
			sock = static_cast<SOCKET>(it_use->first);
			data = reinterpret_cast<EventData*>(it_use->second);
			handle = reinterpret_cast<Handle*>(data->handle);

			if (FD_ISSET(sock, &loop_error_))
			{
				handle->Error(data);
				continue;
			}

			if (FD_ISSET(sock, &loop_recv_))
			{
				if (!handle->Readable(data))
				{
					continue;
				}
			}
		}
	}
	else if (poll_type == POLL_TYPE_WRITE)
	{  // handle write
		for (; it != event_map_.end();)
		{
			it_use = it++;
			sock = static_cast<SOCKET>(it_use->first);
			data = reinterpret_cast<EventData*>(it_use->second);
			handle = reinterpret_cast<Handle*>(data->handle);

			if (FD_ISSET(sock, &loop_error_))
			{
				handle->Error(data);
				continue;
			}
			if (FD_ISSET(sock, &loop_send_))
			{
				handle->Writable(data);
			}
		}
	}
	else
	{
		assert(false);
	}

	return true;
}

bool Select::AddEvent(SOCKET sock, EventFlag flag, void* ptr)
{
	// todo:遍历中自增宕机
	FD_SET(sock, &error_set_);

	if (flag & EVENT_READ) {
		FD_SET(sock, &recv_set_);
	}

	if (flag & EVENT_WRITE) {
		FD_SET(sock, &send_set_);
	}

#if NF_PLATFORM == NF_PLATFORM_LINUX
	if (sock > max_sock_)
		max_sock_ = sock;
#endif  // OS_LINUX

	event_map_[sock] = ptr;
	EventData* data = reinterpret_cast<EventData*>(ptr);
	data->event_flag = flag;
	return true;
}

bool Select::ModEvent(SOCKET sock, EventFlag flag, void* ptr)
{
	EventData* data = reinterpret_cast<EventData*>(ptr);
	if (data->event_flag == flag)
	{
		return true;
	}

	FD_SET(sock, &error_set_);

	if (flag & EVENT_READ)
	{
		FD_SET(sock, &recv_set_);
	}
	else
	{
		FD_CLR(sock, &recv_set_);
	}

	if (flag & EVENT_WRITE)
	{
		FD_SET(sock, &send_set_);
	}
	else
	{
		FD_CLR(sock, &send_set_);
	}

	data->event_flag = flag;
	event_map_[sock] = ptr;
	return true;
}

bool Select::DelEvent(SOCKET sock, void* ptr)
{
	event_map_.erase(sock);
	FD_CLR(sock, &recv_set_);
	FD_CLR(sock, &send_set_);
	FD_CLR(sock, &error_set_);
	return true;
}

