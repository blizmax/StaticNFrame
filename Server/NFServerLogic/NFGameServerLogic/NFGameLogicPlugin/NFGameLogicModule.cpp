

#include "NFGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCGameLogicModule::NFCGameLogicModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCGameLogicModule::~NFCGameLogicModule()
{

}

bool NFCGameLogicModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	
	m_pNetClientModule->AddEventCallBack(NF_ST_PROXY, this, &NFCGameLogicModule::OnProxySocketEvent);

	m_pNetClientModule->AddReceiveCallBack(NF_ST_PROXY, EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT, this, &NFCGameLogicModule::OnHandleAccountConnect);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_PROXY, EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT, this, &NFCGameLogicModule::OnHandleAccountDisConnect);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_PROXY, EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT, this, &NFCGameLogicModule::OnHandleAccountReConnect);
	return true;
}

bool NFCGameLogicModule::AfterInit()
{
	return true;
}

bool NFCGameLogicModule::Execute()
{
	return true;
}

bool NFCGameLogicModule::BeforeShut()
{
	return true;
}

bool NFCGameLogicModule::Shut()
{
	auto pData = m_playerAccountInfo.First();
	while (pData)
	{
		NF_SAFE_DELETE(pData);
		pData = m_playerAccountInfo.Next();
	}
	m_playerAccountInfo.ClearAll();
	return true;
}

void NFCGameLogicModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{

}

void NFCGameLogicModule::OnHandleAccountConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::AccountConnectGameServer_C xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	PlayerAccountInfo* pInfo = GetPlayerAccountInfo(playerId);
	if (pInfo == nullptr)
	{
		pInfo = NF_NEW PlayerAccountInfo();
		m_playerAccountInfo.AddElement(playerId, pInfo);
	}

	pInfo->uid = xMsg.uid();
	pInfo->account = xMsg.account();
	pInfo->ip = xMsg.ip();
	pInfo->proxyUnlinkId = unLinkId;

	pInfo->SetSendMsg([pInfo, this](const std::string& msg) {
		this->m_pNetClientModule->SendByServerID(pInfo->proxyUnlinkId, 0, msg.data(), msg.length(), pInfo->uid);
	});

	NFILuaScriptModule* pLuaScriptModule = (NFILuaScriptModule*)pPluginManager->FindModule(typeid(NFILuaScriptModule).name());
	if (pLuaScriptModule)
	{
		pLuaScriptModule->RunAccountConnectFunc(pInfo);
	}
}

void NFCGameLogicModule::OnHandleAccountDisConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::AccountConnectGameServer_C xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	PlayerAccountInfo* pInfo = GetPlayerAccountInfo(playerId);
	if (pInfo)
	{
		pInfo->uid = xMsg.uid();
		pInfo->account = xMsg.account();
		pInfo->ip = xMsg.ip();
		pInfo->proxyUnlinkId = 0;

		NFILuaScriptModule* pLuaScriptModule = (NFILuaScriptModule*)pPluginManager->FindModule(typeid(NFILuaScriptModule).name());
		if (pLuaScriptModule)
		{
			pLuaScriptModule->RunAccountDisConnectFunc(pInfo);
		}
	}
}

void NFCGameLogicModule::OnHandleAccountReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::AccountConnectGameServer_C xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	PlayerAccountInfo* pInfo = GetPlayerAccountInfo(playerId);
	if (pInfo == nullptr)
	{
		pInfo = NF_NEW PlayerAccountInfo();
		m_playerAccountInfo.AddElement(playerId, pInfo);
	}

	pInfo->uid = xMsg.uid();
	pInfo->account = xMsg.account();
	pInfo->ip = xMsg.ip();
	pInfo->proxyUnlinkId = unLinkId;

	pInfo->SetSendMsg([pInfo, this](const std::string& msg) {
		this->m_pNetClientModule->SendByServerID(pInfo->proxyUnlinkId, 0, msg.data(), msg.length(), pInfo->uid);
	});

	NFILuaScriptModule* pLuaScriptModule = (NFILuaScriptModule*)pPluginManager->FindModule(typeid(NFILuaScriptModule).name());
	if (pLuaScriptModule)
	{
		pLuaScriptModule->RunAccountReConnectFunc(pInfo);
	}
}

void NFCGameLogicModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		OnHandleProxyDisconnect(unLinkId);
	}
}

void NFCGameLogicModule::OnHandleProxyDisconnect(const uint32_t unLinkId)
{
	NFILuaScriptModule* pLuaScriptModule = (NFILuaScriptModule*)pPluginManager->FindModule(typeid(NFILuaScriptModule).name());
	PlayerAccountInfo* pInfo = m_playerAccountInfo.First();
	while (pInfo)
	{
		if (pInfo->proxyUnlinkId == unLinkId)
		{
			pInfo->proxyUnlinkId = 0;
			if (pLuaScriptModule)
			{
				pLuaScriptModule->RunAccountDisConnectFunc(pInfo);
			}
		}
		pInfo = m_playerAccountInfo.Next();
	}
}