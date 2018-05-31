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

typedef std::function<void(void* pArg)> NetRecvHandler;
typedef std::function<void(short events, void* pArg)> NetEventHandler;

class EventHandle
{
public:

public:
	EventHandle()
	{
		Clear();
	}

	virtual ~EventHandle()
	{
	}

	void Clear()
	{
		mReadHandler = NetRecvHandler();
		mEventHandler = NetEventHandler();
		pArg = nullptr;
	}

	NetRecvHandler mReadHandler;
	NetEventHandler mEventHandler;
	void* pArg;
};

class NFPoll;

class EventData
{
public:
	EventHandle mHandle;
	SocketFlag mSockFlag;
	EventFlag mEventFlag;
	NFPoll* mPoll;
	SOCKET mSock;
	bool mWriteFlag;
	bool mConnected;

	EventData();
	~EventData();

	bool Init(int max_sock);

	bool Connect(const char* ip, uint16_t port);

	bool Send(const char* buf, uint32_t len);

	int Recv(char* buf, uint32_t size);

	void Close();

	bool Dispatch(uint32_t timeout);

	void recv_cb();

	void event_cb(short events);

	void set_cb(NetRecvHandler recvcb, NetEventHandler eventcb, void *pArg);
};
