// -------------------------------------------------------------------------
//    @FileName         :    NFCNetClientModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#include "NFCNetClientModule.h"
#include <iostream>
#include "NFIPacketParse.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFEvppClient.h"

NFCNetClientModule::NFCNetClientModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mxServerMap.resize(NF_ST_MAX);
	for (int serverType = NF_ST_NONE + 1; serverType < NF_ST_MAX; serverType++)
	{
		AddEventCallBack((NF_SERVER_TYPES)serverType, this, &NFCNetClientModule::OnHandleNetEvent);
	}
#ifdef USE_NET_EVPP
	m_eventLoop = nullptr;
	m_eventLoopThreadPool = nullptr;
#endif
}

NFCNetClientModule::~NFCNetClientModule()
{
}

bool NFCNetClientModule::Awake()
{
#ifdef USE_NET_EVPP
	if (m_pPluginManager->GetAppName() == "RebotServer")
	{
		InitThreadPool(10);
	}
	else
	{
		InitThreadPool(1);
	}
#endif
	return true;
}


bool NFCNetClientModule::Init()
{
	return true;
}

/**
 * @brief 初始化线程池
 */
void NFCNetClientModule::InitThreadPool(uint32_t threadCount)
{
#ifdef USE_NET_EVPP
	m_eventLoop = NF_NEW evpp::EventLoop();
	m_eventLoopThreadPool = NF_NEW evpp::EventLoopThreadPool(m_eventLoop, threadCount);
	m_eventLoopThreadPool->Start(true);
#endif
}

bool NFCNetClientModule::AfterInit()
{
	return true;
}

bool NFCNetClientModule::BeforeShut()
{
	mxCallBack.clear();
	return true;
}

bool NFCNetClientModule::Shut()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->Shut();
			}
		}
	}
	
	return true;
}

bool NFCNetClientModule::Finalize()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->Finalize();
			}
		}
	}
#ifdef USE_NET_EVPP
	m_eventLoopThreadPool->Stop(true);
#endif

	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				NF_SAFE_DELETE(mxServerMap[i][j]);
				mxServerMap[i][j] = nullptr;
			}
		}
	}
	mxServerMap.clear();
#ifdef USE_NET_EVPP
	if (m_eventLoopThreadPool)
	{
		NF_SAFE_DELETE(m_eventLoopThreadPool);
	}

	if (m_eventLoop)
	{
		NF_SAFE_DELETE(m_eventLoop);
	}
#endif
	return true;
}

bool NFCNetClientModule::Execute()
{
	ProcessExecute();
	return true;
}

uint32_t NFCNetClientModule::GetFreeUnLinkId(NF_SERVER_TYPES eServerType)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		size_t sz = mxServerMap[eServerType].size();

		for (size_t index = 0; index < sz; index++)
		{
			if (mxServerMap[eServerType][index] == nullptr)
			{	
				return GetUnLinkId(NF_IS_CLIENT, eServerType, index);
			}
		}

		if (sz >= MAX_SERVER_TYPE_CLIENT_COUNT)
		{
			return 0;
		}

		mxServerMap[eServerType].push_back(nullptr);

		uint32_t usLinkId = GetUnLinkId(NF_IS_CLIENT, eServerType, sz);

		return usLinkId;
	}
	return 0;
}

uint32_t NFCNetClientModule::AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, uint32_t nPort, uint32_t packetParsetype, bool bWebSocket)
{
	if (eServerType > NF_ST_NONE && eServerType < NF_ST_MAX)
	{
		uint32_t usId = GetFreeUnLinkId(eServerType);
		if (usId == 0)
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Add Connecting Server Failed! Ip:{}, Port:{}, than max connection:65535", strIp.c_str(), nPort);
			return 0;
		}

		uint32_t index = GetServerIndexFromUnlinkId(usId);

		NFClientFlag flag;
		flag.strIP = strIp;
		flag.nPort = nPort;
		flag.bWebSocket = bWebSocket;
		
#ifdef USE_NET_EVPP
		NFEvppClient* pClient = NF_NEW NFEvppClient(m_eventLoopThreadPool->GetNextLoop(), usId, flag);
#else
		
#endif

		pClient->SetPacketParseType(packetParsetype);
		pClient->SetRecvCB((NFINetModule*)this, &NFINetModule::OnReceiveNetPack);
		pClient->SetEventCB((NFINetModule*)this, &NFINetModule::OnSocketNetEvent);
		if (index < mxServerMap[eServerType].size() && mxServerMap[eServerType][index] == nullptr)
		{
			mxServerMap[eServerType][index] = pClient;
			return pClient->GetLinkId();
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "Add Connecting Server Failed! Ip:%s, Port:%d, than max connection:65535", strIp.c_str(), nPort);
		}
		NF_SAFE_DELETE(pClient);
	}
	return 0;
}

