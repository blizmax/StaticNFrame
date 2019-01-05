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
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCGameServerModule::NFCGameServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCGameServerModule::~NFCGameServerModule()
{
}

bool NFCGameServerModule::Init()
{
	m_pMongoModule = pPluginManager->FindModule<NFIMongoModule>();
	m_pAsynMongoModule = pPluginManager->FindModule<NFIAsynMongoModule>();
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_GAME, this, &NFCGameServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_GAME, this, &NFCGameServerModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_GAME);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_GAME, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo("game server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo("game server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}

		if (!pConfig->mMongoIp.empty())
		{
			if (pConfig->mMongoPort > 0)
			{
				bool ret = m_pMongoModule->AddMongoServer(NF_ST_GAME, pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
				if (ret == false)
				{
					NFLogError("Game Server Connected Mongo Failed, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
					return false;
				}
				//0号，给LUA使用
				ret = m_pMongoModule->AddMongoServer(0, pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
				if (ret == false)
				{
					NFLogError("Game Server Connected Mongo Failed, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
					return false;
				}

				ret = m_pAsynMongoModule->AddMongoServer(NF_ST_GAME, pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
				if (ret == false)
				{
					NFLogError("Game Server Connected Mongo Failed, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
					return false;
				}
				//0号，给LUA使用
				ret = m_pAsynMongoModule->AddMongoServer(0, pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
				if (ret == false)
				{
					NFLogError("Game Server Connected Mongo Failed, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
					return false;
				}

				NFLogInfo("Game Server Connected Mongo Success, ip:{}, port:{}, dbname:{}", pConfig->mMongoIp, pConfig->mMongoPort, pConfig->mMongoDbName);
			}
		}
	}
	else
	{
		NFLogError("I Can't get the Game Server config!");
		return false;
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

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{

	}
}

void NFCGameServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFLogWarning("msg:{} not handled!", nMsgId);
}
