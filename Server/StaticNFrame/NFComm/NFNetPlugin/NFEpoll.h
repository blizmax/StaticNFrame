// -------------------------------------------------------------------------
//    @FileName         :    NFEpoll.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFBasePoll.h"
#include "NFSocket.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <sys/epoll.h>

class NFEpoll : public NFPoll
{
protected:
	int                  max_fd_;
	int                  read_fd_;
	int                  write_fd_;
	PollType             poll_type_;
	int                  poll_index_;
	int                  poll_counts_;
	struct epoll_event*  read_events_;
	struct epoll_event*  write_events_;

public:
	NFEpoll();
	virtual ~NFEpoll();

public:
	virtual bool Init(int max_sock);
	virtual void UnInit();
	virtual bool Poll(bool poolWrite, bool poolRead, uint64_t timeout_ms);

public:
	virtual bool AddEvent(SOCKET sock, EventFlag flag, EventData* ptr);
	virtual bool ModEvent(SOCKET sock, EventFlag flag, EventData* ptr);
	virtual bool DelEvent(SOCKET sock, EventData* ptr);

protected:
	void _Release();
	bool _DelPollEvent(EventData* ptr);
	void _DelPollEvent(EventData* ptr, struct epoll_event* events);
	bool _CtlPollEvent(int poll_fd, SOCKET sock, EventData* ptr, int opt, uint32_t flag);
	bool _Poll(uint64_t timeout_ms);
	bool _PollRead(uint64_t timeout_ms);
	bool _PollWrite(uint64_t timeout_ms);
};

#endif