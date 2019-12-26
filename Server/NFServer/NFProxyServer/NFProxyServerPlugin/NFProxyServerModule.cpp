// -------------------------------------------------------------------------
//    @FileName         :    NFProxyServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyServerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFServer/NFServerCommon/NFServerCommon.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFIProxyClient_GameModule.h"
#include "NFServer/NFServerCommon/NFIProxyClient_LoginModule.h"
#include "NFServer/NFServerCommon/NFIProxyClient_WorldModule.h"
#include <NFComm/NFCore/NFJson.h>

NFCProxyServerModule::NFCProxyServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_pMasterClientModule = nullptr;
}

NFCProxyServerModule::~NFCProxyServerModule()
{
}

bool NFCProxyServerModule::Init()
{
	m_pMasterClientModule = m_pPluginManager->CreateAloneModule<NFICommonClient_MasterModule>();
	if (m_pMasterClientModule)
	{
		m_pMasterClientModule->SetServerType(NF_ST_PROXY);
		m_pMasterClientModule->Awake();
		m_pMasterClientModule->Init();
		m_pMasterClientModule->AfterInit();
		m_pMasterClientModule->ReadyExecute();
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "can't find NFICommonClient_MasterModule, connect master server failed!");
	}

	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_SEND_OTHERS_TO_SERVER, this, &NFCProxyServerModule::OnHandleServerReport);

	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnProxySocketEvent);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		if (!m_pPluginManager->IsLoadAllServer())
		{
			if (pConfig->mServerType != NF_ST_PROXY)
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
				exit(0);
			}
		}

		uint32_t unlinkId = FindModule<NFINetServerModule>()->AddServer(NF_ST_PROXY, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort, pConfig->mWebSocket, pConfig->mParseType, pConfig->bForeignNetwork, pConfig->bRefuseAttackIp);
		if (unlinkId != 0)
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "proxy server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "proxy server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
			return false;
		}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Proxy Server config!");
		return false;
	}

	return true;
}

bool NFCProxyServerModule::AfterInit()
{
	return true;
}

bool NFCProxyServerModule::Execute()
{
	return true;
}

bool NFCProxyServerModule::BeforeShut()
{
	return true;
}

bool NFCProxyServerModule::Shut()
{
	return true;
}

void NFCProxyServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "ip:{} connected proxy server success!", ip);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "ip:{} disconnected proxy server!", ip);
	}
}

void NFCProxyServerModule::OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterClientModule->GetMasterServerData()->mUnlinkId) return;

	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		switch (xData.server_type())
		{
		case NF_SERVER_TYPES::NF_ST_WORLD:
		{
			FindModule<NFIProxyClient_WorldModule>()->OnHandleServerReport(xData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_GAME:
		{
			FindModule<NFIProxyClient_GameModule>()->OnHandleServerReport(xData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_LOGIN:
		{
			FindModule<NFIProxyClient_LoginModule>()->OnHandleServerReport(xData);
		}
		break;
		}
	}
}