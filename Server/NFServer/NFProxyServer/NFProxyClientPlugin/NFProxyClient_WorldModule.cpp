// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClient_WorldModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCProxyClient_WorldModule::NFCProxyClient_WorldModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCProxyClient_WorldModule::~NFCProxyClient_WorldModule()
{
}

bool NFCProxyClient_WorldModule::Init()
{
	return true;
}

bool NFCProxyClient_WorldModule::AfterInit()
{
	FindModule<NFINetClientModule>()->AddEventCallBack(NF_ST_WORLD, this, &NFCProxyClient_WorldModule::OnProxySocketEvent);
	//FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, this, &NFCProxyClient_WorldModule::OnHandleOtherMessage);

	return true;
}

bool NFCProxyClient_WorldModule::Execute()
{
	return true;
}

bool NFCProxyClient_WorldModule::BeforeShut()
{
	return true;
}

bool NFCProxyClient_WorldModule::Shut()
{
	mUnlinkWorldMap.ClearAll();
	mWorldMap.ClearAll();
	return true;
}

void NFCProxyClient_WorldModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Connect World Server Success!");

		//连接成功，发送网关服务器IP以及数据给World服务器
		RegisterServer(unLinkId);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server DisConnect World Server!");

		OnClientDisconnect(unLinkId);
	}
}

void NFCProxyClient_WorldModule::OnClientDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkWorldMap.GetElement(unLinkId);
	if (pServerData)
	{
		//proxy server 与 world server 一旦断开，不需要主动重新连接, 清理后将关闭这个unLinkId
		pServerData->mUnlinkId = 0;
		pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);

		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the world server disconnect, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
			, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_PROXY, NF_ST_WORLD, unLinkId, pServerData);

		mUnlinkWorldMap.RemoveElement(unLinkId);

		FindModule<NFINetClientModule>()->CloseServer(unLinkId);
	}
}

void NFCProxyClient_WorldModule::OnHandleServerReport(const NFMsg::ServerInfoReport& xData)
{
	if (xData.server_type() != NF_ST_WORLD) return;
	//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "proxy will connect world server");
	NF_SHARE_PTR<NFServerData> pServerData = GetServerByServerId(xData.server_id());
	if (pServerData == nullptr)
	{
		pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
		mWorldMap.AddElement(xData.server_id(), pServerData);

		pServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_WORLD, xData.server_ip(), xData.server_port());
		mUnlinkWorldMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	if (pServerData->mUnlinkId <= 0)
	{
		pServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_WORLD, xData.server_ip(), xData.server_port());
		mUnlinkWorldMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	pServerData->mServerInfo = xData;
}

void NFCProxyClient_WorldModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "msg:{} not handled!", nMsgId);
}

void NFCProxyClient_WorldModule::RegisterServer(uint32_t linkId)
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		FindModule<NFINetClientModule>()->SendToServerByPB(linkId, EGMI_NET_PROXY_TO_WORLD_REGISTER, xMsg, 0);
	}

	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkWorldMap.GetElement(linkId);
	if (pServerData)
	{
		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_PROXY, NF_ST_WORLD, linkId, pServerData);
	}
}