void NFCNetClientModule::CloseServer(const uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

	uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);
	if (isServer != NF_IS_CLIENT)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a client link, this usLinkId:{} is not of the client", unLinkId);
		return;
	}

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			pClient->CloseServer();
			pClient->SetStatus(eConnectStatus_REMOVE);
		}
	}
}

/**
 * @brief 关闭unLinkId所代表的连接
 *
 * @param  unLinkId 要关闭的客户端的唯一id
 * @return void
 */
void NFCNetClientModule::DisconnectServer(const uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);

	uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);
	if (isServer != NF_IS_CLIENT)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a client link, this usLinkId:{} is not of the client", unLinkId);
		return;
	}

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			pClient->CloseServer();
		}
	}
}

void NFCNetClientModule::CloseServerByServerType(NF_SERVER_TYPES eServerType)
{
	if (eServerType < NF_ST_MAX)
	{
		for (size_t j = 0; j < mxServerMap[eServerType].size(); j++)
		{
			if (mxServerMap[eServerType][j])
			{
				mxServerMap[eServerType][j]->CloseServer();
				mxServerMap[eServerType][j]->SetStatus(eConnectStatus_REMOVE);
			}
		}
	}
}

void NFCNetClientModule::CloseAllServer()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				mxServerMap[i][j]->CloseServer();
				mxServerMap[i][j]->SetStatus(eConnectStatus_REMOVE);
			}
		}
	}
}

void NFCNetClientModule::SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendByServerID(unLinkId, nMsgID, strData, nPlayerID, operateId);
}

void NFCNetClientModule::SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId)
{
	SendByServerID(unLinkId, nMsgID, strData.c_str(), strData.length(), nPlayerID, operateId);
}

void NFCNetClientModule::SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);
	uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);
	if (isServer != NF_IS_CLIENT)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a client link, this usLinkId:{} is not of the client", unLinkId);
		return;
	}

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			SendMsg(pClient, nMsgID, msg, nLen, nPlayerID, operateId);
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is a client link, but client not exist, this usLinkId:{}", unLinkId);
		}
	}
}

void NFCNetClientModule::SendMsg(NFIClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	if (pClient == nullptr) return;

	pClient->Send(nMsgID, msg, nLen, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId)
{
	SendToAllServer(nMsgID, strData.c_str(), strData.length(), nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			if (mxServerMap[i][j])
			{
				SendMsg(mxServerMap[i][j], nMsgID, msg, nLen, nPlayerID, operateId);
			}
		}
	}
}

void NFCNetClientModule::SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendToAllServer(nMsgID, strData, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId)
{
	SendToAllServer(eServerType, nMsgID, strData.c_str(), strData.length(), nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	if (eServerType < NF_ST_MAX)
	{
		for (size_t j = 0; j < mxServerMap[eServerType].size(); j++)
		{
			if (mxServerMap[eServerType][j])
			{
				SendMsg(mxServerMap[eServerType][j], nMsgID, msg, nLen, nPlayerID, operateId);
			}
		}
	}
}

void NFCNetClientModule::SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId)
{
	std::string strData;
	if (!xData.SerializeToString(&strData))
	{
		return;
	}

	SendToAllServer(nMsgID, strData, nPlayerID, operateId);
}

