// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClient_WorldModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFMessageDefine/NFMsgDefine.h"

NFCProxyClient_WorldModule::NFCProxyClient_WorldModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCProxyClient_WorldModule::~NFCProxyClient_WorldModule()
{
}

bool NFCProxyClient_WorldModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCProxyClient_WorldModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_WORLD, this, &NFCProxyClient_WorldModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, this, &NFCProxyClient_WorldModule::OnHandleOtherMessage);

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

bool NFCProxyClient_WorldModule::Execute()
{
	return true;
}

bool NFCProxyClient_WorldModule::BeforeShut()
{
	return true;
}

bool NFCProxyClient_WorldModule::Shut()
{
	return true;
}

void NFCProxyClient_WorldModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		m_pNetClientModule->SendToServerByPB(m_unLinkId, EGMI_NET_PROXY_TO_WORLD_REGISTER, xMsg, 0);
	}
}

void NFCProxyClient_WorldModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_unLinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Proxy Server Connect World Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_WORLD_SUCCESS, unLinkId, NF_ST_WORLD, nullptr);

		//连接成功，发送网关服务器IP以及数据给世界服务器
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Proxy Server DisConnect World Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_WORLD_FAIL, unLinkId, NF_ST_WORLD, nullptr);
		m_unLinkId = 0;
	}
}

void NFCProxyClient_WorldModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogWarning("msg:{} not handled", nMsgId);
}
