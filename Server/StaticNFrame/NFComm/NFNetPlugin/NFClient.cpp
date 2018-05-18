#include "NFClient.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

void NFClient::conn_recvcb(struct bufferevent* pEv, void *pArg)
{
	NFClient*	pClient = (NFClient*)pArg;
	if (pClient == nullptr) return;

	if (!pClient->OnRecvData(pEv))
	{
		pClient->OnDisConnectLib();
	}
	pClient->SetOnRecvTime(NFGetTime());
}

void NFClient::conn_writecb(struct bufferevent* pEv, void *pArg)
{
	// Intentionally unimplemented...
}

void NFClient::conn_eventcb(struct bufferevent* pEv, short what, void *pArg)
{
	NFClient* p = (NFClient*)pArg;
	if (p == nullptr) return;

	if (what & BEV_EVENT_CONNECTED){
		p->OnConnectLib(static_cast<SOCKET>(bufferevent_getfd(pEv)));
	}
	if (what & BEV_EVENT_EOF)
	{
		p->OnDisConnectLib();
		return;
	}

	if (what & BEV_EVENT_ERROR) {
#ifdef _WIN32
		if (ArkGetLastError() == WSAEISCONN)
		{
			p->SetSocketId(static_cast<SOCKET>(bufferevent_getfd(pEv)));
			p->SendConnected();
			p->SetStatus(eConnectStatus_ConnectOk);
			return;
		}
#endif
		p->OnDisConnectLib();
		LogWarning(0, "NetWarning", " CloseProc Error Code " + std::string(evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR())));
	}
}

NFClient::NFClient()
{
	m_usPort = 0;
	m_eStatus = eConnectStatus_UnConnect;
	m_pMainBase=NULL;
	m_nSocketId = INVALID_SOCKET;
	m_unDisConnTime = 0;
	m_pingTime = 0;
	m_pTimeoutEve = NULL;
	m_tOnRecvTime=0;
}

NFClient::NFClient(uint32_t nId, const stClientFlag& flag):m_flag(flag) 
{
	m_usPort = 0;
	m_eStatus = eConnectStatus_UnConnect;
	m_pMainBase=NULL;
	m_nSocketId = INVALID_SOCKET;
	m_unDisConnTime = 0;
	m_pTimeoutEve = NULL;
	m_tOnRecvTime=0;
	m_usLinkId = nId;
	m_pingTime = 0;
}

NFClient::~NFClient() 
{
    bufferevent_free(m_pBev);
	event_base_free(m_pMainBase);
	m_pMainBase = NULL;
	m_pBev = NULL;
}

bool NFClient::Init()
{
	NetObject::Init();
	m_pMainBase = event_base_new();
	if (NULL == m_pMainBase) 
	{
		LogError(0, "NetError", "error: client event_base_new failed!");
		return false;
	}

	StartThread();
	return true;
}

bool NFClient::Shut()
{
	Close();
	SetStatus(eConnectStatus_UnConnect);
	return true;
}

bool NFClient::Execute()
{
	ProcessMsgLogicThread();
	return true;
}

void NFClient::ProcessMsgLogicThread()
{
	std::vector<NFThreadNetMsg*> vecMsg;
	m_threadNetMsgs.Pop(vecMsg);

	for (int i = 0; i < (int)vecMsg.size(); i++)
	{
		NFThreadNetMsg* pBuff = vecMsg[i];
		if (pBuff == nullptr) continue;

		switch (pBuff->eType)
		{
		case eMsgType_RECIVEDATA:
		{
			if (mRecvCB)
			{
				mRecvCB(pBuff->usLink, pBuff->nValue, pBuff->nMsgId, (const char*)pBuff->nBuffer.data(), pBuff->nBuffer.size());
			}
		}
		break;
		case eMsgType_CONNECTED:
		{
			if (mEventCB)
			{
				mEventCB(eMsgType_CONNECTED, pBuff->usLink);
			}
		}
		break;
		case eMsgType_DISCONNECTED:
		{
			if (mEventCB)
			{
				mEventCB(eMsgType_DISCONNECTED, pBuff->usLink);
			}
		}
		break;
		default:
		break;
		}
		NFSafeDelete(pBuff);
	}
}

void NFClient::CheckConnect()
{
	if (m_flag.bAutoConnect){
		NFClient::Reconnect();
	}
}

bool NFClient::Send(const void* pData, uint32_t unSize)
{
	if (IsConnectOK() && bufferevent_get_enabled(m_pBev) != 0){
// 		int32 size = evbuffer_get_length(bufferevent_get_output(m_pBev));
// 		if (size > 4096) {
// 			LOG(ERROR) << "connector libevent chache too big close size: " << size << " LinkId: " << GetId();
// 		}
		int nRet = bufferevent_write(m_pBev, pData, unSize);
		if (nRet == -1) {
			LogError(0, "NetError", "send msg error !");
			//LError("NetLog", "send msg error !");
		}
 		return true;
 	}
	return false;
}

void NFClient::SendConnected()
{
	NFThreadNetMsg* pNetMsg = NF_NEW NFThreadNetMsg();
	pNetMsg->usLink = m_usLinkId;
	pNetMsg->eType = eMsgType_CONNECTED;

	m_threadNetMsgs.Push(pNetMsg);
}

