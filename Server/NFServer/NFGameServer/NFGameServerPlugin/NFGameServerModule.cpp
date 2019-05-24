// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFGameServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCGameServerModule::NFCGameServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCGameServerModule::~NFCGameServerModule()
{
}

bool NFCGameServerModule::Init()
{
	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_GAME, this, &NFCGameServerModule::OnProxySocketEvent);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, this, &NFCGameServerModule::OnHandleOtherMessage);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_TO_GAME_REGISTER, this, &NFCGameServerModule::OnProxyServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_TO_GAME_UNREGISTER, this, &NFCGameServerModule::OnProxyServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_TO_GAME_REFRESH, this, &NFCGameServerModule::OnProxyServerRefreshProcess);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_GAME);
	if (pConfig)
	{
		uint32_t unlinkId = FindModule<NFINetServerModule>()->AddServer(NF_ST_GAME, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "game server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "game server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Game Server config!");
		return false;
	}

	return true;
}

bool NFCGameServerModule::AfterInit()
{
	return true;
}

bool NFCGameServerModule::Execute()
{
	return true;
}

bool NFCGameServerModule::BeforeShut()
{
	return true;
}

bool NFCGameServerModule::Shut()
{
	return true;
}

void NFCGameServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		OnHandleServerDisconnect(unLinkId);
	}
}

void NFCGameServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, playerId, "msg:{} not handled!", nMsgId);
}

//游戏服务器注册协议回调
void NFCGameServerModule::OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Register Game Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_GAME, NF_ST_PROXY, unLinkId, pServerData);
	}
}

void NFCGameServerModule::OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server UnRegister Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCGameServerModule::OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

		//NFLogInfo("Game Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCGameServerModule::OnHandleServerDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = nullptr;

	pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the game server disconnect from proxy server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());


			FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_GAME, NF_ST_PROXY, unLinkId, pServerData);
			return;
		}

		pServerData = mProxyMap.Next();
	}
}
