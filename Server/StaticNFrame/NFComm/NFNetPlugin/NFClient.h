#pragma once

#include "NetObject.h"
#include "NFINet.h"
//#include "JyThread.h"
#include <thread>
#include "NFComm/NFCore/NFThread.hpp"

enum eConnectStatus
{
	eConnectStatus_UnConnect,    //未连接
	eConnectStatus_Connecting,	 //连接中
	eConnectStatus_ConnectOk,	 //连接OK
	eConnectStatus_Disconnect,   //断开连接
};

//uint32_t STDCALL OnClientThread(void * data);

class NFClient : public NetObject
{
public:
	NFClient();
	NFClient(uint32_t nId, const stClientFlag& flag);
	virtual ~NFClient();
public:
	template<typename BaseType>
	void SetRecvCB(BaseType* pBaseType, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	}

	template<typename BaseType>
	void SetEventCB(BaseType* pBaseType, void (BaseType::*handleEvent)(const eMsgType nEvent, const uint32_t unLinkId))
	{
		mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2);
	}
public:
	static void conn_recvcb(struct bufferevent* pEv, void *pArg);
	static void conn_eventcb(struct bufferevent* pEv, short events, void *pArg);
	static void conn_writecb(struct bufferevent* pEv, void *pArg);
	static void log_cb(int severity, const char* msg) { }
	static void timeout_ping(int fd, short event, void* params);
public:
	virtual bool Init() override;
	virtual bool Shut() override;
	virtual bool Execute() override;
public:
	virtual bool Ping(const stMsg* pMsg) override;
	virtual void ProcessMsgLogicThread();
public:
	const string& GetName() const;

	stClientFlag& Getflag();
	void		CheckConnect();
	bool		IsSuspend() const;
public:
	void		StartThread();
	event_base* GetMainBase() const;
	bool		Send(const void* pData, uint32_t unSize);
	void		SendConnected();
	void		SetSocketId(SOCKET nSocket);
	void		SendDisconnect();
	void		Close();
	bool		Connect();
	bool		Reconnect();
	void		OnConnectLib(SOCKET nSocket);
	void		OnDisConnectLib();
public:
	eConnectStatus GetStatus() const;

	void SetStatus(eConnectStatus val);

	bool IsConnectOK() const;

	virtual uint64_t GetPingTime() const override;

	void PingTime(uint64_t val);

	time_t GetOnRecvTime() const;

	void SetOnRecvTime(time_t val);

	void			OnClientThread();
private:
	event_base*					m_pMainBase;
	SOCKET						m_nSocketId;
	uint16_t					m_usPort;
	eConnectStatus				m_eStatus;
	time_t						m_unDisConnTime;
	uint64_t					m_pingTime;
	event*						m_pTimeoutEve;
	time_t						m_tOnRecvTime;           //移动响应时间
	NFThread					m_thread;       //线程对象
private:
	NET_RECEIVE_FUNCTOR			mRecvCB;
	NET_EVENT_FUNCTOR			mEventCB;
private:
	stClientFlag				m_flag;
	std::string					m_strName;
};
