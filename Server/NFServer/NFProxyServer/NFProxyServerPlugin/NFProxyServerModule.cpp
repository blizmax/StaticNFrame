// -------------------------------------------------------------------------
//    @FileName         :    NFProxyServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyServerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFServer/NFServerCommon/NFServerCommon.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>

#include <NFComm/NFCore/NFJson.h>

NFCProxyServerModule::NFCProxyServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCProxyServerModule::~NFCProxyServerModule()
{
}

bool NFCProxyServerModule::Init()
{
	m_pServerNetEventModule = pPluginManager->FindModule<NFIServerNetEventModule>();
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnHandleOtherMessage);

	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY_INNER, this, &NFCProxyServerModule::OnProxyInnerSocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, this, &NFCProxyServerModule::OnHandleInnerOtherMessage);

	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_WORLD_TO_PROXY_REGISTER, this, &NFCProxyServerModule::OnWorldServerRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_WORLD_TO_PROXY_UNREGISTER, this, &NFCProxyServerModule::OnWorldServerUnRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_WORLD_TO_PROXY_REFRESH, this, &NFCProxyServerModule::OnWorldServerRefreshProcess);

	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_GAME_TO_PROXY_REGISTER, this, &NFCProxyServerModule::OnGameServerRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_GAME_TO_PROXY_UNREGISTER, this, &NFCProxyServerModule::OnGameServerUnRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_GAME_TO_PROXY_REFRESH, this, &NFCProxyServerModule::OnGameServerRefreshProcess);


	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_PROXY, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort, pConfig->mWebSocket);
		if (unlinkId != 0)
		{
			NFLogInfo("proxy server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo("proxy server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
			return false;
		}

		unlinkId = m_pNetServerModule->AddServer(NF_ST_PROXY_INNER, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerInnerPort);
		if (unlinkId != 0)
		{
			NFLogInfo("proxy inner server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerInnerPort);
		}
		else
		{
			NFLogInfo("proxy inner server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerInnerPort);
			return false;
		}
	}
	else
	{
		NFLogError("I Can't get the Proxy Server config!");
		return false;
	}

	return true;
}

//游戏服务器注册协议回调
void NFCProxyServerModule::OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		NFLogInfo("Game Server Register Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	
		pServerData->SetSendString([this, pServerData](const std::string& msg) {
			m_pNetServerModule->SendByServerID(pServerData->mUnlinkId, 0, msg, 0);
		});
		m_pServerNetEventModule->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_PROXY, NF_ST_GAME, unLinkId, pServerData);
	}
}

void NFCProxyServerModule::OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mGameMap.RemoveElement(xData.server_id());

		NFLogInfo("Game Server UnRegister Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCProxyServerModule::OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		//NFLogInfo("Game Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

bool NFCProxyServerModule::AfterInit()
{
	return true;
}

bool NFCProxyServerModule::Execute()
{
	return true;
}

bool NFCProxyServerModule::BeforeShut()
{
	return true;
}

bool NFCProxyServerModule::Shut()
{
	return true;
}

void NFCProxyServerModule::OnProxyInnerSocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		OnHandleInnerServerDisconnect(unLinkId);
	}
}

void NFCProxyServerModule::OnHandleInnerOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFLogWarning("game or world send other message not handled:playerId:{},msgId:{},ip:{}", playerId, nMsgId, ip);
}

void NFCProxyServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("ip:{} connected proxy server success!", ip);		
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogError("ip:{} disconnected proxy server!", ip);		
	}
}

void NFCProxyServerModule::OnHandleInnerServerDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = nullptr;
	pServerData = mWorldMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError("the world server disconnect from proxy server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			pServerData->SetSendString([this, pServerData](const std::string& msg) {
				NFLogError("world disconnect, can't send msg:{}", msg);
			});
			m_pServerNetEventModule->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_PROXY, NF_ST_WORLD, unLinkId, pServerData);
			return;
		}

		pServerData = mWorldMap.Next();
	}

	pServerData = mGameMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError("the game server disconnect from proxy server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			pServerData->SetSendString([this, pServerData](const std::string& msg) {
				NFLogError("game disconnect, can't send msg:{}", msg);
			});
			m_pServerNetEventModule->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_PROXY, NF_ST_GAME, unLinkId, pServerData);
			return;
		}

		pServerData = mGameMap.Next();
	}
}

void NFCProxyServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFLogWarning("other message not handled:playerId:{},msgId:{},ip:{}", playerId, nMsgId, ip);
}

void NFCProxyServerModule::OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}


		NFLogInfo("World Server Register Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	
		pServerData->SetSendString([this, pServerData](const std::string& msg) {
			m_pNetServerModule->SendByServerID(pServerData->mUnlinkId, 0, msg, 0);
		});
		m_pServerNetEventModule->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_PROXY, NF_ST_WORLD, unLinkId, pServerData);
	}
}

void NFCProxyServerModule::OnWorldServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mWorldMap.RemoveElement(xData.server_id());

		NFLogInfo("World Server UnRegister Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCProxyServerModule::OnWorldServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}


		//NFLogInfo("World Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}
