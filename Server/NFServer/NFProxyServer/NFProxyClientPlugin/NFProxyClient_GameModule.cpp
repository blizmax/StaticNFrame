// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClient_GameModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCProxyClient_GameModule::NFCProxyClient_GameModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCProxyClient_GameModule::~NFCProxyClient_GameModule()
{
}

bool NFCProxyClient_GameModule::Init()
{
	m_pServerNetEventModule = m_pPluginManager->FindModule<NFIServerNetEventModule>();
	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	
	return true;
}

bool NFCProxyClient_GameModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_GAME, this, &NFCProxyClient_GameModule::OnProxySocketEvent);
	//m_pNetClientModule->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyClient_GameModule::OnHandleOtherMessage);

	return true;
}

bool NFCProxyClient_GameModule::Execute()
{
	return true;
}

bool NFCProxyClient_GameModule::BeforeShut()
{
	return true;
}

bool NFCProxyClient_GameModule::Shut()
{
	mUnlinkGameMap.ClearAll();
	mGameMap.ClearAll();
	return true;
}

void NFCProxyClient_GameModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Connect Game Server Success!");

		//���ӳɹ����������ط�����IP�Լ����ݸ���Ϸ������
		RegisterServer(unLinkId);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server DisConnect Game Server!");

		OnClientDisconnect(unLinkId);
	}
}

void NFCProxyClient_GameModule::OnClientDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkGameMap.GetElement(unLinkId);
	if (pServerData)
	{
		//proxy server �� game server һ���Ͽ�������Ҫ������������, ����󽫹ر����unLinkId
		pServerData->mUnlinkId = 0;
		pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);

		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the game server disconnect, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
			, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		m_pServerNetEventModule->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_PROXY, NF_ST_GAME, unLinkId, pServerData);

		mUnlinkGameMap.RemoveElement(unLinkId);

		m_pNetClientModule->CloseServer(unLinkId);
	}
}

void NFCProxyClient_GameModule::OnHandleServerReport(const NFMsg::ServerInfoReport& xData)
{
	if (xData.server_type() != NF_ST_GAME) return;

	NF_SHARE_PTR<NFServerData> pServerData = GetServerByServerId(xData.server_id());
	if (pServerData == nullptr)
	{
		pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
		mGameMap.AddElement(xData.server_id(), pServerData);

		pServerData->mUnlinkId = m_pNetClientModule->AddServer(NF_ST_GAME, xData.server_ip(), xData.server_port());
		mUnlinkGameMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	if (pServerData->mUnlinkId <= 0)
	{
		pServerData->mUnlinkId = m_pNetClientModule->AddServer(NF_ST_GAME, xData.server_ip(), xData.server_port());
		mUnlinkGameMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	pServerData->mServerInfo = xData;
}

void NFCProxyClient_GameModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, playerId, "msg:{} not handled!", nMsgId);
}

void NFCProxyClient_GameModule::RegisterServer(uint32_t linkId)
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
		pData->set_game_name(pConfig->mGameName);

		m_pNetClientModule->SendToServerByPB(linkId, EGMI_NET_PROXY_TO_GAME_REGISTER, xMsg, 0);
	}

	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkGameMap.GetElement(linkId);
	if (pServerData)
	{
		m_pServerNetEventModule->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_PROXY, NF_ST_GAME, linkId, pServerData);
	}
}


