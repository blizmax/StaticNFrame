// -------------------------------------------------------------------------
//    @FileName         :    NFCLoginServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLoginServerModule
//
// -------------------------------------------------------------------------

#include "NFLoginServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCLoginServerModule::NFCLoginServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCLoginServerModule::~NFCLoginServerModule()
{
}

bool NFCLoginServerModule::Init()
{
	m_pMongoModule = pPluginManager->FindModule<NFIMongoModule>();
	m_pAsynMongoModule = pPluginManager->FindModule<NFIAsynMongoModule>();
	m_pHttpServerModule = pPluginManager->FindModule<NFIHttpServerModule>();
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_LOGIN, this, &NFCLoginServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_LOGIN, this, &NFCLoginServerModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_LOGIN);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_LOGIN, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo("login server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo("login server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}

		if (pConfig->mHttpPort > 0)
		{
			int ret = m_pHttpServerModule->InitServer(NF_ST_LOGIN, pConfig->mHttpPort);
			if (ret == 0)
			{
				NFLogError("Login Server Open Http Port:{} Failed!", pConfig->mHttpPort);
				return false;
			}
			NFLogInfo("Login Server Open Http Port:{} Success!", pConfig->mHttpPort);
		}

		if (!pConfig->mMongoIp.empty())
		{
			if (pConfig->mMongoPort > 0)
			{
				bool ret = m_pMongoModule->AddMongoServer(NF_ST_LOGIN, pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
				if (ret == false)
				{
					NFLogError("Login Server Connected Mongo Failed, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
					return false;
				}

				ret = m_pAsynMongoModule->AddMongoServer(NF_ST_LOGIN, pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
				if (ret == false)
				{
					NFLogError("Login Server Connected Mongo Failed, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
					return false;
				}
				NFLogInfo("Login Server Connected Mongo Success, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
			}
		}
	}
	else
	{
		NFLogError("I Can't get the Login Server config!");
		return false;
	}

	return true;
}

bool NFCLoginServerModule::AfterInit()
{
	return true;
}

bool NFCLoginServerModule::Execute()
{
	return true;
}

bool NFCLoginServerModule::BeforeShut()
{
	return true;
}

bool NFCLoginServerModule::Shut()
{
	return true;
}

void NFCLoginServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("Client Connect Login Server Success, Ip:{}", ip);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("Client DisConnect From Login Server, Ip:{}", ip);
	}
}

void NFCLoginServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}
