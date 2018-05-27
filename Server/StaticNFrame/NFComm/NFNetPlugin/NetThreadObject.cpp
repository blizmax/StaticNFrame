// -------------------------------------------------------------------------
//    @FileName         :    NetObject.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NetThreadObject.h"

NetThreadObject::NetThreadObject() : m_pBev(nullptr), m_usLinkId(0), m_eStatus(), m_lastRecvTime(0), m_lastDisconnetTime(0), m_lastPingTime(0)
{
	m_eStatus = eConnectStatus_UnConnect;
	m_nSocketId = INVALID_SOCKET;
	m_buffer.AssureSpace(MAX_RECV_BUFFER_SIZE);
}

NetThreadObject::~NetThreadObject()
{
	if (m_pBev)
	{
		bufferevent_free(m_pBev);
	}
	m_eStatus = eConnectStatus_UnConnect;
	m_nSocketId = INVALID_SOCKET;
	m_buffer.Clear();
	m_pBev = nullptr;
}

int NetThreadObject::Dismantle()
{
	char* outData = nullptr;
	uint32_t outLen = 0;
	uint32_t allLen = 0;
	uint32_t nMsgId = 0;
	uint64_t nValue = 0;
	int ret = NFIPacketParse::DeCode(m_buffer.ReadAddr(), m_buffer.ReadableSize(), outData, outLen, allLen, nMsgId, nValue);
	if (ret < 0)
	{
		bufferevent_disable(m_pBev, EV_READ | EV_WRITE);
		evutil_closesocket(m_nSocketId);
		m_nSocketId = INVALID_SOCKET;
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

bufferevent* NetThreadObject::GetBev() const
{
	return m_pBev;
}

uint32_t NetThreadObject::GetLinkId() const
{
	return m_usLinkId;
}

bool NetThreadObject::OnRecvData(bufferevent* pBufEv)
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
	}

	return true;
}

eConnectStatus NetThreadObject::GetStatus()
{
	return m_eStatus;
}

void NetThreadObject::SetStatus(eConnectStatus val)
{
	m_eStatus.store(val);
}

bool NetThreadObject::IsConnectOK() const
{
	return m_eStatus.load() == eConnectStatus_ConnectOk;
}

uint64_t NetThreadObject::GetLastRecvTime() const
{
	return m_lastRecvTime;
}

void NetThreadObject::SetLastRecvTime(uint64_t val)
{
	m_lastRecvTime = val;
}

uint64_t NetThreadObject::GetLastPingTime() const
{
	return m_lastPingTime;
}

void NetThreadObject::SetLastPingTime(uint64_t val)
{
	m_lastPingTime = val;
}

void NetThreadObject::SetSocketId(SOCKET nSocket)
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

bool NetThreadObject::Send(const void* pData, uint32_t unSize)
{
	if (IsConnectOK() && bufferevent_get_enabled(m_pBev) != 0)
	{
		int nRet = bufferevent_write(m_pBev, pData, unSize);
		if (nRet < 0)
		{
			NFLogNormalError(0, "NetError", "send msg error !");
			return false;
		}
		return true;
	}
	return false;
}

