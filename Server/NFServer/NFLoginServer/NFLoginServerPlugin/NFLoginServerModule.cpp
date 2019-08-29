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
	m_pPluginManager = p;
}

NFCLoginServerModule::~NFCLoginServerModule()
{
}

bool NFCLoginServerModule::Init()
{
	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_LOGIN, this, &NFCLoginServerModule::OnProxySocketEvent);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, this, &NFCLoginServerModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_LOGIN);
	if (pConfig)
	{
		uint32_t unlinkId = FindModule<NFINetServerModule>()->AddServer(NF_ST_LOGIN, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "login server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "login server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}

		//if (pConfig->mHttpPort > 0)
		//{
		//	int ret = FindModule<NFIHttpServerModule>()->InitServer(NF_ST_LOGIN, pConfig->mHttpPort);
		//	if (ret == 0)
		//	{
		//		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "Login Server Open Http Port:{} Failed!", pConfig->mHttpPort);
		//		return false;
		//	}
		//	NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Login Server Open Http Port:{} Success!", pConfig->mHttpPort);
		//}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Login Server config!");
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
		std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Client Connect Login Server Success, Ip:{}", ip);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Client DisConnect From Login Server, Ip:{}", ip);
	}
}

void NFCLoginServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}
