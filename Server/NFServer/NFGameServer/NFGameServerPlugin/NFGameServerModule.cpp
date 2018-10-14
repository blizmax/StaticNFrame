// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFGameServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>

NFCGameServerModule::NFCGameServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCGameServerModule::~NFCGameServerModule()
{
}

bool NFCGameServerModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_GAME, this, &NFCGameServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_GAME, this, &NFCGameServerModule::OnHandleOtherMessage);

	if (pPluginManager->IsLoadAllServer())
	{
		std::vector<NFServerConfig*> vec = NFConfigMgr::Instance()->GetServerConfigFromServerType(NF_ST_GAME);
		NFServerConfig* pConfig = vec.front();
		if (pConfig)
		{
			m_pNetServerModule->AddServer(NF_ST_GAME, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFServerConfig* pConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
		if (pConfig)
		{
			m_pNetServerModule->AddServer(NF_ST_GAME, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	return true;
}

bool NFCGameServerModule::AfterInit()
{
	return true;
}

bool NFCGameServerModule::Execute()
{
	return true;
}

bool NFCGameServerModule::BeforeShut()
{
	return true;
}

bool NFCGameServerModule::Shut()
{
	return true;
}

void NFCGameServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("Proxy Server Connect Game Server Success, Ip:%s", ip.c_str());
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("Proxy Server DisConnect, Ip:%s", ip.c_str());
	}
}

void NFCGameServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}
