// -------------------------------------------------------------------------
//    @FileName         :    NFCMasterServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCMasterServerModule
//
// -------------------------------------------------------------------------

#include "NFMasterServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCMasterServerModule::NFCMasterServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCMasterServerModule::~NFCMasterServerModule()
{
}

bool NFCMasterServerModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetServerConfig(pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_MASTER, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo("master server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo("master server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFLogError("I Can't get the Master Server config!");
		return false;
	}

	return true;
}

bool NFCMasterServerModule::AfterInit()
{
	return true;
}

bool NFCMasterServerModule::Execute()
{
	return true;
}

bool NFCMasterServerModule::BeforeShut()
{
	return true;
}

bool NFCMasterServerModule::Shut()
{
	return true;
}

void NFCMasterServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("Server Connect Master Server Success, Ip:{}", ip.c_str());
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("Server DisConnect From Master Server, Ip:{}", ip.c_str());
	}
}

void NFCMasterServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}
