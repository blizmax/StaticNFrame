// -------------------------------------------------------------------------
//    @FileName         :    NFEvppServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/06/01
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
//          佛祖保佑             永无BUG
//
// -------------------------------------------------------------------------

#include "NFEvppServer.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFINetModule.h"


NFEvppServer::NFEvppServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag) : NFIServer(serverType, serverId, flag)
{
	m_eventLoop = nullptr;
	m_tcpServer = nullptr;
	mExit = false;
}

NFEvppServer::~NFEvppServer()
{
	if (m_eventLoop)
	{
		NF_SAFE_DELETE(m_eventLoop);
	}

	if (m_tcpServer)
	{
		NF_SAFE_DELETE(m_tcpServer);
	}

	m_eventLoop = nullptr;
	m_tcpServer = nullptr;
}

void NFEvppServer::ProcessMsgLogicThread()
{
	std::vector<MsgFromNetInfo*> vecMsg;
	mMsgQueue.Pop(vecMsg);
	for(size_t i = 0; i < vecMsg.size(); i++)
	{
		MsgFromNetInfo* pMsg = vecMsg[i];
		if (pMsg == nullptr) continue;

		if (pMsg->nType == eMsgType_CONNECTED)
		{
			NetEvppObject* pObject = AddNetObject(pMsg->mTCPConPtr);
			if (pObject)
			{
				pMsg->mTCPConPtr->set_context(evpp::Any(pObject));
				pObject->OnHandleConnect();
			}
		}
		else if (pMsg->nType == eMsgType_DISCONNECTED)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject)
				{
					pObject->OnHandleDisConnect();
				}
				pMsg->mTCPConPtr->set_context(evpp::Any(nullptr));
			}
		}
		else if (pMsg->nType == eMsgType_RECIVEDATA)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject)
				{
					pObject->OnRecvData(pMsg->strMsg.data(), pMsg->strMsg.length());
				}
			}
		}
		else
		{
			//
		}

		NF_SAFE_DELETE(pMsg);
	}
}

bool NFEvppServer::Init()
{
	m_eventLoop = NF_NEW evpp::EventLoopThread();
	m_eventLoop->set_name(GetServerName(mServerType));
	m_eventLoop->Start(true);
	std::string listenAddr = NF_FORMAT("0.0.0.0:{}", mFlag.nPort);

	m_tcpServer = NF_NEW evpp::TCPServer(m_eventLoop->loop(), listenAddr, GetServerName(mServerType), mFlag.nWorkThreadNum);

	//链接回调是在别的线程里运行的
	m_tcpServer->SetConnectionCallback([this](const evpp::TCPConnPtr& conn) {
		if (conn->IsConnected())
		{
			conn->SetTCPNoDelay(true);
			MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
			pMsg->nType = eMsgType_CONNECTED;
			mMsgQueue.Push(pMsg);
		}
		else
		{
			MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
			pMsg->nType = eMsgType_DISCONNECTED;
			mMsgQueue.Push(pMsg);
		}
	});

	//消息回调是在别的线程里运行的
	m_tcpServer->SetMessageCallback([this](const evpp::TCPConnPtr& conn,
		evpp::Buffer* msg) {

		evpp::Slice xMsgBuff;
		if (msg)
		{
			xMsgBuff = msg->NextAll();
			MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
			pMsg->nType = eMsgType_RECIVEDATA;
			pMsg->strMsg = std::string(xMsgBuff.data(), xMsgBuff.size());
			mMsgQueue.Push(pMsg);
		}
	});

	if (m_tcpServer->Init())
	{
		return m_tcpServer->Start();
	}

	return false;
}

std::string NFEvppServer::GetLinkIp(uint32_t usLinkId)
{
	NetEvppObject* pObject = GetNetObject(usLinkId);
	if (pObject)
	{
		return pObject->GetStrIp();
	}

	return std::string("");
}

NetEvppObject* NFEvppServer::AddNetObject(const evpp::TCPConnPtr& conn)
{
	if (GetNetObjectCount() >= GetMaxConnectNum())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "connected count >= mnMaxConnect:%d! Can't add connect", GetMaxConnectNum());
		return nullptr;
	}

	uint32_t usLinkId = GetFreeUnLinkId();
	if (usLinkId == 0)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "connected count >= mnMaxConnect:%d! Can't add connect", GetMaxConnectNum());
		return nullptr;
	}

	uint32_t index = GetServerIndexFromUnlinkId(usLinkId);
	if (index >= mNetObjectArray.size() || mNetObjectArray[index] != nullptr)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "GetServerIndexFromUnLinkId Failed!");
		return nullptr;
	}

	NetEvppObject* pObject = NF_NEW NetEvppObject(conn);
	mNetObjectArray[index] = pObject;
	mNetObjectCount++;

	pObject->SetLinkId(usLinkId);
	pObject->SetStrIp(conn->remote_addr());
	pObject->SetPort(0);
	pObject->SetPacketParseType(mPacketParseType);
	

	pObject->SetRecvCB(this, &NFEvppServer::OnReceiveNetPack);
	pObject->SetEventCB(this, &NFEvppServer::OnSocketNetEvent);

	return pObject;
}

