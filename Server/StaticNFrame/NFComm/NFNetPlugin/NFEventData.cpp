// -------------------------------------------------------------------------
//    @FileName         :    NFEventData.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/30
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFEventData.h"
#include "NFPoll.h"
#include "NFSocket.h"
#include <NFComm/NFPluginModule/NFLogMgr.h>

EventData::EventData()
{
	mSockFlag = SOCKET_ACCEPT;
	mEventFlag = EVENT_NULL;
	mSock = INVALID_SOCKET;
	mPoll = CreateModule();
	mWriteFlag = false;
	mConnected = false;
}

EventData::~EventData()
{
	DeleteModule(mPoll);
}

bool EventData::Init(int max_sock)
{
	bool retCode = mPoll->Init(max_sock);
	if (!retCode)
	{
		NFLogError("Net poll init fail\n");
		return false;
	}
	return true;
}

bool EventData::Connect(const char* ip, uint16_t port)
{
	SOCKET sock = NFSocket::Create();
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	int retCode = NFSocket::Connect(sock, ip, port);
	if (!retCode)
	{
		NFSocket::Close(sock);
		event_cb(EVENT_DISCONNECT);
		return false;
	}

	mSockFlag = SOCKET_CONNECT;
	mSock = sock;
	mWriteFlag = false;

	retCode = mPoll->AddEvent(sock, EVENT_WRITE, this);
	if (!retCode)
	{
		NFSocket::Close(sock);
		event_cb(EVENT_DISCONNECT);
		return false;
	}
	return true;
}

bool EventData::Send(const char* buf, uint32_t len)
{
	int retCode = 0;

	if (len > 0 && mConnected)
	{
		retCode = NFSocket::Send(mSock, buf, len);
		if (retCode < 0)
		{
			return false;
		}
	}
	return true;
}

int EventData::Recv(char* buf, uint32_t size)
{
	int ret = NFSocket::Recv(mSock, buf, size);
	if (ret < 0)
	{// err
		if (ret == -2)
		{// EAGAIN/WSAEWOULDBLOCK
			return 0;
		}
		return -1;
	}
	else if (ret == 0)
	{// remote close
		return -1;
	}
	return ret;
}

void EventData::Close()
{
	mPoll->DelEvent(mSock, this);
	NFSocket::Close(mSock);
	mSockFlag = SOCKET_ACCEPT;
	mEventFlag = EVENT_NULL;
	mSock = INVALID_SOCKET;
	mWriteFlag = false;
	mConnected = false;
	mHandle.Clear();
}

bool EventData::Dispatch(uint32_t timeout)
{
	mPoll->Poll(false, true, timeout);
	mPoll->Poll(true, false, timeout);
	return true;
}

void EventData::recv_cb()
{
	if (mHandle.mEventHandler)
	{
		mHandle.mReadHandler(mHandle.pArg);
	}
}

void EventData::event_cb(short events)
{
	if (events & EVENT_CONNECT)
	{
		NF_ASSERT(mSockFlag == SOCKET_CONNECT);
		mSockFlag = SOCKET_CONNECT;
		mPoll->ModEvent(mSock, EVENT_READ, this);
		mWriteFlag = true;
		mConnected = true;
	}
	else if (events & EVENT_DISCONNECT || events & EVENT_ERROR)
	{
		Close();
		mConnected = false;
	}
	else if (events & EVENT_WRITE)
	{
		mPoll->ModEvent(mSock, EVENT_READ, this);
	}

	if (mHandle.mEventHandler)
	{
		mHandle.mEventHandler(events, mHandle.pArg);
	}
}

void EventData::set_cb(NetRecvHandler recvcb, NetEventHandler eventcb, void* pArg)
{
	mHandle.mReadHandler = recvcb;
	mHandle.mEventHandler = eventcb;
	mHandle.pArg = pArg;
}