// -------------------------------------------------------------------------
//    @FileName         :    NFCWorldClient_MasterModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFWorldClientPlugin
//
// -------------------------------------------------------------------------

#include "NFWorldClient_MasterModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCWorldClient_MasterModule::NFCWorldClient_MasterModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCWorldClient_MasterModule::~NFCWorldClient_MasterModule()
{
}

bool NFCWorldClient_MasterModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCWorldClient_MasterModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCWorldClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCWorldClient_MasterModule::OnHandleOtherMessage);

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

bool NFCWorldClient_MasterModule::Execute()
{
	return true;
}

bool NFCWorldClient_MasterModule::BeforeShut()
{
	return true;
}

bool NFCWorldClient_MasterModule::Shut()
{
	return true;
}

void NFCWorldClient_MasterModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("World Server Connect Master Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_WORLD_CONNECT_MASTER_SUCCESS, unLinkId, NF_ST_MASTER, nullptr);
	
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("World Server DisConnect Master Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_WORLD_CONNECT_MASTER_FAIL, unLinkId, NF_ST_MASTER, nullptr);
		m_unLinkId = 0;
	}
}

void NFCWorldClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_unLinkId) return;
}

void NFCWorldClient_MasterModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_WORLD);
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

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_WORLD_TO_MASTER_REGISTER, xMsg, 0);
	}
}
