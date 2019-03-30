// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClient_MasterModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFCpu.h"

NFCProxyClient_MasterModule::NFCProxyClient_MasterModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCProxyClient_MasterModule::~NFCProxyClient_MasterModule()
{
}

bool NFCProxyClient_MasterModule::Init()
{
	m_pProxyClient_GameModule = m_pPluginManager->FindModule<NFIProxyClient_GameModule>();
	m_pProxyClient_WorldModule = m_pPluginManager->FindModule<NFIProxyClient_WorldModule>();
	m_pProxyClient_LoginModule = m_pPluginManager->FindModule<NFIProxyClient_LoginModule>();
	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	m_pMasterServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
	return true;
}

bool NFCProxyClient_MasterModule::AfterInit()
{
	m_pServerNetEventModule = m_pPluginManager->FindModule<NFIServerNetEventModule>();
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCProxyClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCProxyClient_MasterModule::OnHandleOtherMessage);

	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY, this, &NFCProxyClient_MasterModule::OnHandleServerReport);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(m_pPluginManager, NF_ST_MASTER);
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

bool NFCProxyClient_MasterModule::Execute()
{
	ServerReport();
	return true;
}

bool NFCProxyClient_MasterModule::BeforeShut()
{
	return true;
}

bool NFCProxyClient_MasterModule::Shut()
{
	return true;
}

void NFCProxyClient_MasterModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_inner_port(pConfig->mServerInnerPort);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		m_pNetClientModule->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_NET_PROXY_TO_MASTER_REGISTER, xMsg, 0);
	}

	m_pMasterServerData->SetSendString([this](const std::string& msg) {
		m_pNetClientModule->SendByServerID(m_pMasterServerData->mUnlinkId, 0, msg, 0);
	});
	m_pServerNetEventModule->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_PROXY, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
}

void NFCProxyClient_MasterModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Connect Master Server Success!");

		//连接成功，发送网关服务器IP以及数据给世界服务器
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server DisConnect Master Server!");
		m_pMasterServerData->SetSendString([this](const std::string& msg) {
			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "master disconnect, can't send msg:{}", msg);
		});
		m_pServerNetEventModule->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_PROXY, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);

	}
}

void NFCProxyClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "msg:{} not handled", nMsgId);
}

void NFCProxyClient_MasterModule::ServerReport()
{
	static uint64_t mLastReportTime = m_pPluginManager->GetNowTime();
	if (mLastReportTime + 10000 > m_pPluginManager->GetNowTime())
	{
		return;
	}

	mLastReportTime = m_pPluginManager->GetNowTime();

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_inner_port(pConfig->mServerInnerPort);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		m_pNetClientModule->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_STS_SERVER_REPORT, xMsg, 0);
	}
}

void NFCProxyClient_MasterModule::OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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
			m_pProxyClient_WorldModule->OnHandleServerReport(xData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_GAME:
		{
			m_pProxyClient_GameModule->OnHandleServerReport(xData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_LOGIN:
		{
			m_pProxyClient_LoginModule->OnHandleServerReport(xData);
		}
		break;
		}
	}
}


