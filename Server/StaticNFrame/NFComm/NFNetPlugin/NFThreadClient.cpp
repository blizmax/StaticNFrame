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

#include "event2/thread.h"

/**
* @brief libevent读数据回调
*
* @param pEv   libevent读写数据类
* @param pArg  传入的参数
* @return
*/
static void conn_recvcb(struct bufferevent* pEv, void* pArg)
{
	NFThreadClient* pClient = static_cast<NFThreadClient*>(pArg);
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
	NFThreadClient* p = static_cast<NFThreadClient*>(pArg);
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

NFThreadClient::NFThreadClient(uint32_t nId, const NFClientFlag& flag) : NFClient(nId, flag)
{
	m_pMainBase = nullptr;
#ifdef WIN32
	WSADATA wsaData;
	int nResult = WSAStartup(0x0201, &wsaData);
	if (nResult)
	{
		std::cout << "WSAStartup failed with error code:" << nResult << std::endl;
		return;
	}
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif
}

NFThreadClient::~NFThreadClient()
{
	/**
	 *@brief  必须先析构m_pBev， 然后析构m_pMainBase
	 */
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
	}
	m_pBev = nullptr;
	if (m_pMainBase)
	{
		event_base_free(m_pMainBase);
	}
	m_pMainBase = nullptr;
}

bool NFThreadClient::Init()
{
	m_pMainBase = event_base_new();
	if (NULL == m_pMainBase)
	{
		LogError(0, "NetError", "error: client event_base_new failed!");
		return false;
	}

	StartThread();
	return true;
}

bool NFThreadClient::Shut()
{
	/**
	 * @brief	为了防止多线程冲突，这个顺序不要改变
	 */
	event_base_loopbreak(m_pMainBase);
	m_thread.StopThread();
	m_buffer.Clear();
	SetStatus(eConnectStatus_UnConnect);

	std::vector<NFThreadNetMsg*> vecMsg;
	m_threadNetMsgs.Pop(vecMsg);
	for (size_t i = 0; i < vecMsg.size(); i++)
	{
		NFSafeDelete(vecMsg[i]);
	}
	vecMsg.clear();
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
	}
	return true;
}

bool NFThreadClient::Execute()
{
	ProcessMsgLogicThread();
	return true;
}

void NFThreadClient::ProcessMsgLogicThread()
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

void NFThreadClient::CheckConnect()
{
	if (m_flag.bAutoConnect)
	{
		NFThreadClient::Reconnect();
	}
}

void NFThreadClient::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
{
	NFThreadNetMsg* pNetMsg = new NFThreadNetMsg();
	pNetMsg->usLink = usLink;
	pNetMsg->eType = type;
	pNetMsg->nMsgId = nMsgId;
	pNetMsg->nValue = nValue;
	pNetMsg->nBuffer.Init(pBuf, sz);

	m_threadNetMsgs.Push(pNetMsg);
}

void NFThreadClient::SendConnected()
{
	NFThreadNetMsg* pNetMsg = NF_NEW NFThreadNetMsg();
	pNetMsg->usLink = m_usLinkId;
	pNetMsg->eType = eMsgType_CONNECTED;

	m_threadNetMsgs.Push(pNetMsg);
}

void NFThreadClient::SendDisconnect()
{
	NFThreadNetMsg* pNetMsg = NF_NEW NFThreadNetMsg();
	pNetMsg->usLink = m_usLinkId;
	pNetMsg->eType = eMsgType_DISCONNECTED;

	m_threadNetMsgs.Push(pNetMsg);
}

void NFThreadClient::Close()
{
	SetStatus(eConnectStatus_Disconnect);
	m_buffer.Clear();
	event_base_loopbreak(m_pMainBase);
}

bool NFThreadClient::Connect()
{
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
		m_pBev = nullptr;
	}
	m_pBev = bufferevent_socket_new(GetMainBase(), -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
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

void NFThreadClient::OnClientThread()
{
	Connect();
	event_base_dispatch(GetMainBase());
}

void NFThreadClient::StartThread()
{
	m_thread.StartThread(this, &NFThreadClient::OnClientThread);
}

bool NFThreadClient::Reconnect()
{
	if (eConnectStatus_Disconnect == m_eStatus)
	{
		time_t nCurTime = time(nullptr);
		if (nCurTime - m_lastDisconnetTime > 5)
		{ //5s重启
			SetStatus(eConnectStatus_Connecting);
			StartThread();
		}
		return true;
	}
	return false;
}

void NFThreadClient::OnHandleConnect(SOCKET nSocket)
{
	SetSocketId(nSocket);
	SendConnected();
	SetStatus(eConnectStatus_ConnectOk);
}

void NFThreadClient::OnHandleDisConnect()
{
	SendDisconnect();
	Close();
}