NetEvppObject* NFEvppServer::GetNetObject(uint32_t usLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(usLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(usLinkId);

	if (serverType != mServerType)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "serverType != mServerType, this usLinkId:%s is not of the server:%s", usLinkId, GetServerName(mServerType).c_str());
		return nullptr;
	}

	if (serverIndex < mNetObjectArray.size())
	{
		auto pObject = mNetObjectArray[serverIndex];
		if (pObject)
		{
			return pObject;
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "the usLinkId:{} is nullptr", usLinkId);
		}
	}
	return nullptr;
}

void NFEvppServer::CloseLinkId(uint32_t usLinkId)
{
	NetEvppObject* pObject = GetNetObject(usLinkId);
	if (pObject)
	{
		pObject->SetNeedRemove(true);
		pObject->CloseObject();
	}
}

uint32_t NFEvppServer::GetFreeUnLinkId()
{
	if (mNetObjectArray.empty())
	{
		mNetObjectArray.push_back(nullptr);
		return GetUnLinkId(mServerType, 0);
	}

	size_t sz = mNetObjectArray.size();

	for (size_t index = 0; index < sz; index++)
	{
		if (mNetObjectArray[index] == nullptr)
		{
			return GetUnLinkId(mServerType, index);
		}
	}

	if (sz >= mFlag.nMaxConnectNum)
	{
		return 0;
	}

	mNetObjectArray.push_back(nullptr);

	return GetUnLinkId(mServerType, sz);
}

bool NFEvppServer::Shut()
{
	mExit = false;
	m_tcpServer->Stop([this]() {
		this->mExit = true;
	});

	while (!mExit)
	{
		NFSLEEP(1);
	}

	m_eventLoop->Stop(true);

	return true;
}

bool NFEvppServer::Finalize()
{
	for (size_t i = 0; i < mNetObjectArray.size(); i++)
	{
		if (mNetObjectArray[i] != nullptr)
		{
			mNetObjectCount--;
			NF_SAFE_DELETE(mNetObjectArray[i]);
		}
	}
	mNetObjectArray.clear();

	if (m_eventLoop)
	{
		NF_SAFE_DELETE(m_eventLoop);
	}

	if (m_tcpServer)
	{
		NF_SAFE_DELETE(m_tcpServer);
	}

	m_eventLoop = nullptr;
	m_tcpServer = nullptr;
	return true;
}

bool NFEvppServer::Execute()
{
	ExecuteClose();
	ProcessMsgLogicThread();
	return true;
}

void NFEvppServer::ExecuteClose()
{
	for (size_t i = 0; i < mvRemoveObject.size(); i++)
	{
		uint32_t unLinkId = mvRemoveObject[i];
		uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
		uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

		NF_ASSERT_MSG(serverType == mServerType, "the unlinkId is not of the server");
		if (serverIndex < mNetObjectArray.size())
		{
			auto pObject = mNetObjectArray[serverIndex];
			if (pObject && pObject->GetNeedRemove())
			{
				mNetObjectArray[serverIndex] = nullptr;
				mNetObjectCount--;
				NF_SAFE_DELETE(pObject);
			}
			else
			{
				NF_ASSERT_MSG(false, "the NetObject of the unlinkId is not remove!");
			}
		}
		else
		{
			NF_ASSERT_MSG(false, "the unlinkId's index is not right!");
		}
	}

	mvRemoveObject.clear();
}

void NFEvppServer::OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (mRecvCB)
	{
		mRecvCB(unLinkId, playerId, nMsgId, msg, nLen);
	}
}

void NFEvppServer::OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_DISCONNECTED)
	{
		uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
		uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

		NF_ASSERT_MSG(serverType == mServerType, "the unlinkId is not of the server");
		if (serverIndex < mNetObjectArray.size())
		{
			auto pObject = mNetObjectArray[serverIndex];
			if (pObject && pObject->GetNeedRemove())
			{
				mvRemoveObject.push_back(unLinkId);
			}
			else
			{
				NF_ASSERT_MSG(pObject && pObject->GetNeedRemove(), "the pObject is nullptr or is not remove!");
			}
		}
		else
		{
			NF_ASSERT_MSG(serverIndex < mNetObjectArray.size(), "the unlinkId is not right!");
		}
	}

	if (mEventCB)
	{
		mEventCB(nEvent, unLinkId);
	}
}

bool NFEvppServer::SendAll(const void* pData, uint32_t unSize)
{
	for (size_t i = 0; i < mNetObjectArray.size(); i++)
	{
		auto pObject = mNetObjectArray[i];
		if (pObject)
		{
			return pObject->Send(pData, unSize);
		}
	}
	return true;
}

bool NFEvppServer::Send(uint32_t usLinkId, const void* pData, uint32_t unSize)
{
	NetEvppObject* pObject = GetNetObject(usLinkId);
	if (pObject)
	{
		return pObject->Send(pData, unSize);
	}
	return false;
}