void NFClient::SendDisconnect()
{
	NFThreadNetMsg* pNetMsg = NF_NEW NFThreadNetMsg();
	pNetMsg->usLink = m_usLinkId;
	pNetMsg->eType = eMsgType_DISCONNECTED;

	m_threadNetMsgs.Push(pNetMsg);
}

void NFClient::Close()
{
	if (m_pTimeoutEve)
	{
		evtimer_del(m_pTimeoutEve);
	}
	SetStatus(eConnectStatus_Disconnect);
	m_unDisConnTime = time(NULL);
	Reset();
	event_base_loopbreak(m_pMainBase);
}

void NFClient::timeout_ping(int fd, short event, void* params)
{
	NFClient* p = (NFClient*)params;
	if (p == nullptr) return;

	if (p->GetStatus() == eConnectStatus_ConnectOk){
		//stMsg msg(MSG_Net_Ping);
		//msg.wSZ = sizeof(msg);
		//p->PingTime(NFGetTime());
		//p->Send(&msg, sizeof(msg));
	}
}

bool NFClient::Connect()
{
    if (m_pBev){
		bufferevent_free(m_pBev);
		m_pBev = NULL;
    }
	m_pBev = bufferevent_socket_new(GetMainBase(), -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	struct sockaddr_in  sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, m_flag.strIP.c_str(), (void*)&sin.sin_addr.s_addr);
	sin.sin_port = htons(m_flag.nPort);
	bufferevent_setcb(m_pBev, conn_recvcb, conn_writecb, conn_eventcb, this);

	//设置水位为0
	bufferevent_setwatermark(m_pBev, EV_READ, 0, 0);
	bufferevent_setwatermark(m_pBev, EV_WRITE, 0, 0);

	bufferevent_enable(m_pBev, EV_WRITE | EV_READ);
	event_set_log_callback(&NFClient::log_cb);


	if (bufferevent_socket_connect(m_pBev, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		LogError(0, "NetError", "connect failed! IP: " + m_flag.strIP+ " port:" + lexical_cast<std::string>(m_flag.nPort));
		//LError("NetLog", "connect failed! IP: " << m_strIP << " port: " << m_usPort);
		return false;
	}

	struct timeval tv = { 5, 0 };
	m_pTimeoutEve = event_new(m_pMainBase, -1, EV_PERSIST, timeout_ping, this);
	evtimer_add(m_pTimeoutEve, &tv);

	//test
	//struct timeval tv1 = { 0, 3 };
	//m_pBufSZEve = event_new(m_pMainBase, -1, EV_PERSIST, buffeve_link_client, this);
	//evtimer_add(m_pBufSZEve, &tv1);

	return true;
}

void NFClient::OnClientThread()
{
	Connect();
	event_base_dispatch(GetMainBase());	
}

void NFClient::StartThread()
{
//	m_thread = std::make_shared<std::thread>(&NFClient::OnClientThread, this);
//	m_thread.StopThread();
	m_thread.StartThread(this, &NFClient::OnClientThread);
}

bool NFClient::Reconnect()
{
	if (eConnectStatus_Disconnect == m_eStatus){
		time_t nCurTime = time(NULL);
		if (nCurTime - m_unDisConnTime > 5){     //5s重启
			SetStatus(eConnectStatus_Connecting);
			StartThread();
		}
		return true;
	}
	return false;
}


bool NFClient::Ping(const stMsg* pMsg)
{
	int32_t ntime = (int32_t)(NFGetTime() - GetPingTime());
	
	if (ntime > 50)
	{
		LogWarning(0, "NetWarning", "Ping time : " + lexical_cast<std::string>(ntime/2));
		//LOG(WARNING)<<" Ping time : "<< ntime/2;
	}
	
	return true;
}

void NFClient::OnConnectLib(SOCKET nSocket)
{
	SetSocketId(nSocket);
	SendConnected();
	SetStatus(eConnectStatus_ConnectOk);
}

void NFClient::OnDisConnectLib ()
{
	SendDisconnect();
	Close();
}

void NFClient::SetSocketId(SOCKET nSocket)
{
	m_nSocket = nSocket;
	int tcp_nodelay = 1;
	int ret = 0;
	ret = setsockopt(nSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&tcp_nodelay, sizeof(tcp_nodelay));
	if (ret < 0)
	{
		std::cout << "setsockopt TCP_NODELAY failed" << std::endl;
	}
	int bufflen = 65536;
	ret = setsockopt(nSocket, SOL_SOCKET, SO_RCVBUF, (const char *)&bufflen, sizeof(bufflen));
	if (ret < 0)
	{
		std::cout << "setsockopt SO_RCVBUF failed" << std::endl;
	}
	ret = setsockopt(nSocket, SOL_SOCKET, SO_SNDBUF, (const char *)&bufflen, sizeof(bufflen));
	if (ret < 0)
	{
		std::cout << "setsockopt SO_SNDBUF failed" << std::endl;
	}

	ret = evutil_make_socket_nonblocking(nSocket);
	if (ret < 0)
	{
		std::cout << "evutil_make_socket_nonblocking failed" << std::endl;
	}
}
