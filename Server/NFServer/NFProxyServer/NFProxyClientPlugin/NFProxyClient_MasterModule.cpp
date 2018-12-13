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

NFCProxyClient_MasterModule::NFCProxyClient_MasterModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCProxyClient_MasterModule::~NFCProxyClient_MasterModule()
{
}

bool NFCProxyClient_MasterModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCProxyClient_MasterModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_MASTER, this, &NFCProxyClient_MasterModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCProxyClient_MasterModule::OnHandleOtherMessage);

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
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_PROXY);
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

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_PROXY_TO_MASTER_REGISTER, xMsg, 0);
	}
}

void NFCProxyClient_MasterModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Proxy Server Connect Master Server Success!");
		//NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_WORLD_SUCCESS, unLinkId, NF_ST_WORLD, nullptr);

		//连接成功，发送网关服务器IP以及数据给世界服务器
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Proxy Server DisConnect Master Server!");
		//NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_WORLD_FAIL, unLinkId, NF_ST_WORLD, nullptr);
	}
}

void NFCProxyClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogWarning("msg:{} not handled", nMsgId);
}

void NFCProxyClient_MasterModule::ServerReport()
{
	static uint64_t mLastReportTime = pPluginManager->GetNowTime();
	if (mLastReportTime + 10000 > pPluginManager->GetNowTime())
	{
		return;
	}

	mLastReportTime = pPluginManager->GetNowTime();

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_PROXY);
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
		pData->set_server_cur_count(0);

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_STS_SERVER_REPORT, xMsg, 0);
	}
}
