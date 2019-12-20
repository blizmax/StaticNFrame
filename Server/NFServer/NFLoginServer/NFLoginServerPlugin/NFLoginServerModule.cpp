// -------------------------------------------------------------------------
//    @FileName         :    NFCLoginServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLoginServerModule
//
// -------------------------------------------------------------------------

#include "NFLoginServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCLoginServerModule::NFCLoginServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCLoginServerModule::~NFCLoginServerModule()
{
}

bool NFCLoginServerModule::Init()
{
	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_LOGIN, this, &NFCLoginServerModule::OnProxySocketEvent);
	
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_TO_LOGIN_REGISTER, this, &NFCLoginServerModule::OnProxyServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_TO_LOGIN_UNREGISTER, this, &NFCLoginServerModule::OnProxyServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_TO_LOGIN_REFRESH, this, &NFCLoginServerModule::OnProxyServerRefreshProcess);
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_LOGIN);
	if (pConfig)
	{
		if (!m_pPluginManager->IsLoadAllServer())
		{
			if (pConfig->mServerType != NF_ST_LOGIN)
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
				exit(0);
			}
		}

		uint32_t unlinkId = FindModule<NFINetServerModule>()->AddServer(NF_ST_LOGIN, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "login server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "login server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Login Server config!");
		return false;
	}

	return true;
}

bool NFCLoginServerModule::AfterInit()
{
	return true;
}

bool NFCLoginServerModule::Execute()
{
	return true;
}

bool NFCLoginServerModule::BeforeShut()
{
	return true;
}

bool NFCLoginServerModule::Shut()
{
	return true;
}

void NFCLoginServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Connect Login Server Success, Ip:{}", ip);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy DisConnect From Login Server, Ip:{}", ip);

		OnHandleServerDisconnect(unLinkId);
	}
}

void NFCLoginServerModule::OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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


		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Register Login Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_LOGIN, NF_ST_PROXY, unLinkId, pServerData);
	}
}

void NFCLoginServerModule::OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Login Server UnRegister Proxy Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCLoginServerModule::OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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


		//NFLogInfo("Proxy Server Refresh Login Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCLoginServerModule::OnHandleServerDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = nullptr;

	pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the login server disconnect from proxy server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());


			FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_LOGIN, NF_ST_PROXY, unLinkId, pServerData);
			return;
		}

		pServerData = mProxyMap.Next();
	}
}
