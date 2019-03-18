

#include "NFCProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFCore/NFMD5.h"

NFCProxyLogicModule::NFCProxyLogicModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCProxyLogicModule::~NFCProxyLogicModule()
{

}

bool NFCProxyLogicModule::Init()
{
	m_pServerNetEventModule = pPluginManager->FindModule<NFIServerNetEventModule>();

	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetProxyServerModule = pPluginManager->FindModule<NFIProxyServerModule>();

	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnProxySocketEvent);

	m_pServerNetEventModule->AddEventCallBack(NF_ST_PROXY, NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleGameEventCallBack);
	m_pServerNetEventModule->AddEventCallBack(NF_ST_PROXY, NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleWorldEventCallBack);
	return true;
}

void NFCProxyLogicModule::OnHandleGameEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		
	}
}

void NFCProxyLogicModule::OnHandleWorldEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
	
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