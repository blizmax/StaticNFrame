// -------------------------------------------------------------------------
//    @FileName         :    NFSelect.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/27
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFPoll.h"

class EventData;

class NFSelect : public NFPoll
{
protected:
	typedef std::map<SOCKET, EventData*> EVENT_MAP;

	SOCKET max_sock_;
	timeval timeout_;
	fd_set recv_set_;
	fd_set send_set_;
	fd_set error_set_;
	fd_set loop_recv_;
	fd_set loop_send_;
	fd_set loop_error_;
	EVENT_MAP event_map_;

public:
	NFSelect();
	virtual ~NFSelect();

public:
	virtual bool Init(int max_sock);
	virtual void UnInit();
	virtual bool Poll(bool poolWrite, bool poolRead, uint64_t timeout_ms);

public:
	virtual bool AddEvent(SOCKET sock, EventFlag flag, EventData* ptr);
	virtual bool ModEvent(SOCKET sock, EventFlag flag, EventData* ptr);
	virtual bool DelEvent(SOCKET sock, EventData* ptr);

protected:
	bool _EventLoop(uint64_t timeout_ms, PollType poll_type);

protected:
	int _ResetEvent();
};
