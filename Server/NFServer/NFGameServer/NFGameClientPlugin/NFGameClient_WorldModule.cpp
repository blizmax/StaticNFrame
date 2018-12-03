// -------------------------------------------------------------------------
//    @FileName         :    NFGameClient_WorldModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFGameClient_WorldModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCGameClient_WorldModule::NFCGameClient_WorldModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
	m_unLinkId = 0;
}

NFCGameClient_WorldModule::~NFCGameClient_WorldModule()
{
}

bool NFCGameClient_WorldModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCGameClient_WorldModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_WORLD, this, &NFCGameClient_WorldModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, this, &NFCGameClient_WorldModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_WORLD);
	if (pConfig)
	{
		m_unLinkId = m_pNetClientModule->AddServer(NF_ST_WORLD, pConfig->mServerIp, pConfig->mServerPort);
	}
	else
	{
		NFLogError("I Can't get the Game Server config!");
		return false;
	}

	return true;
}

bool NFCGameClient_WorldModule::Execute()
{
	ServerReport();
	return true;
}

bool NFCGameClient_WorldModule::BeforeShut()
{
	return true;
}

bool NFCGameClient_WorldModule::Shut()
{
	return true;
}

void NFCGameClient_WorldModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Game Server Connect World Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_GAME_CONNECT_WORLD_SUCCESS, unLinkId, NF_ST_WORLD, nullptr);
		
		//连接成功，发送游戏服务器IP以及数据给世界服务器
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Game Server DisConnect World Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_GAME_CONNECT_WORLD_FAIL, unLinkId, NF_ST_WORLD, nullptr);
	}
}

void NFCGameClient_WorldModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_unLinkId) return;

	NFLogWarning("msg:{} not handled!", nMsgId);
}

void NFCGameClient_WorldModule::RegisterServer()
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

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_GAME_TO_WORLD_REGISTER, xMsg, 0);
	}
}

void NFCGameClient_WorldModule::ServerReport()
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


