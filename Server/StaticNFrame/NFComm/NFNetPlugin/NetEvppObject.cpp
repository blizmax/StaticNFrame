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
	mIsServer = true;
	mPacketParseType = 0;
}

NetEvppObject::~NetEvppObject()
{
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

uint32_t NetEvppObject::GetLinkId() const
{
	return m_usLinkId;
}

void NetEvppObject::SetLinkId(uint32_t linkId)
{
	m_usLinkId = linkId;
}

void NetEvppObject::OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue, uint32_t operateId)
{
	switch (type)
	{
	case eMsgType_RECIVEDATA:
	{
		if (mRecvCB)
		{
			mRecvCB(usLink, nValue, operateId, nMsgId, pBuf, sz);
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
	OnHandleMsgPeer(eMsgType_CONNECTED, m_usLinkId, nullptr, 0, 0, 0, 0);
}

void NetEvppObject::OnHandleDisConnect()
{
	SetNeedRemove(true);
	OnHandleMsgPeer(eMsgType_DISCONNECTED, m_usLinkId, nullptr, 0, 0, 0, 0);
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

bool NetEvppObject::Send(const uint8_t rpcType, const uint64_t rpcReqId, const char* msg, const uint32_t nLen)
{
	if (!GetNeedRemove() && mConnPtr && mConnPtr->IsConnected())
	{
		//mConnPtr->loop()->RunInLoop(std::bind(&SendToThread, mConnPtr, mPacketParseType, nMsgID, evpp::Slice(msg, nLen).ToString(), nPlayerID));
		NFBuffer mxSendBuffer;
		NFIPacketParse::EnCode(mPacketParseType, 0, 0, 0, rpcType, rpcReqId, msg, nLen, mxSendBuffer);
		mConnPtr->Send((const void*)mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
		return true;
	}
	return false;
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

void SendToThread(evpp::TCPConnPtr connPtr, uint32_t nPacketParseType, const uint32_t nMsgID, const std::string& message, const uint64_t nPlayerID, const uint32_t operateId)
{
	NFBuffer mxSendBuffer;
	NFIPacketParse::EnCode(nPacketParseType, nMsgID, nPlayerID, operateId, 0, 0, message.data(), message.length(), mxSendBuffer);
	connPtr->Send((const void*)mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
}

bool NetEvppObject::Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	if (!GetNeedRemove() && mConnPtr && mConnPtr->IsConnected())
	{
		//mConnPtr->loop()->RunInLoop(std::bind(&SendToThread, mConnPtr, mPacketParseType, nMsgID, evpp::Slice(msg, nLen).ToString(), nPlayerID));
		NFBuffer mxSendBuffer;
		NFIPacketParse::EnCode(mPacketParseType, nMsgID, nPlayerID, operateId, 0, 0, msg, nLen, mxSendBuffer);
		mConnPtr->Send((const void*)mxSendBuffer.ReadAddr(), mxSendBuffer.ReadableSize());
		return true;
	}
	return false;
}


