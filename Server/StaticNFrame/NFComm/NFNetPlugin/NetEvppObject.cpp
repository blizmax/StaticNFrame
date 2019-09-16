// -------------------------------------------------------------------------
//    @FileName         :    NetEvppObject.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/06/3
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                    _ooOoo_
//                   o8888888o
//                   88" . "88
//                   (| -_- |)
//                    O\ = /O
//                ____/`---'\____
//              .   ' \\| |// `.
//               / \\||| : |||// \
//             / _||||| -:- |||||- \
//               | | \\\ - /// | |
//            | \_| ''\---/'' | |
//              \ .-\__ `-` ___/-. /
//           ___`. .' /--.--\ `. . __
//        ."" '< `.___\_<|>_/___.' >'"".
//       | | : `- \`.;`\ _ /`;.`/ - ` : | |
//         \ \ `-. \_ __\ /__ _/ .-` / /
// ======`-.____`-.___\_____/___.-`____.-'======
//                    `=---='
//
// .............................................
//          ·ð×æ±£ÓÓ             ÓÀÎÞBUG
//
// -------------------------------------------------------------------------

#include "NetEvppObject.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NFComm/NFCore/NFSHA2.h"
#include "NFComm/NFCore/NFBase64.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFHttpFormat.h"
#include "NFComm/NFCore/NFStringUtility.h"


#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


NetEvppObject::NetEvppObject(const evpp::TCPConnPtr& conn) : m_usLinkId(0), m_port(0), mNeedRemove(false), mConnPtr(conn)
{
	m_buffer.AssureSpace(MAX_RECV_BUFFER_SIZE);
	mIsServer = true;
	mPacketParseType = 0;
}

NetEvppObject::~NetEvppObject()
{
	m_buffer.Clear();
}

std::string NetEvppObject::GetStrIp() const
{
	return m_strIp;
}

void NetEvppObject::SetStrIp(std::string val)
{
	m_strIp = val;
}

uint32_t NetEvppObject::GetPort() const
{
	return m_port;
}

void NetEvppObject::SetPort(uint32_t val)
{
	m_port = val;
}

void NetEvppObject::SetIsServer(bool b)
{
	mIsServer = b;
}

bool NetEvppObject::OnRecvData(const char* data, size_t length)
{
	m_buffer.PushData(data, length);

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

int NetEvppObject::Dismantle()
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

uint32_t NetEvppObject::GetLinkId() const
{
	return m_usLinkId;
}

void NetEvppObject::SetLinkId(uint32_t linkId)
{
	m_usLinkId = linkId;
}

void NetEvppObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, const std::string& strMsg)
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

void NetEvppObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue)
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

bool NetEvppObject::GetNeedRemove() const
{
	return mNeedRemove;
}

void NetEvppObject::SetNeedRemove(bool val)
{
	mNeedRemove = val;
}

void NetEvppObject::OnHandleConnect()
{
	OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NetEvppObject::OnHandleDisConnect()
{
	SetNeedRemove(true);
	//CloseObject();
	OnHandleMsgPeer(eMsgType_DISCONNECTED, m_usLinkId, nullptr, 0, 0, 0);
}

void NetEvppObject::CloseObject()
{
	if (mConnPtr)
	{
		if (mConnPtr->IsConnected() || mConnPtr->IsConnecting())
		{
			mConnPtr->Close();
		}
	}
}

bool NetEvppObject::Send(const void* pData, uint32_t unSize)
{
	if (!GetNeedRemove() && mConnPtr && mConnPtr->IsConnected())
	{

		mConnPtr->Send(pData, unSize);
		return true;
	}
	return false;
}

void SendToThread(evpp::TCPConnPtr connPtr, uint32_t nPacketParseType, const uint32_t nMsgID, const std::string& message, const uint64_t nPlayerID)
{
	NFBuffer mxSendBuffer;
	NFIPacketParse::EnCode(nPacketParseType, nMsgID, nPlayerID, message.data(), message.length(), mxSendBuffer);
	connPtr->Send((const void*)mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
}

bool NetEvppObject::Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID)
{
	if (!GetNeedRemove() && mConnPtr && mConnPtr->IsConnected())
	{
		//mConnPtr->loop()->RunInLoop(std::bind(&SendToThread, mConnPtr, mPacketParseType, nMsgID, evpp::Slice(msg, nLen).ToString(), nPlayerID));
		NFBuffer mxSendBuffer;
		NFIPacketParse::EnCode(mPacketParseType, nMsgID, nPlayerID, msg, nLen, mxSendBuffer);
		mConnPtr->Send((const void*)mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
		return true;
	}
	return false;
}


