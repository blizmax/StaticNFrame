

#include "NFCProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFCore/NFMD5.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"

NFCProxyLogicModule::NFCProxyLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCProxyLogicModule::~NFCProxyLogicModule()
{

}

bool NFCProxyLogicModule::Init()
{
	m_pServerNetEventModule = m_pPluginManager->FindModule<NFIServerNetEventModule>();

	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
	m_pNetProxyServerModule = m_pPluginManager->FindModule<NFIProxyServerModule>();

	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnHandleOtherMessage);

	m_pServerNetEventModule->AddEventCallBack(NF_ST_PROXY, NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleGameEventCallBack);
	m_pServerNetEventModule->AddEventCallBack(NF_ST_PROXY, NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleWorldEventCallBack);
	return true;
}

void NFCProxyLogicModule::OnHandleGameEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		mGameMap.AddElement(unLinkId, pServerData);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		mGameMap.RemoveElement(unLinkId);
	}
}

void NFCProxyLogicModule::OnHandleWorldEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		mWorldMap.AddElement(unLinkId, pServerData);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		mWorldMap.RemoveElement(unLinkId);
	}
}

bool NFCProxyLogicModule::AfterInit()
{
	return true;
}

bool NFCProxyLogicModule::Execute()
{
	return true;
}

bool NFCProxyLogicModule::BeforeShut()
{
	return true;
}

bool NFCProxyLogicModule::Shut()
{
	mGameMap.ClearAll();
	mWorldMap.ClearAll();
	return true;
}

void NFCProxyLogicModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		
	}
}

void NFCProxyLogicModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg -- ip:{}, operateId:{}, msgId:{}, len:{}", ip, nMsgId,  playerId, nLen);
	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
	if (pServerData)
	{
		m_pNetClientModule->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, 0);
	}
}