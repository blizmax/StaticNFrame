

#include "NFCProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFJson.h"

NFCProxyLogicModule::NFCProxyLogicModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCProxyLogicModule::~NFCProxyLogicModule()
{

}

bool NFCProxyLogicModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetProxyServerModule = pPluginManager->FindModule<NFIProxyServerModule>();

	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, EGMI_NET_MSG_JSON_MSG, this, &NFCProxyLogicModule::OnHandleJsonMessage);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY_INNER, EGMI_NET_MSG_JSON_MSG, this, &NFCProxyLogicModule::OnHandleGameJsonMessage);
	return true;
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

void NFCProxyLogicModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{

}

void NFCProxyLogicModule::OnHandleGameJsonMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	ProxyPlayerData* pData = GetPlayerData(playerId);
	if (pData && pData->unlinkId > 0)
	{
		std::string strJson = "{\"cmd_name\":\"Pmd.UserJsMessageForwardUserPmd_CS\", \"msg\":" + std::string(msg, nLen) + "}";
		m_pNetServerModule->SendByServerID(pData->unlinkId, 0, strJson,0);
		//NFLogDebug("send msg:{}", strJson);
	}
}

void NFCProxyLogicModule::OnHandleJsonMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ProxyCmdMessage cmdMessage;
	std::string jsonMsg = std::string(msg, nLen);
	if (NFServerCommon::JsonStringToMessage(jsonMsg, cmdMessage) == false)
	{
		NFLogError("error msg:{}", jsonMsg);
		return;
	}

	//NFLogDebug("recv msg:{}", jsonMsg);

	if (cmdMessage.cmd_name() == "Pmd.UserLoginTokenLoginUserPmd_C")
	{
		NFMsg::UserLoginTokenLoginUserPmd_C msg;
		if (NFServerCommon::JsonStringToMessage(jsonMsg, msg) == false)
		{
			NFLogError("json to NFMsg::UserLoginTokenLoginUserPmd_C failed!");
			return;
		}
		OnHandleUser_LoginTokenLoginUserPmd(unLinkId, msg);
	}
	else if (cmdMessage.cmd_name() == "Pmd.UserJsMessageForwardUserPmd_CS")
	{
		NFMsg::UserJsMessageForwardUserPmd_CS msg;
		if (NFServerCommon::JsonStringToMessage(jsonMsg, msg) == false)
		{
			NFLogError("json to NFMsg::UserJsMessageForwardUserPmd_CS failed!");
			return;
		}
		OnHandleUser_UserJsMessageForwardUserPmd(unLinkId, msg);
	}
	else if (cmdMessage.cmd_name() == "Pmd.UserLoginReconnectLoginUserPmd_C")
	{
		NFMsg::UserLoginReconnectLoginUserPmd_C msg;
		if (NFServerCommon::JsonStringToMessage(jsonMsg, msg) == false)
		{
			NFLogError("json to NFMsg::UserLoginReconnectLoginUserPmd_C failed!");
			return;
		}
		OnHandleUser_UserLoginReconnectLoginUserPmd(unLinkId, msg);
	}
	else
	{
		NFLogError("msg:{} not handled!", cmdMessage.cmd_name());
		NFLogError("{}", jsonMsg);
	}
}

void NFCProxyLogicModule::OnHandleUser_UserJsMessageForwardUserPmd(const uint32_t unLinkId, const NFMsg::UserJsMessageForwardUserPmd_CS& msg)
{
	ProxyPlayerData* pData = GetPlayerDataByLinkId(unLinkId);
	if (pData == nullptr)
	{
		NFLogError("Can't find player data, unLinkId:{}", unLinkId);
		m_pNetServerModule->CloseLinkId(unLinkId);
		return;
	}

	auto pServerData = m_pNetProxyServerModule->GetGameServerData(pData->gameServerId);
	if (pServerData)
	{
		m_pNetServerModule->SendByServerID(pServerData->mUnlinkId, 0, msg.msg(), pData->uid);
	}
}

