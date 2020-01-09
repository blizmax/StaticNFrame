// -------------------------------------------------------------------------
//    @FileName         :    NFEvppClient.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-06-04
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                               _(\_/) 
//                             ,((((^`\
//                            ((((  (6 \ 
//                          ,((((( ,    \
//      ,,,_              ,(((((  /"._  ,`,
//     ((((\\ ,...       ,((((   /    `-.-'
//     )))  ;'    `"'"'""((((   (      
//    (((  /            (((      \
//     )) |                      |
//    ((  |        .       '     |
//    ))  \     _ '      `t   ,.')
//    (   |   y;- -,-""'"-.\   \/  
//    )   / ./  ) /         `\  \
//       |./   ( (           / /'
//       ||     \\          //'|
//       ||      \\       _//'||
//       ||       ))     |_/  ||
//       \_\     |_/          ||
//       `'"                  \_\
//                            `'" 
// -------------------------------------------------------------------------

#include "NFEvppClient.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFIPacketParse.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFEvppServer.h"

NFEvppClient::NFEvppClient(evpp::EventLoop* eventLoop, uint32_t nId, const NFClientFlag& flag):NFIClient(nId, flag)
{
	m_safeExit.store(false);
	m_pObject = nullptr;
	m_tcpClient = nullptr;

	std::string strIpPort = NF_FORMAT("{}:{}", m_flag.strIP, m_flag.nPort);

	if (eventLoop)
	{
		m_tcpClient = NF_NEW evpp::TCPClient(eventLoop, strIpPort, "NFEvppClient");
	}
}

NFEvppClient::~NFEvppClient()
{

}

bool NFEvppClient::Init()
{
	if (!Connect())
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "NetError | error: connected failed!");
		return false;
	}
	return true;
}

bool NFEvppClient::Shut()
{
	CloseServer();
	return true;
}

void NFEvppClient::SafeExit()
{
	if (m_tcpClient)
	{
		NF_SAFE_DELETE(m_tcpClient);
	}
	m_safeExit.store(true);
}

bool NFEvppClient::Finalize()
{
	m_safeExit.store(false);
	m_tcpClient->loop()->QueueInLoop(std::bind(&NFEvppClient::SafeExit, this));
	while (!m_safeExit.load())
	{
		NFSLEEP(1);
	}
	m_tcpClient = nullptr;
	if (m_pObject)
	{
		NF_SAFE_DELETE(m_pObject);
	}
	return true;
}

bool NFEvppClient::Execute()
{
	ProcessMsgLogicThread();
	return true;
}

void NFEvppClient::CloseServer()
{
	if (m_pObject)
	{
		m_pObject->CloseObject();
	}
}

