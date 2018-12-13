// -------------------------------------------------------------------------
//    @FileName         :    NFCGameClient_MasterModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFGameClient_MasterModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCGameClient_MasterModule::NFCGameClient_MasterModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
	m_unLinkId = 0;
}

NFCGameClient_MasterModule::~NFCGameClient_MasterModule()
{
}

bool NFCGameClient_MasterModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pGameClient_ProxyModule = pPluginManager->FindModule<NFIGameClient_ProxyModule>();
	m_pGameClient_WorldModule = pPluginManager->FindModule<NFIGameClient_WorldModule>();
	return true;
}

bool NFCGameClient_MasterModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCGameClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCGameClient_MasterModule::OnHandleOtherMessage);

	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCGameClient_MasterModule::OnHandleOtherMessage);
	
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_SEND_PROXY_TO_GAME, this, &NFCGameClient_MasterModule::OnHandleServerReport);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_SEND_WORLD_TO_GAME, this, &NFCGameClient_MasterModule::OnHandleServerReport);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		m_unLinkId = m_pNetClientModule->AddServer(NF_ST_MASTER, pConfig->mServerIp, pConfig->mServerPort);
	}
	else
	{
		NFLogError("I Can't get the Master Server config!");
		return false;
	}

	return true;
}

bool NFCGameClient_MasterModule::Execute()
{
	ServerReport();
	return true;
}

bool NFCGameClient_MasterModule::BeforeShut()
{
	return true;
}

bool NFCGameClient_MasterModule::Shut()
{
	return true;
}

void NFCGameClient_MasterModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Game Server Connect Master Server Success!");
		//NFEventMgr::Instance()->FireExecute(NFEVENT_GAME_CONNECT_WORLD_SUCCESS, unLinkId, NF_ST_WORLD, nullptr);

		//连接成功，发送游戏服务器IP以及数据给世界服务器
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Game Server DisConnect Master Server!");
		//NFEventMgr::Instance()->FireExecute(NFEVENT_GAME_CONNECT_WORLD_FAIL, unLinkId, NF_ST_WORLD, nullptr);
	}
}

void NFCGameClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_unLinkId) return;

	NFLogWarning("msg:{} not handled!", nMsgId);
}

void NFCGameClient_MasterModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_GAME);
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

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_GAME_TO_MASTER_REGISTER, xMsg, 0);
	}
}

void NFCGameClient_MasterModule::ServerReport()
{
	static uint64_t mLastReportTime = pPluginManager->GetNowTime();
	if (mLastReportTime + 10000 > pPluginManager->GetNowTime())
	{
		return;
	}

	mLastReportTime = pPluginManager->GetNowTime();

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_GAME);
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
		pData->set_server_cur_count(0);

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_STS_SERVER_REPORT, xMsg, 0);
	}
}

void NFCGameClient_MasterModule::OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		switch (xData.server_type())
		{
		case NF_SERVER_TYPES::NF_ST_WORLD:
		{
			m_pGameClient_WorldModule->OnHandleWorldReport(xData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_PROXY:
		{
			m_pGameClient_ProxyModule->OnHandleProxyReport(xData);
		}
		break;
		}
	}
}

