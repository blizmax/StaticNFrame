// -------------------------------------------------------------------------
//    @FileName         :    NFCWorldServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCWorldServerModule
//
// -------------------------------------------------------------------------

#include "NFWorldServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include <NFComm/NFPluginModule/NFILuaScriptModule.h>

NFCWorldServerModule::NFCWorldServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCWorldServerModule::~NFCWorldServerModule()
{
}

bool NFCWorldServerModule::Init()
{
	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_WORLD, this, &NFCWorldServerModule::OnProxySocketEvent);
	//m_pNetServerModule->AddReceiveCallBack(NF_ST_WORLD, this, &NFCWorldServerModule::OnHandleOtherMessage);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_GAME_TO_WORLD_REGISTER, this, &NFCWorldServerModule::OnGameServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_GAME_TO_WORLD_UNREGISTER, this, &NFCWorldServerModule::OnGameServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_GAME_TO_WORLD_REFRESH, this, &NFCWorldServerModule::OnGameServerRefreshProcess);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_PROXY_TO_WORLD_REGISTER, this, &NFCWorldServerModule::OnProxyServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_PROXY_TO_WORLD_UNREGISTER, this, &NFCWorldServerModule::OnProxyServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_PROXY_TO_WORLD_REFRESH, this, &NFCWorldServerModule::OnProxyServerRefreshProcess);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_WORLD);
	if (pConfig)
	{
		if (!m_pPluginManager->IsLoadAllServer())
		{
			if (pConfig->mServerType != NF_ST_WORLD)
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
				exit(0);
			}
		}

		mServerId = pConfig->mServerId;
		uint32_t unlinkId = FindModule<NFINetServerModule>()->AddServer(NF_ST_WORLD, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "world server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "world server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the World Server config!");
		return false;
	}

	return true;
}

bool NFCWorldServerModule::AfterInit()
{
	return true;
}

bool NFCWorldServerModule::Execute()
{
	return true;
}

bool NFCWorldServerModule::BeforeShut()
{
	return true;
}

bool NFCWorldServerModule::Shut()
{
	return true;
}

void NFCWorldServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		OnClientDisconnect(unLinkId);
	}
}

void NFCWorldServerModule::OnClientDisconnect(uint32_t unLinkId)
{
	mLinkGamMap.RemoveElement(unLinkId);

	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the game server disconnect from world server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_WORLD, NF_ST_GAME, unLinkId, pServerData);
			return;
		}

		pServerData = mGameMap.Next();
	}

	pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the proxy server disconnect from world server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_WORLD, NF_ST_PROXY, unLinkId, pServerData);
			return;
		}

		pServerData = mProxyMap.Next();
	}
}

void NFCWorldServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		std::string strMsg(msg, nLen);
		pLuaScriptModule->RunNetRecvLuaFunc("LuaNFrame.DispatchWorldTcp", unLinkId, playerId, nMsgId, strMsg);
	}
	else
	{
		NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, playerId, "msg:{} not handled!", nMsgId);
	}
}

void NFCWorldServerModule::OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

		mGameMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server UnRegister World Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCWorldServerModule::OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mGameMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		//NFLogInfo("Game Server Refresh World Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCWorldServerModule::OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mGameMap.AddElement(xData.server_id(), pServerData);
		}

		if (mLinkGamMap.GetElement(unLinkId) == nullptr)
		{
			mLinkGamMap.AddElement(unLinkId, pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server Register World Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	
		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_WORLD, NF_ST_GAME, unLinkId, pServerData);
	}
}

void NFCWorldServerModule::OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}


		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Register World Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_WORLD, NF_ST_PROXY, unLinkId, pServerData);
	}
}

void NFCWorldServerModule::OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "World Server UnRegister Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCWorldServerModule::OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}


		//NFLogInfo("World Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

