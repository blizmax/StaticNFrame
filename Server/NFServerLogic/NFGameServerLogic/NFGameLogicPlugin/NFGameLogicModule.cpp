

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
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();

	m_pNetServerModule->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT, this, &NFCGameLogicModule::OnHandleAccountConnect);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT, this, &NFCGameLogicModule::OnHandleAccountDisConnect);
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
		this->m_pNetServerModule->SendByServerID(pInfo->proxyUnlinkId, 0, msg.data(), msg.length(), pInfo->uid);
	});

	NFILuaScriptModule* pLuaScriptModule = pPluginManager->FindModule<NFILuaScriptModule>();
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

		NFILuaScriptModule* pLuaScriptModule = pPluginManager->FindModule<NFILuaScriptModule>();
		if (pLuaScriptModule)
		{
			pLuaScriptModule->RunAccountDisConnectFunc(pInfo);
		}
	}
}