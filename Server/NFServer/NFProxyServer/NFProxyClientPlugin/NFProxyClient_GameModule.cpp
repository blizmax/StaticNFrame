// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
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
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCProxyClient_GameModule::~NFCProxyClient_GameModule()
{
}

bool NFCProxyClient_GameModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCProxyClient_GameModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_GAME, this, &NFCProxyClient_GameModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyClient_GameModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_GAME);
	if (pConfig)
	{
		m_unLinkId = m_pNetClientModule->AddServer(NF_ST_GAME, pConfig->mServerIp, pConfig->mServerPort);
	}
	else
	{
		NFLogError("I Can't get the Game Server config!");
		return false;
	}

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
	return true;
}

void NFCProxyClient_GameModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Proxy Server Connect Game Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_GAME_SUCCESS, unLinkId, NF_ST_GAME, nullptr);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Proxy Server DisConnect Game Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_GAME_FAIL, unLinkId, NF_ST_GAME, nullptr);
		m_unLinkId = 0;
	}
}

void NFCProxyClient_GameModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_unLinkId) return;
}

void NFCProxyClient_GameModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_PROXY_TO_GAME_REGISTER, xMsg, 0);
	}
}

