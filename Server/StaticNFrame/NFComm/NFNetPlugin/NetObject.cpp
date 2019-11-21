// -------------------------------------------------------------------------
//    @FileName         :    NetObject.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//
//                    .::::.
//                  .::::::::.
//                 :::::::::::  FUCK YOU
//             ..:::::::::::'
//           '::::::::::::'
//             .::::::::::
//        '::::::::::::::..
//             ..::::::::::::.
//           ``::::::::::::::::
//            ::::``:::::::::'        .:::.
//           ::::'   ':::::'       .::::::::.
//         .::::'      ::::     .:::::::'::::.
//        .:::'       :::::  .:::::::::' ':::::.
//       .::'        :::::.:::::::::'      ':::::.
//      .::'         ::::::::::::::'         ``::::.
//  ...:::           ::::::::::::'              ``::.
// ```` ':.          ':::::::::'                  ::::..
//                    '.:::::'                    ':'````..
//
// -------------------------------------------------------------------------
#include "NetObject.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NFComm/NFCore/NFSHA2.h"
#include "NFComm/NFCore/NFBase64.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFHttpFormat.h"
#include "NFComm/NFCore/NFStringUtility.h"

/**
* @brief libevent读数据回调
*
* @param pEv   libevent读写数据类
* @param pArg  传入的参数
* @return
*/

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

void NetObject::conn_recvcb(struct bufferevent* pEv, void* pArg)
{
	NetObject* pObject = static_cast<NetObject*>(pArg);
	if (pObject == nullptr) return;

	if (pObject->GetNeedRemove()) return;

	if (!pObject->OnRecvData(pEv))
	{
		pObject->OnHandleDisConnect();
	}
}

/**
* @brief libevent连接事件回调
*
* @param pEv		libevent读写数据类
* @param events	事件
* @param pArg		传入的参数
* @return
*/
void NetObject::conn_eventcb(struct bufferevent* pEv, short events, void* pArg)
{
	NetObject* p = static_cast<NetObject*>(pArg);
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
		//NFLogError("NetError | CloseProc Error Code ",std::string(evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR())));
	}
}

/**
* @brief libevent写数据回调
*
* @param pEv   libevent读写数据类
* @param pArg  传入的参数
* @return
*/
void NetObject::conn_writecb(struct bufferevent* pEv, void* pArg)
{
	// Intentionally unimplemented...
}

NetObject::NetObject() : m_pBev(nullptr), m_usLinkId(0), m_port(0), mNeedRemove(false)
{
	mWSFrameType = 0;
	m_nSocketId = INVALID_SOCKET;
	m_buffer.AssureSpace(MAX_RECV_BUFFER_SIZE);
	mWebSocket = false;
	mIsServer = true;
	mPacketParseType = 0;
}

NetObject::~NetObject()
{
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
	}
	m_nSocketId = INVALID_SOCKET;
	m_buffer.Clear();
	m_pBev = nullptr;
}

void NetObject::SetWebSocket(bool b)
{
	mWebSocket = b;
}

bool NetObject::IsWebSocket() const
{
	return mWebSocket;
}

std::string NetObject::GetStrIp() const
{
	return m_strIp;
}

void NetObject::SetStrIp(std::string val)
{
	m_strIp = val;
}

uint32_t NetObject::GetPort() const
{
	return m_port;
}

void NetObject::SetPort(uint32_t val)
{
	m_port = val;
}

void NetObject::SetIsServer(bool b)
{
	mIsServer = b;
}

int NetObject::Dismantle()
{
	char* outData = nullptr;
	uint32_t outLen = 0;
	uint32_t allLen = 0;
	uint32_t nMsgId = 0;
	uint64_t nValue = 0;
	int ret = NFIPacketParse::DeCode(mPacketParseType, m_buffer.ReadAddr(), m_buffer.ReadableSize(), outData, outLen, allLen, nMsgId, nValue);
	if (ret < 0)
	{
		m_buffer.Consume(m_buffer.ReadableSize());
		return -1;
	}
	else if (ret > 0)
	{
		return 1;
	}
	else
	{
		OnHandleMsgPeer(eMsgType_RECIVEDATA, m_usLinkId, outData, outLen, nMsgId, nValue);
		m_buffer.Consume(allLen);
		return 0;
	}
}

bufferevent* NetObject::GetBev() const
{
	return m_pBev;
}

