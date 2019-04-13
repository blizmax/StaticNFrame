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

#include <NFComm/NFCore/NFJson.h>

NFCProxyServerModule::NFCProxyServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCProxyServerModule::~NFCProxyServerModule()
{
}

bool NFCProxyServerModule::Init()
{
	m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_PROXY, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort, pConfig->mWebSocket, 1);
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
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "ip:{} connected proxy server success!", ip);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "ip:{} disconnected proxy server!", ip);
	}
}

void NFCProxyServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "other message not handled:playerId:{},msgId:{},ip:{}", playerId, nMsgId, ip);
}