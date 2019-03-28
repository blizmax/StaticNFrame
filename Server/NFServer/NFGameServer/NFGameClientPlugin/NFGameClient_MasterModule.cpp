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
#include "NFComm/NFCore/NFCpu.h"

NFCGameClient_MasterModule::NFCGameClient_MasterModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
	m_onlineNum = 0;
}

NFCGameClient_MasterModule::~NFCGameClient_MasterModule()
{
}

bool NFCGameClient_MasterModule::Init()
{
	m_pServerNetEventModule = pPluginManager->FindModule<NFIServerNetEventModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pGameClient_WorldModule = pPluginManager->FindModule<NFIGameClient_WorldModule>();
	m_pMasterServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
	return true;
}

bool NFCGameClient_MasterModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCGameClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCGameClient_MasterModule::OnHandleOtherMessage);

	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCGameClient_MasterModule::OnHandleOtherMessage);
	
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_SEND_OTHERS_TO_GAME, this, &NFCGameClient_MasterModule::OnHandleServerReport);

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
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Master Server config!");
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
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server Connect Master Server Success!");

		//连接成功，发送游戏服务器IP以及数据给世界服务器
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server DisConnect Master Server!");

		m_pMasterServerData->SetSendString([this](const std::string& msg) {
			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "master disconnect, can't send msg:{}", msg);
		});
		m_pServerNetEventModule->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_GAME, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
	}
}

void NFCGameClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "msg:{} not handled!", nMsgId);
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
		pData->set_game_id(pConfig->mGameId);
		pData->set_game_name(pConfig->mGameName);
		pData->set_server_state(NFMsg::EST_NARMAL);

		m_pNetClientModule->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_NET_GAME_TO_MASTER_REGISTER, xMsg, 0);
	}

	m_pMasterServerData->SetSendString([this](const std::string& msg) {
		m_pNetClientModule->SendByServerID(m_pMasterServerData->mUnlinkId, 0, msg, 0);
	});
	m_pServerNetEventModule->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_GAME, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
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
		pData->set_game_id(pConfig->mGameId);
		pData->set_game_name(pConfig->mGameName);
		pData->set_server_state(NFMsg::EST_NARMAL);
		pData->set_server_cur_count(m_onlineNum);

		m_pNetClientModule->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_STS_SERVER_REPORT, xMsg, 0);
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
		}
	}
}

