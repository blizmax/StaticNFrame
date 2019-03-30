

#include "NFGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCGameLogicModule::NFCGameLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_onlineNum = 0;
}

NFCGameLogicModule::~NFCGameLogicModule()
{

}

bool NFCGameLogicModule::Init()
{
	m_pGameClient_MasterModule = m_pPluginManager->FindModule<NFIGameClient_MasterModule>();
	m_pServerNetEventModule = m_pPluginManager->FindModule<NFIServerNetEventModule>();

	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	
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
	m_playerAccountInfo.ClearAll();
	return true;
}

void NFCGameLogicModule::OnHandleAccountConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::AccountConnectGameServer_C xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	NF_SHARE_PTR<AccountInfo> pInfo = GetPlayerAccountInfo(xMsg.uid());
	if (pInfo == nullptr)
	{
		pInfo = NF_SHARE_PTR<AccountInfo>(NF_NEW AccountInfo());
		m_playerAccountInfo.AddElement(xMsg.uid(), pInfo);
	}

	pInfo->uid = xMsg.uid();
	pInfo->account = xMsg.account();
	pInfo->ip = xMsg.ip();
	pInfo->unlinkId = unLinkId;

	m_onlineNum++;

	pInfo->SetSendMsg([pInfo, this](const std::string& msg) {
		this->m_pNetClientModule->SendByServerID(pInfo->unlinkId, 0, msg.data(), msg.length(), pInfo->uid);
	});

	m_pServerNetEventModule->OnAccountNetEvent(eAccountEventType_CONNECTED, NF_ST_GAME, unLinkId, pInfo);
}

void NFCGameLogicModule::OnHandleAccountDisConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::AccountConnectGameServer_C xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	NF_SHARE_PTR<AccountInfo> pInfo = GetPlayerAccountInfo(xMsg.uid());
	if (pInfo)
	{
		pInfo->uid = xMsg.uid();
		pInfo->account = xMsg.account();
		pInfo->ip = xMsg.ip();
		pInfo->unlinkId = 0;

		m_onlineNum--;

		m_pServerNetEventModule->OnAccountNetEvent(eAccountEventType_DISCONNECTED, NF_ST_GAME, unLinkId, pInfo);
	}
}

void NFCGameLogicModule::OnHandleAccountReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::AccountConnectGameServer_C xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	NF_SHARE_PTR<AccountInfo> pInfo = GetPlayerAccountInfo(xMsg.uid());
	if (pInfo == nullptr)
	{
		pInfo = NF_SHARE_PTR<AccountInfo>(NF_NEW AccountInfo());
		m_playerAccountInfo.AddElement(xMsg.uid(), pInfo);
	}

	pInfo->uid = xMsg.uid();
	pInfo->account = xMsg.account();
	pInfo->ip = xMsg.ip();
	pInfo->unlinkId = unLinkId;

	m_onlineNum++;

	pInfo->SetSendMsg([pInfo, this](const std::string& msg) {
		this->m_pNetClientModule->SendByServerID(pInfo->unlinkId, 0, msg.data(), msg.length(), pInfo->uid);
	});

	m_pServerNetEventModule->OnAccountNetEvent(eAccountEventType_RECONNECTED, NF_ST_GAME, unLinkId, pInfo);
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
	NFILuaScriptModule* pLuaScriptModule = (NFILuaScriptModule*)m_pPluginManager->FindModule(typeid(NFILuaScriptModule).name());
	NF_SHARE_PTR<AccountInfo> pInfo = m_playerAccountInfo.First();
	while (pInfo)
	{
		if (pInfo->unlinkId == unLinkId)
		{
			pInfo->unlinkId = 0;
			m_pServerNetEventModule->OnAccountNetEvent(eAccountEventType_DISCONNECTED, NF_ST_GAME, unLinkId, pInfo);
		}
		pInfo = m_playerAccountInfo.Next();
	}
}