void NFCProxyLogicModule::OnHandleUser_LoginTokenLoginUserPmd(const uint32_t unLinkId, const NFMsg::UserLoginTokenLoginUserPmd_C& msg)
{
	ProxyPlayerData* pData = GetPlayerData(msg.accountid());
	if (pData == nullptr)
	{
		pData = NF_NEW ProxyPlayerData();
		mPlayerData.AddElement(msg.accountid(), pData);
	}

	pData->uid = msg.accountid();
	pData->gameServerId = msg.zoneid();

	ProxyPlayerData* pLinkData = GetPlayerDataByLinkId(unLinkId);
	if (pLinkData == nullptr || pLinkData != pData)
	{
		pLinkData = pData;
		mUnlinkIdPlayerData.AddElement(unLinkId, pLinkData);
	}

	pData->unlinkId = unLinkId;

	auto pServerData = m_pNetProxyServerModule->GetGameServerData(pData->gameServerId);
	if (pServerData == nullptr)
	{
		NFLogError("Game Server:{} Not Existed! Maybe dump!", pData->gameServerId);
		pData->unlinkId = 0;
		m_pNetServerModule->CloseLinkId(unLinkId);
		return;
	}

	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFMsg::AccountConnectGameServer_C account;
	account.set_uid(pData->uid);
	account.set_ip(ip);
	
	m_pNetServerModule->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT, account, pData->uid);

	std::string strJson = "{\"accountid\":" + lexical_cast<std::string>(pData->uid) + ",\"cmd_name\":\"Pmd.UserLoginReturnOkLoginUserPmd_S\"}";
	m_pNetServerModule->SendByServerID(unLinkId, 0, strJson, 0);
}

void NFCProxyLogicModule::OnHandleUser_UserLoginReconnectLoginUserPmd(const uint32_t unLinkId, const NFMsg::UserLoginReconnectLoginUserPmd_C& msg)
{
	ProxyPlayerData* pData = GetPlayerData(msg.accountid());
	if (pData == nullptr)
	{
		pData = NF_NEW ProxyPlayerData();
		mPlayerData.AddElement(msg.accountid(), pData);
	}

	pData->uid = msg.accountid();
	pData->gameServerId = msg.zoneid();

	ProxyPlayerData* pLinkData = GetPlayerDataByLinkId(unLinkId);
	if (pLinkData == nullptr || pLinkData != pData)
	{
		pLinkData = pData;
		mUnlinkIdPlayerData.AddElement(unLinkId, pLinkData);
	}

	pData->unlinkId = unLinkId;

	auto pServerData = m_pNetProxyServerModule->GetGameServerData(pData->gameServerId);
	if (pServerData == nullptr)
	{
		NFLogError("Game Server:{} Not Existed! Maybe dump!", pData->gameServerId);
		pData->unlinkId = 0;
		m_pNetServerModule->CloseLinkId(unLinkId);
		return;
	}

	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
	NFMsg::AccountConnectGameServer_C account;
	account.set_uid(pData->uid);
	account.set_ip(ip);

	m_pNetServerModule->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT, account, pData->uid);
}

void NFCProxyLogicModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		OnAccountDisconnect(unLinkId);
	}
}

void NFCProxyLogicModule::OnAccountDisconnect(const uint32_t unLinkId)
{
	ProxyPlayerData* pData = GetPlayerDataByLinkId(unLinkId);
	if (pData)
	{
		pData->unlinkId = 0;

		mUnlinkIdPlayerData.RemoveElement(unLinkId);

		auto pServerData = m_pNetProxyServerModule->GetGameServerData(pData->gameServerId);
		if (pServerData == nullptr)
		{
			return;
		}

		std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
		NFMsg::AccountConnectGameServer_C account;
		account.set_uid(pData->uid);

		m_pNetServerModule->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT, account, pData->uid);
	}
}