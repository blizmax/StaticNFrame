// -------------------------------------------------------------------------
//    @FileName         :    NFThreadClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "NFThreadClient.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFLibEvent.h"
#include "NFLoopThreadClient.h"

/**
* @brief libevent读数据回调
*
* @param pEv   libevent读写数据类
* @param pArg  传入的参数
* @return
*/
static void conn_recvcb(struct bufferevent* pEv, void* pArg)
{
	NFLoopThreadClient* pClient = static_cast<NFLoopThreadClient*>(pArg);
	if (pClient == nullptr) return;

	if (!pClient->OnRecvData(pEv))
	{
		pClient->OnHandleDisConnect();
	}
	pClient->SetLastRecvTime(NFGetTime());
}

/**
* @brief libevent连接事件回调
*
* @param pEv		libevent读写数据类
* @param events	事件
* @param pArg		传入的参数
* @return
*/
static void conn_eventcb(struct bufferevent* pEv, short events, void* pArg)
{
	NFLoopThreadClient* p = static_cast<NFLoopThreadClient*>(pArg);
	if (p == nullptr) return;

	if (events & BEV_EVENT_CONNECTED)
	{
		p->OnHandleConnect(static_cast<SOCKET>(bufferevent_getfd(pEv)));
	}
	if (events & BEV_EVENT_EOF)
	{
		p->OnHandleDisConnect();
		return;
	}

	if (events & BEV_EVENT_ERROR)
	{
#ifdef _WIN32
		if (ArkGetLastError() == WSAEISCONN)
		{
			p->OnHandleConnect(static_cast<SOCKET>(bufferevent_getfd(pEv)));;
			return;
		}
#endif
		p->OnHandleDisConnect();
		//LogWarning(0, "NetWarning", " CloseProc Error Code " + std::string(evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR())));
	}
}

/**
* @brief libevent写数据回调
*
* @param pEv   libevent读写数据类
* @param pArg  传入的参数
* @return
*/
static void conn_writecb(struct bufferevent* pEv, void* pArg)
{
	// Intentionally unimplemented...
}

/**
* @brief log回调
*
* @param severity
* @param msg		要打印的消息
* @return
*/
static void log_cb(int severity, const char* msg)
{
	// Intentionally unimplemented...
}

NFLoopThreadClient::NFLoopThreadClient(uint32_t nId, const NFClientFlag& flag) : m_flag(flag)
{
	m_usLinkId = nId;
#ifdef WIN32
	WSADATA wsaData;
	int nResult = WSAStartup(0x0201, &wsaData);
	if (nResult)
	{
		std::cout << "WSAStartup failed with error code:" << nResult << std::endl;
		return;
	}
#endif
}

NFLoopThreadClient::~NFLoopThreadClient()
{
}

bool NFLoopThreadClient::Init()
{
	StartThread();
	return true;
}

bool NFLoopThreadClient::Shut()
{
	if (m_loopThread->IsRunning())
	{
		m_loopThread->Stop(true);
	}
	std::vector<NFThreadNetMsg*> vecMsg;
	m_threadNetMsgs.Pop(vecMsg);
	for (size_t i = 0; i < vecMsg.size(); i++)
	{
		NFSafeDelete(vecMsg[i]);
	}
	vecMsg.clear();
	return true;
}

bool NFLoopThreadClient::Execute()
{
	ProcessMsgLogicThread();
	return true;
}

void NFLoopThreadClient::ProcessMsgLogicThread()
{
	std::vector<NFThreadNetMsg*> vecMsg;
	m_threadNetMsgs.Pop(vecMsg);

	for (int i = 0; i < static_cast<int>(vecMsg.size()); i++)
	{
		NFThreadNetMsg* pBuff = vecMsg[i];
		if (pBuff == nullptr) continue;

		switch (pBuff->eType)
		{
		case eMsgType_RECIVEDATA:
			{
				if (mRecvCB)
				{
					mRecvCB(pBuff->usLink, pBuff->nValue, pBuff->nMsgId, static_cast<const char*>(pBuff->nBuffer.data()), pBuff->nBuffer.size());
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

void NFLoopThreadClient::CheckConnect()
{
	if (m_flag.bAutoConnect)
	{
		NFLoopThreadClient::Reconnect();
	}
}

void NFLoopThreadClient::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
{
	NFThreadNetMsg* pNetMsg = new NFThreadNetMsg();
	pNetMsg->usLink = usLink;
	pNetMsg->eType = type;
	pNetMsg->nMsgId = nMsgId;
	pNetMsg->nValue = nValue;
	pNetMsg->nBuffer.Init(pBuf, sz);

	m_threadNetMsgs.Push(pNetMsg);
}

void NFLoopThreadClient::SendConnected()
{
	NFThreadNetMsg* pNetMsg = NF_NEW NFThreadNetMsg();
	pNetMsg->usLink = m_usLinkId;
	pNetMsg->eType = eMsgType_CONNECTED;

	m_threadNetMsgs.Push(pNetMsg);
}

void NFLoopThreadClient::SendDisconnect()
{
	NFThreadNetMsg* pNetMsg = NF_NEW NFThreadNetMsg();
	pNetMsg->usLink = m_usLinkId;
	pNetMsg->eType = eMsgType_DISCONNECTED;

	m_threadNetMsgs.Push(pNetMsg);
}

int NFLoopThreadClient::Close()
{
	SetStatus(eConnectStatus_Disconnect);
	m_buffer.Clear();
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
		m_pBev = nullptr;
	}
	return 0;
}

bool NFLoopThreadClient::Connect()
{
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
		m_pBev = nullptr;
	}
	m_pBev = bufferevent_socket_new(m_loopThread->event_base(), -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET ;
	inet_pton(AF_INET, m_flag.strIP.c_str(), (void*)&sin.sin_addr.s_addr);
	sin.sin_port = htons(m_flag.nPort);
	bufferevent_setcb(m_pBev, &conn_recvcb, &conn_writecb, &conn_eventcb, this);

	//设置水位为0
	bufferevent_setwatermark(m_pBev, EV_READ, 0, 0);
	bufferevent_setwatermark(m_pBev, EV_WRITE, 0, 0);

	bufferevent_enable(m_pBev, EV_WRITE | EV_READ);
	event_set_log_callback(&log_cb);

	if (bufferevent_socket_connect(m_pBev, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) < 0)
	{
		LogError(0, "NetError", "connect failed! IP: " + m_flag.strIP + " port:" + lexical_cast<std::string>(m_flag.nPort));
		return false;
	}

	return true;
}

int NFLoopThreadClient::OnClientThread()
{
	if (Connect())
	{
		return 0;
	}
	return 1;
}

void NFLoopThreadClient::StartThread()
{
	m_loopThread->Start(true, std::bind(&NFLoopThreadClient::OnClientThread, this), std::bind(&NFLoopThreadClient::Close, this));
}

bool NFLoopThreadClient::Reconnect()
{
	if (m_loopThread->IsStopped())
	{
		return true;
	}
	return false;
}

void NFLoopThreadClient::OnHandleConnect(SOCKET nSocket)
{
	SetSocketId(nSocket);
	SendConnected();
	SetStatus(eConnectStatus_ConnectOk);
}

void NFLoopThreadClient::OnHandleDisConnect()
{
	SendDisconnect();
	m_loopThread->Loop()->Stop();
	SetStatus(eConnectStatus_Disconnect);
}

