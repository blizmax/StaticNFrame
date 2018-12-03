// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#include "NFLoginClient_MasterModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCLoginClient_MasterModule::NFCLoginClient_MasterModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCLoginClient_MasterModule::~NFCLoginClient_MasterModule()
{
}

bool NFCLoginClient_MasterModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCLoginClient_MasterModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCLoginClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCLoginClient_MasterModule::OnHandleOtherMessage);

	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_TO_LOGIN_SEND_WORLD, this, &NFCLoginClient_MasterModule::OnHandleMasterSendWorldMessage);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		m_unLinkId = m_pNetClientModule->AddServer(NF_ST_MASTER, pConfig->mServerIp, pConfig->mServerPort);
	}
	else
	{
		NFLogError("I Can't get the Game Server config!");
		return false;
	}

	return true;
}

void NFCLoginClient_MasterModule::OnHandleMasterSendWorldMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);
	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

		NF_SHARE_PTR<NFMsg::ServerInfoReport> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFMsg::ServerInfoReport>(NF_NEW NFMsg::ServerInfoReport());
			*pServerData = xData;

			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

	}
}

bool NFCLoginClient_MasterModule::Execute()
{
	return true;
}

bool NFCLoginClient_MasterModule::BeforeShut()
{
	return true;
}

bool NFCLoginClient_MasterModule::Shut()
{
	return true;
}

void NFCLoginClient_MasterModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Login Server Connect Master Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_LOGIN_CONNECT_MASTER_SUCCESS, unLinkId, NF_ST_MASTER, nullptr);
	
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Login Server DisConnect Master Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_LOGIN_CONNECT_MASTER_FAIL, unLinkId, NF_ST_MASTER, nullptr);
		m_unLinkId = 0;
	}
}

void NFCLoginClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_unLinkId) return;
}

void NFCLoginClient_MasterModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_LOGIN);
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

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_LOGIN_TO_MASTER_REGISTER, xMsg, 0);
	}
}