bool NFEvppClient::Connect()
{
	m_pObject = new NetEvppObject(nullptr);

	m_pObject->SetPacketParseType(GetPacketParseType());
	m_pObject->SetIsServer(false);
	m_pObject->SetLinkId(m_usLinkId);
	m_pObject->SetStrIp(m_flag.strIP);
	m_pObject->SetPort(m_flag.nPort);

	m_pObject->SetRecvCB(mRecvCB);
	m_pObject->SetEventCB(mEventCB);

	//消息回调是在别的线程里运行的
	m_tcpClient->SetConnectionCallback([this](const evpp::TCPConnPtr& conn)
	{
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
	m_tcpClient->SetMessageCallback([this](const evpp::TCPConnPtr& conn,
		evpp::Buffer* msg)
	{
		if (msg)
		{
			while (true)
			{
				char* outData = nullptr;
				uint32_t outLen = 0;
				uint32_t allLen = 0;
				uint32_t nMsgId = 0;
				uint64_t nValue = 0;
				uint32_t nOpreateId = 0;
				uint8_t nRpcType = 0;
				uint64_t nRpcReqId = 0;
				int ret = NFIPacketParse::DeCode(mPacketParseType, msg->data(), msg->size(), outData, outLen, allLen, nMsgId, nValue, nOpreateId, nRpcType, nRpcReqId);
				if (ret < 0)
				{
					NFLogError(NF_LOG_SYSTEMLOG, 0, "net client parse data failed!");
					msg->Reset();
					break;
				}
				else if (ret > 0)
				{
					break;
				}
				else
				{
					if (nRpcType > 0)
					{
						this->RpcCallBack(nRpcReqId, (uint32_t)NFRpcErrorCode::OK, std::string(outData, outLen));
						msg->Skip(allLen);
					}
					else
					{
						MsgFromNetInfo* pMsg = new MsgFromNetInfo(conn);
						pMsg->nType = eMsgType_RECIVEDATA;
						pMsg->strMsg = std::string(outData, outLen);
						pMsg->nMsgId = nMsgId;
						pMsg->nValue = nValue;
						pMsg->nOperateId = nOpreateId;
						mMsgQueue.Push(pMsg);

						msg->Skip(allLen);
					}

					continue;
				}
			}
		}
	});
	m_tcpClient->set_auto_reconnect(false);

	m_tcpClient->Connect();

	return true;
}

void NFEvppClient::ProcessMsgLogicThread()
{
	std::vector<MsgFromNetInfo*> vecMsg;
	mMsgQueue.Pop(vecMsg);
	for (size_t i = 0; i < vecMsg.size(); i++)
	{
		MsgFromNetInfo* pMsg = vecMsg[i];
		if (pMsg == nullptr) continue;

		if (pMsg->nType == eMsgType_CONNECTED)
		{
			if (m_pObject)
			{
				m_pObject->SetConnPtr(pMsg->mTCPConPtr);
				pMsg->mTCPConPtr->set_context(evpp::Any(m_pObject));
				m_pObject->OnHandleConnect();
			}
		}
		else if (pMsg->nType == eMsgType_DISCONNECTED)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject && pObject == m_pObject)
				{
					m_pObject->OnHandleDisConnect();
				}
				else
				{
					NFLogError(NF_LOG_SYSTEMLOG, 0, "pObject != m_pObject, thread client wrong!!");
				}
				pMsg->mTCPConPtr->set_context(evpp::Any(nullptr));
			}
			else
			{
				if (m_pObject)
				{
					m_pObject->OnHandleDisConnect();
				}
			}

			if (m_pObject)
			{
				NF_SAFE_DELETE(m_pObject);
				m_pObject = nullptr;
			}
		}
		else if (pMsg->nType == eMsgType_RECIVEDATA)
		{
			if (!pMsg->mTCPConPtr->context().IsEmpty())
			{
				NetEvppObject* pObject = evpp::any_cast<NetEvppObject*>(pMsg->mTCPConPtr->context());
				if (pObject && pObject == m_pObject)
				{
					pObject->OnHandleMsgPeer(eMsgType_RECIVEDATA, pObject->m_usLinkId, (char*)pMsg->strMsg.data(), pMsg->strMsg.length(), pMsg->nMsgId, pMsg->nValue, pMsg->nOperateId);
				}
				else
				{
					NFLogError(NF_LOG_SYSTEMLOG, 0, "pObject != m_pObject, thread client wrong!!");
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

bool NFEvppClient::Send(const uint8_t rpcType, const uint64_t rpcReqId, const char* msg, const uint32_t nLen)
{
	if (m_pObject)
	{
		return m_pObject->Send(rpcType, rpcReqId, msg, nLen);
	}
	return false;
}

bool NFEvppClient::Send(const void* pData, uint32_t unSize)
{
	if (m_pObject)
	{
		return m_pObject->Send(pData, unSize);
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
bool NFEvppClient::Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t opreateId)
{
	if (m_pObject)
	{
		return m_pObject->Send(nMsgID, msg, nLen, nPlayerID, opreateId);
	}
	return false;
}

std::future<NFRpcReqResult> NFEvppClient::AsyncCall(const std::string& rpc_name, const char* msg, const uint32_t nLen)
{
	auto p = std::make_shared<std::promise<NFRpcReqResult>>();
	std::future<NFRpcReqResult> future = p->get_future();

	uint64_t fu_id = 0;
	{
		std::unique_lock<std::mutex> lock(cb_mtx_);
		fu_id_++;
		fu_id = fu_id_;
		future_map_.emplace(fu_id, std::move(p));
	}


	Send((uint8_t)NFRpcRequestType::req_res, fu_id, msg, nLen);
	return future;
}

void NFEvppClient::AsyncCall(const std::string& rpc_name, std::function<void(uint32_t error_code, const std::string& data)> cb, uint32_t timeout, const char* msg, const uint32_t nLen)
{
	uint64_t cb_id = 0;
	{
		std::unique_lock<std::mutex> lock(cb_mtx_);
		callback_id_++;
		callback_id_ |= (uint64_t(1) << 63);
		cb_id = callback_id_;
		auto call = std::make_shared<call_t>(m_tcpClient->loop(), std::move(cb), timeout);
		call->start_timer();
		callback_map_.emplace(cb_id, call);
	}

	Send((uint8_t)NFRpcRequestType::req_res, cb_id, msg, nLen);
}

void NFEvppClient::RpcCallBack(uint64_t req_id, uint32_t ec, const std::string& data)
{
	temp_req_id_ = req_id;
	auto cb_flag = req_id >> 63;
	if (cb_flag) {
		std::shared_ptr<call_t> cl = nullptr;
		{
			std::unique_lock<std::mutex> lock(cb_mtx_);
			cl = std::move(callback_map_[req_id]);
		}

		assert(cl);
		if (!cl->has_timeout()) {
			cl->cancel();
			cl->callback(ec, data);
		}
		else {
			cl->callback((uint32_t)NFRpcErrorCode::TIMEOUT, data);
		}

		std::unique_lock<std::mutex> lock(cb_mtx_);
		callback_map_.erase(req_id);
	}
	else {
		std::unique_lock<std::mutex> lock(cb_mtx_);
		auto& f = future_map_[req_id];
		if (ec) {
			//LOG<<ec.message();
			if (!f) {
				//std::cout << "invalid req_id" << std::endl;
				return;
			}
		}

		assert(f);
		f->set_value(NFRpcReqResult{ data });
		future_map_.erase(req_id);
	}
}

void NFEvppClient::ClearRpcCache()
{
	{
		std::unique_lock<std::mutex> lock(cb_mtx_);
		callback_map_.clear();
		future_map_.clear();
	}
}

