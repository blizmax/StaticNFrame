// -------------------------------------------------------------------------
//    @FileName         :    NFClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-05-24
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFClient.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"

/**
	* @brief libevent读数据回调
	*
	* @param pEv   libevent读写数据类
	* @param pArg  传入的参数
	* @return
	*/
static void conn_recvcb(struct bufferevent* pEv, void* pArg);

/**
	* @brief libevent连接事件回调
	*
	* @param pEv		libevent读写数据类
	* @param events	事件
	* @param pArg		传入的参数
	* @return
	*/
static void conn_eventcb(struct bufferevent* pEv, short events, void* pArg);

/**
	* @brief libevent写数据回调
	*
	* @param pEv   libevent读写数据类
	* @param pArg  传入的参数
	* @return
	*/
static void conn_writecb(struct bufferevent* pEv, void* pArg);

/**
	* @brief log回调
	*
	* @param severity
	* @param msg		要打印的消息
	* @return
	*/
static void log_cb(int severity, const char* msg);

NFClient::NFClient(uint32_t nId, const NFClientFlag& flag) : m_pMainBase(nullptr)
{
}

NFClient::NFClient() : m_pMainBase(nullptr)
{
}

NFClient::~NFClient()
{
	event_base_free(m_pMainBase);
	m_pMainBase = nullptr;
}

void conn_recvcb(struct bufferevent* pEv, void *pArg)
{
	NFClient*	pClient = static_cast<NFClient*>(pArg);
	if (pClient == nullptr) return;

	if (!pClient->OnRecvData(pEv))
	{
		pClient->OnHandleDisConnect();
	}
	pClient->SetLastRecvTime(NFGetTime());
}

void conn_writecb(struct bufferevent* pEv, void *pArg)
{
	// Intentionally unimplemented...
}

void log_cb(int severity, const char* msg)
{
	// Intentionally unimplemented...
}

void conn_eventcb(struct bufferevent* pEv, short what, void *pArg)
{
	NFClient* p = static_cast<NFClient*>(pArg);
	if (p == nullptr) return;

	if (what & BEV_EVENT_CONNECTED) {
		p->OnHandleConnect(static_cast<SOCKET>(bufferevent_getfd(pEv)));
	}
	if (what & BEV_EVENT_EOF)
	{
		p->OnHandleDisConnect();
		return;
	}

	if (what & BEV_EVENT_ERROR) {
#ifdef _WIN32
		if (ArkGetLastError() == WSAEISCONN)
		{
			p->OnHandleConnect(static_cast<SOCKET>(bufferevent_getfd(pEv)));;
			return;
		}
#endif
		p->OnHandleDisConnect();
		LogWarning(0, "NetWarning", " CloseProc Error Code " + std::string(evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR())));
	}
}

void NFClient::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
{
	switch (type)
	{
	case eMsgType_RECIVEDATA:
	{
		if (mRecvCB)
		{
			mRecvCB(usLink, nValue, nMsgId, pBuf, sz);
		}
	}
	case eMsgType_CONNECTED:
	case eMsgType_DISCONNECTED:
	{
		if (mEventCB)
		{
			mEventCB(type, usLink);
		}
	}
	break;
	default:
		break;
	}
}


void NFClient::OnHandleConnect(SOCKET nSocket)
{
	SetSocketId(nSocket);
	SetStatus(eConnectStatus_ConnectOk);
	OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NFClient::OnHandleDisConnect()
{
	SetStatus(eConnectStatus_Disconnect);
	OnHandleMsgPeer(eMsgType_DISCONNECTED, m_usLinkId, nullptr, 0, 0, 0);
	Close();
}

bool NFClient::Init()
{
	m_pMainBase = event_base_new();
	if (m_pMainBase == nullptr)
	{
		LogError(0, "NetError", "error: client event_base_new failed!");
		return false;
	}

	if (!Connect())
	{
		LogError(0, "NetError", "error: connected failed!");
		return false;
	}
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
	if (m_pMainBase)
	{
		event_base_loop(m_pMainBase, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}
	return true;
}

const string& NFClient::GetName() const
{
	return m_strName;
}

const NFClientFlag& NFClient::GetFlag() const
{
	return m_flag;
}

void NFClient::Close()
{
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
		m_pBev = nullptr;
	}
	m_nSocketId = INVALID_SOCKET;
	m_buffer.Clear();
}

bool NFClient::Connect()
{
	if (m_pBev) {
		bufferevent_free(m_pBev);
		m_pBev = nullptr;
	}
	m_pBev = bufferevent_socket_new(m_pMainBase, -1, BEV_OPT_CLOSE_ON_FREE);
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
	event_set_log_callback(&log_cb);

	if (bufferevent_socket_connect(m_pBev, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) < 0) {
		LogError(0, "NetError", "connect failed! IP: " + m_flag.strIP + " port:" + lexical_cast<std::string>(m_flag.nPort));
		return false;
	}

	return true;
}

bool NFClient::Reconnect()
{
	if (eConnectStatus_Disconnect == m_eStatus) 
	{
		Connect();
		return true;
	}
	return false;
}

event_base* NFClient::GetMainBase() const
{
	return m_pMainBase;
}

void NFClient::CheckConnect()
{
	if (m_flag.bAutoConnect) {
		NFClient::Reconnect();
	}
}