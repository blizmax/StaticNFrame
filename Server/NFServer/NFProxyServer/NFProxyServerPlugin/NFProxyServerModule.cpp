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

NFCProxyServerModule::NFCProxyServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_gameServerUnlinkId = 0;
}

NFCProxyServerModule::~NFCProxyServerModule()
{
}

bool NFCProxyServerModule::Init()
{
	std::cout << "sizeof(NFProxyData)=" << sizeof(NFProxyData) << std::endl;
	Subscribe(NFEVENT_PROXY_CONNECT_GAME_SUCCESS, 0, NF_ST_GAME, __FUNCTION__);
	Subscribe(NFEVENT_PROXY_CONNECT_GAME_FAIL, 0, NF_ST_GAME, __FUNCTION__);

	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyServerModule::OnHandleOtherMessage);

	if (pPluginManager->IsLoadAllServer())
	{
		std::vector<NFServerConfig*> vec = NFConfigMgr::Instance()->GetServerConfigFromServerType(NF_ST_PROXY);
		NFServerConfig* pConfig = vec.front();
		if (pConfig)
		{
			m_pNetServerModule->AddServer(NF_ST_PROXY, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFServerConfig* pConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
		if (pConfig)
		{
			m_pNetServerModule->AddServer(NF_ST_PROXY, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
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
}

void NFCProxyServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		//NFLogDebug("ip:%s connected success!", ip.c_str());		
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		//NFLogError("ip:%s disconnected!", ip.c_str());		
	}
}

void NFCProxyServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (nMsgId >= emsgid_map_begin && nMsgId <= emsgid_map_end)
	{
		NFINetClientModule* pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
		if (pNetClientModule)
		{
			pNetClientModule->SendByServerID(m_gameServerUnlinkId, nMsgId, msg, nLen, playerId);
			return;
		}
	}
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFLogWarning("other message not handled:playerId:%ld,msgId:%d,ip:%s", playerId, nMsgId, ip.c_str());
}
