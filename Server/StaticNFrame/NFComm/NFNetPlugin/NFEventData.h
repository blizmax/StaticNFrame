// -------------------------------------------------------------------------
//    @FileName         :    NFEventData.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/28
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFNetBase.h"

struct EventData;

class EventHandle
{
	typedef std::function<void(EventData*)> EventHandler;
public:
	EventHandle()
	{
		mReadHandler = EventHandler();
		mWriteHandler = EventHandler();
		mErrorHandler = EventHandler();
	}

	virtual ~EventHandle()
	{
	}

	EventHandler mReadHandler;
	EventHandler mWriteHandler;
	EventHandler mErrorHandler;
};

class NFPoll;

struct EventData
{
	EventHandle handle;
	SocketFlag sock_flag;
	EventFlag event_flag;
	NFPoll* poll;
	SOCKET sock;

	EventData();
	~EventData();
};
