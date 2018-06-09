// -------------------------------------------------------------------------
//    @FileName         :    NFMyClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/28
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFMyClient.h"
#include "NFEventData.h"
#include <NFComm/NFPluginModule/NFLogMgr.h>
#include "NFIPacketParse.h"

void event_cb(short events, void* pArg)
{
	NFMyClient* pClient = static_cast<NFMyClient*>(pArg);
	if (pClient == nullptr) return;

	if (events & EVENT_CONNECT)
	{
		pClient->OnHandleConnect();
	}
	else if (events & EVENT_DISCONNECT)
	{
		pClient->OnHandleDisConnect();
	}
	else if (events & EVENT_ERROR)
	{
		pClient->OnHandleDisConnect();
	}
}

void recv_cb(void* pArg)
{
	NFMyClient* pClient = static_cast<NFMyClient*>(pArg);
	if (pClient == nullptr) return;

	if (!pClient->OnRecvData())
	{
		pClient->OnHandleDisConnect();
	}
}

NFMyClient::NFMyClient(uint32_t nId, const NFClientFlag& flag) : m_nSocketId(INVALID_SOCKET), m_usLinkId(nId), m_pEventData(nullptr)
{
	m_pEventData = new EventData();
	m_pEventData->Init(100);
	m_flag = flag;
	m_buffer.AssureSpace(MAX_RECV_BUFFER_SIZE);
}

NFMyClient::~NFMyClient()
{
	delete m_pEventData;
}

bool NFMyClient::Init()
{
	return Connect(m_flag.strIP, m_flag.nPort);
}

bool NFMyClient::Shut()
{
	m_pEventData->Close();
	return true;
}

bool NFMyClient::Execute()
{
	Dispatch(0);
	return true;
}

bool NFMyClient::Connect(const std::string& ip, int port)
{
	bool retCode = false;

	m_pEventData->set_cb(recv_cb, event_cb, this);

	retCode = m_pEventData->Connect(ip.c_str(), port);
	if (!retCode)
	{
		return false;
	}

	m_nSocketId = m_pEventData->mSock;
	return true;
}

bool NFMyClient::Send(const char* buf, uint32_t len)
{
	if (len > 0 && m_pEventData)
	{
		bool retCode = m_pEventData->Send(buf, len);
		if (retCode == false)
		{
			NFLogError("[Net] send error remote[%s] [%d]", m_flag.strIP.c_str(), m_flag.nPort);
			return false;
		}
	}
	return true;
}

bool NFMyClient::Dispatch(uint32_t timeout)
{
	if (m_pEventData)
	{
		m_pEventData->Dispatch(timeout);
	}
	return true;
}

void NFMyClient::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
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

void NFMyClient::OnHandleConnect()
{
	OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NFMyClient::OnHandleDisConnect()
{
	OnHandleMsgPeer(eMsgType_DISCONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

bool NFMyClient::OnRecvData()
{
	int ret = m_pEventData->Recv(m_buffer.WriteAddr(), m_buffer.WritableSize());
	if (ret < 0)
	{
		return false;
	}
	else if (ret == 0)
	{
		return true;
	}

	m_buffer.Produce(ret);

	while (true)
	{
		ret = Dismantle();
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

int NFMyClient::Dismantle()
{
	char* outData = nullptr;
	uint32_t outLen = 0;
	uint32_t allLen = 0;
	uint32_t nMsgId = 0;
	uint64_t nValue = 0;
	int ret = NFIPacketParse::DeCode(m_buffer.ReadAddr(), m_buffer.ReadableSize(), outData, outLen, allLen, nMsgId, nValue);
	if (ret < 0)
	{
		NFSocket::Close(m_nSocketId);
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

uint32_t NFMyClient::GetLinkId() const
{
	return m_usLinkId;
}