void NetObject::SetBev(bufferevent* bev)
{
	m_pBev = bev;
}

uint32_t NetObject::GetLinkId() const
{
	return m_usLinkId;
}

void NetObject::SetLinkId(uint32_t linkId)
{
	m_usLinkId = linkId;
}

void NetObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, const std::string& strMsg)
{
	switch (type)
	{
	case eMsgType_RECIVEDATA:
	{
		if (mRecvCB)
		{
			mRecvCB(usLink, 0, 0, strMsg.data(), strMsg.length());
		}
	}
	break;
	case eMsgType_CONNECTED:
	{
		if (mEventCB)
		{
			mEventCB(type, usLink);
		}
	}
	break;
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

void NetObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
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
		break;
	case eMsgType_CONNECTED:
	{
		if (mEventCB)
		{
			mEventCB(type, usLink);
		}
	}
	break;
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

bool NetObject::OnRecvData(bufferevent* pBufEv)
{
	if (NULL == pBufEv)
	{
		return false;
	}
	struct evbuffer* pEvbuff = bufferevent_get_input(pBufEv);
	if (NULL == pEvbuff)
	{
		return false;
	}

	int32_t ilen = static_cast<int32_t>(evbuffer_get_length(pEvbuff));
	if (ilen <= 0)
	{
		return true;
	}

	m_buffer.AssureSpace(ilen);

	if (evbuffer_remove(pEvbuff, m_buffer.WriteAddr(), ilen) > 0)
	{
		m_buffer.Produce(ilen);
	}
	else
	{
		return false;
	}

	while (true)
	{
		int ret = Dismantle();
		if (ret < 0)
		{
			return false;
		}
		else if (ret > 0)
		{
			break;
		}
		if (GetNeedRemove())
		{
			return false;
		}
	}

	return true;
}

bool NetObject::GetNeedRemove() const
{
	return mNeedRemove;
}

void NetObject::SetNeedRemove(bool val)
{
	mNeedRemove = val;
}

void NetObject::OnHandleConnect(SOCKET nSocket)
{
	SetSocketId(nSocket);
	OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NetObject::OnHandleDisConnect()
{
	SetNeedRemove(true);
	CloseObject();
	OnHandleMsgPeer(eMsgType_DISCONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NetObject::CloseObject()
{
	if (m_nSocketId != INVALID_SOCKET)
	{
		bufferevent_disable(m_pBev, EV_READ | EV_WRITE);
		evutil_closesocket(m_nSocketId);
		m_nSocketId = INVALID_SOCKET;
	}
}

void NetObject::SetSocketId(SOCKET nSocket)
{
	m_nSocketId = nSocket;
	int tcp_nodelay = 1;
	int ret = 0;
	ret = setsockopt(nSocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char *>(&tcp_nodelay), sizeof(tcp_nodelay));
	if (ret < 0)
	{
		std::cout << "setsockopt TCP_NODELAY failed" << std::endl;
	}
	int bufflen = 65536;
	ret = setsockopt(nSocket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&bufflen), sizeof(bufflen));
	if (ret < 0)
	{
		std::cout << "setsockopt SO_RCVBUF failed" << std::endl;
	}
	ret = setsockopt(nSocket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&bufflen), sizeof(bufflen));
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

bool NetObject::Send(const void* pData, uint32_t unSize)
{
	if (!GetNeedRemove() && bufferevent_get_enabled(m_pBev) != 0)
	{
		int nRet = bufferevent_write(m_pBev, pData, unSize);
		if (nRet < 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "NetError | send msg error !");
			return false;
		}
		return true;
	}
	return false;
}

/**
 * @brief	发送数据 不包含数据头
 *
 * @param pData		发送的数据,
 * @param unSize	数据的大小
 * @return
 */
bool NetObject::Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	if (!GetNeedRemove() && bufferevent_get_enabled(m_pBev) != 0)
	{
		m_sendBuffer.Clear();
		NFIPacketParse::EnCode(mPacketParseType, nMsgID, nPlayerID, msg, nLen, m_sendBuffer);
		int nRet = bufferevent_write(m_pBev, (const void*)m_sendBuffer.ReadAddr(), m_sendBuffer.ReadableSize());
		if (nRet < 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "NetError | send msg error !");
			return false;
		}
		return true;
	}
	return false;
}