void NFCNetClientModule::ProcessExecute()
{
	for (size_t i = 0; i < mxServerMap.size(); i++)
	{
		for (size_t j = 0; j < mxServerMap[i].size(); j++)
		{
			auto pClient = mxServerMap[i][j];
			if (pClient)
			{
				switch (pClient->GetStatus())
				{
				case eConnectStatus_Disconnect:
					{
						pClient->SetStatus(eConnectStatus_RECONNECT);
					}
					break;
				case eConnectStatus_Connecting:
					{
						pClient->Execute();
					}
					break;
				case eConnectStatus_ConnectOk:
					{
						pClient->Execute();
						KeepState(pClient);
					}
					break;
				case eConnectStatus_RECONNECT:
					{
						if (pClient->GetLastActionTime() + 10000 >= (uint64_t)NFTime::Tick())
						{
							break;
						}

						pClient->SetStatus(eConnectStatus_Connecting);
						pClient->Init();
					}
					break;
				case eConnectStatus_REMOVE:
					{
						pClient->Shut();
						pClient->Finalize();
						NF_SAFE_DELETE(mxServerMap[i][j]);
						mxServerMap[i][j] = nullptr;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void NFCNetClientModule::KeepState(NFIClient* pClient)
{
	if (pClient)
	{
		if (pClient->GetLastActionTime() + 10000 >= (uint64_t)NFTime::Tick()) return;

		pClient->SetLastActionTime(NFTime::Tick());
	}
}

void NFCNetClientModule::OnHandleNetEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);
	uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);
	if (isServer != NF_IS_CLIENT)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a client link, this usLinkId:{} is not of the client", unLinkId);
		return;
	}

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			switch (nEvent)
			{
			case eMsgType_CONNECTED:
				{
					OnConnected(pClient);
				}
				break;
			case eMsgType_DISCONNECTED:
				{
					OnDisConnected(pClient);
				}
				break;
			default:
				break;
			}
		}
	}
}

void NFCNetClientModule::OnConnected(NFIClient* pClient)
{
	if (pClient)
	{
		pClient->SetStatus(eConnectStatus_ConnectOk);
	}
}

void NFCNetClientModule::OnDisConnected(NFIClient* pClient)
{
	if (pClient)
	{
		if (pClient->GetStatus() != eConnectStatus_REMOVE)
		{
			pClient->SetStatus(eConnectStatus_Disconnect);
			pClient->SetLastActionTime(NFTime::Tick());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
void NFCNetClientModule::SendByServerID(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendByServerID(unLinkId, msgId, strData, nPlayerID, operateId);
}

void NFCNetClientModule::SendByServerID(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendByServerID(unLinkId, msgId, msg, nLen, nPlayerID, operateId);
}

void NFCNetClientModule::SendToServerByPB(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToServerByPB(unLinkId, msgId, xData, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToAllServer(msgId, strData, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToAllServer(msgId, msg, nLen, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServerByPB(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToAllServerByPB(msgId, xData, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToAllServer(eServerType, msgId, strData, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToAllServer(eServerType, msgId, msg, nLen, nPlayerID, operateId);
}

void NFCNetClientModule::SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId)
{
	uint32_t msgId = MAKE_UINT32(nSubMsgID, nMainMsgID);
	SendToAllServerByPB(eServerType, msgId, xData, nPlayerID, operateId);
}

NFIClient* NFCNetClientModule::GetClientByUnlinkId(uint32_t unLinkId)
{
	uint32_t serverType = GetServerTypeFromUnlinkId(unLinkId);
	uint32_t serverIndex = GetServerIndexFromUnlinkId(unLinkId);
	uint32_t isServer = GetIsServerFromUnlinkId(unLinkId);
	if (isServer != NF_IS_CLIENT)
	{
		NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is not a client link, this usLinkId:{} is not of the client", unLinkId);
		return nullptr;
	}

	if (serverType < NF_ST_MAX && serverIndex < mxServerMap[serverType].size())
	{
		NFIClient* pClient = mxServerMap[serverType][serverIndex];
		if (pClient)
		{
			return pClient;
		}
		else
		{
			NFLogError(NF_LOG_NET_PLUGIN, 0, "usLinkId is a client link, but client not exist, this usLinkId:{}", unLinkId);
		}
	}

	return nullptr;
}

std::future<NFRpcReqResult> NFCNetClientModule::AsyncCall(uint32_t unLinkId, const std::string& rpc_name, const char* msg, const uint32_t nLen)
{
	NFIClient* pClient = GetClientByUnlinkId(unLinkId);
	if (pClient)
	{
		return pClient->AsyncCall(rpc_name, msg, nLen);
	}

	auto p = std::make_shared<std::promise<NFRpcReqResult>>();
	std::future<NFRpcReqResult> future = p->get_future();
	return future;
}

void NFCNetClientModule::AsyncCall(uint32_t unLinkId, const std::string& rpc_name, std::function<void(uint32_t error_code, const std::string& data)> cb, uint32_t timeout, const char* msg, const uint32_t nLen)
{
	NFIClient* pClient = GetClientByUnlinkId(unLinkId);
	if (pClient)
	{
		pClient->AsyncCall(rpc_name, cb, timeout, msg, nLen);
	}
}
