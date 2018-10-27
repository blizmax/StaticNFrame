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

	if (pPluginManager->IsLoadAllServer())
	{
		std::vector<NFServerConfig*> vec = NFConfigMgr::Instance()->GetServerConfigFromServerType(NF_ST_GAME);
		NFServerConfig* pConfig = vec.front();
		if (pConfig)
		{
			m_unLinkId = m_pNetClientModule->AddServer(NF_ST_GAME, pConfig->mServerIp, pConfig->mServerPort);
		}
	}
	else
	{
		NFServerConfig* pConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
		if (pConfig)
		{
			m_unLinkId = m_pNetClientModule->AddServer(NF_ST_GAME, pConfig->mServerIp, pConfig->mServerPort);;
		}
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
		//NFLogDebug("Proxy Server Connect Game Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_GAME_SUCCESS, unLinkId, NF_ST_GAME, nullptr);

		std::string msg = "{\"do\":\"Cmd.SendReqRecommendFriendCmd_C\"}";
		m_pNetClientModule->SendByServerID(unLinkId, 1111, msg, 0);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		//NFLogDebug("Proxy Server DisConnect Game Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_GAME_FAIL, unLinkId, NF_ST_GAME, nullptr);
		m_unLinkId = 0;
	}
}

void NFCProxyClient_GameModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_unLinkId) return;
}
