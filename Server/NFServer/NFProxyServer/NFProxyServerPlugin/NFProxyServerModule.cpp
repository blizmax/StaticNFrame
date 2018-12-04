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
	pPluginManager = p;
	m_gameServerUnlinkId = 0;
	m_worldServerUnlinkId = 0;
}

NFCProxyServerModule::~NFCProxyServerModule()
{
}

bool NFCProxyServerModule::Init()
{
	//监听服务器连接事件
	Subscribe(NFEVENT_PROXY_CONNECT_GAME_SUCCESS, 0, NF_ST_GAME, __FUNCTION__);
	Subscribe(NFEVENT_PROXY_CONNECT_GAME_FAIL, 0, NF_ST_GAME, __FUNCTION__);
	Subscribe(NFEVENT_PROXY_CONNECT_WORLD_SUCCESS, 0, NF_ST_WORLD, __FUNCTION__);
	Subscribe(NFEVENT_PROXY_CONNECT_WORLD_FAIL, 0, NF_ST_WORLD, __FUNCTION__);

	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_PROXY);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_PROXY, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort, pConfig->mWebSocket);
		if (unlinkId != 0)
		{
			NFLogInfo("proxy server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo("proxy server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFLogError("I Can't get the Proxy Server config!");
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

void NFCProxyServerModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{
	if (nEventID == NFEVENT_PROXY_CONNECT_GAME_SUCCESS)
	{
		m_gameServerUnlinkId = (uint32_t)nSrcID;
	}
	else if (nEventID == NFEVENT_PROXY_CONNECT_GAME_FAIL)
	{
		m_gameServerUnlinkId = 0;
	}
	else if (nEventID == NFEVENT_PROXY_CONNECT_WORLD_SUCCESS)
	{
		m_worldServerUnlinkId = (uint32_t)nSrcID;
	}
	else if (nEventID == NFEVENT_PROXY_CONNECT_WORLD_FAIL)
	{
		m_worldServerUnlinkId = 0;
	}
}

void NFCProxyServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogDebug("ip:{} connected proxy server success!", ip);		
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFLogError("ip:{} disconnected proxy server!", ip);		
	}
}

void NFCProxyServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (nMsgId > 0)
	{
		NFINetClientModule* pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
		if (pNetClientModule)
		{
			pNetClientModule->SendByServerID(m_gameServerUnlinkId, nMsgId, msg, nLen, playerId);
			return;
		}
	}
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFLogWarning("other message not handled:playerId:{},msgId:{},ip:{}", playerId, nMsgId, ip);
}
