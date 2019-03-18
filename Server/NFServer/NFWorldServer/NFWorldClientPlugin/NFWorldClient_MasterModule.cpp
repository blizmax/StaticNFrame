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
#include "NFComm/NFCore/NFCpu.h"

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
	m_pServerNetEventModule = pPluginManager->FindModule<NFIServerNetEventModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pMasterServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
	return true;
}

bool NFCWorldClient_MasterModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCWorldClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCWorldClient_MasterModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		//AddServer会自动重连，断开连接时，m_pMasterServerData->mUnlinkId不用清理，不变
		m_pMasterServerData->mUnlinkId = m_pNetClientModule->AddServer(NF_ST_MASTER, pConfig->mServerIp, pConfig->mServerPort);
		m_pMasterServerData->mServerInfo.set_server_id(pConfig->mServerId);
		m_pMasterServerData->mServerInfo.set_server_ip(pConfig->mServerIp);
		m_pMasterServerData->mServerInfo.set_server_port(pConfig->mServerPort);
		m_pMasterServerData->mServerInfo.set_server_name(pConfig->mServerName);
		m_pMasterServerData->mServerInfo.set_server_type(pConfig->mServerType);
		m_pMasterServerData->mServerInfo.set_server_state(NFMsg::EST_NARMAL);
	}
	else
	{
		NFLogError("I Can't get the Master Server config!");
		return false;
	}

	return true;
}

bool NFCWorldClient_MasterModule::Execute()
{
	ServerReport();
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
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("World Server Connect Master Server Success!");
	
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("World Server DisConnect Master Server!");

		m_pMasterServerData->SetSendString([this](const std::string& msg) {
			NFLogError("master disconnect, can't send msg:{}", msg);
		});
		m_pServerNetEventModule->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_WORLD, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
	}
}

void NFCWorldClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;
}

void NFCWorldClient_MasterModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_WORLD);
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
		pData->set_game_id(pConfig->mGameId);
		pData->set_game_name(pConfig->mGameName);

		m_pNetClientModule->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_NET_WORLD_TO_MASTER_REGISTER, xMsg, 0);
	}

	m_pMasterServerData->SetSendString([this](const std::string& msg) {
		m_pNetClientModule->SendByServerID(m_pMasterServerData->mUnlinkId, 0, msg, 0);
	});
	m_pServerNetEventModule->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_WORLD, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
}

void NFCWorldClient_MasterModule::ServerReport()
{
	static uint64_t mLastReportTime = pPluginManager->GetNowTime();
	if (mLastReportTime + 10000 > pPluginManager->GetNowTime())
	{
		return;
	}

	mLastReportTime = pPluginManager->GetNowTime();

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_WORLD);
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
		pData->set_game_id(pConfig->mGameId);
		pData->set_game_name(pConfig->mGameName);
		pData->set_cur_cpu_used(pPluginManager->GetSystemInfo().GetProcessInfo().mCpuUsed);
		pData->set_cur_mem_used(pPluginManager->GetSystemInfo().GetProcessInfo().mMemUsed);
		pData->set_total_mem(pPluginManager->GetSystemInfo().GetMemInfo().mTotalMem);
		pData->set_used_mem(pPluginManager->GetSystemInfo().GetMemInfo().mUsedMem);
		pData->set_free_mem(pPluginManager->GetSystemInfo().GetMemInfo().mFreeMem);
		pData->set_cur_thread_num(pPluginManager->GetSystemInfo().GetProcessInfo().mThreads);
		pData->set_system_info(pPluginManager->GetSystemInfo().GetOsInfo().mOsDescription);
		pData->set_cur_proc_name(pPluginManager->GetSystemInfo().GetProcessInfo().mName);
		pData->set_cur_proc_cwd(pPluginManager->GetSystemInfo().GetProcessInfo().mCwd);

		m_pNetClientModule->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_STS_SERVER_REPORT, xMsg, 0);
	}
}
