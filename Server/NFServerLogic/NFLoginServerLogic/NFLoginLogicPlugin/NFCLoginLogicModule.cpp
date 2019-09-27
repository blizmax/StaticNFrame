

#include "NFCLoginLogicModule.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFISqliteModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"
#include "NFMessageDefine/server_to_server_msg.pb.h"
#include "NFMessageDefine/st_human_packet_code.pb.h"
#include "NFMessageDefine/msg_human.pb.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

NFCLoginLogicModule::NFCLoginLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCLoginLogicModule::~NFCLoginLogicModule()
{

}

bool NFCLoginLogicModule::Init()
{
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_LOGIN, NF_ST_PROXY, this, &NFCLoginLogicModule::OnHandleProxyEventCallBack);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_REPORT, this, &NFCLoginLogicModule::OnHandlePlayerReport);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_NOTIFY_LOGIN_HANDLE_PLAYER_DISCONNECT, this, &NFCLoginLogicModule::OnHandlePlayerDisConnect);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_LOGIN, this, &NFCLoginLogicModule::OnHandleAccountLoginFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_DISCONNECT, this, &NFCLoginLogicModule::OnHandlePlayerDisconnectFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_RECONNECT, this, &NFCLoginLogicModule::OnHandlePlayerReconnectFromProxyServer);
	return true;
}

bool NFCLoginLogicModule::AfterInit()
{
	return true;
}

bool NFCLoginLogicModule::Execute()
{
	return true;
}

bool NFCLoginLogicModule::BeforeShut()
{
	return true;
}

bool NFCLoginLogicModule::Shut()
{
	return true;
}

void NFCLoginLogicModule::OnHandleProxyEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		mProxyMap.AddElement(unLinkId, pServerData);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		mProxyMap.RemoveElement(unLinkId);
	}
}

void NFCLoginLogicModule::OnHandlePlayerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyPlayerReport cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		pLuaScriptModule->SessionReport(cgMsg.user_id(), cgMsg.ip());
	}
}

void NFCLoginLogicModule::OnHandlePlayerDisConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyPlayerDisconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		pLuaScriptModule->SessionClose(cgMsg.user_id());
	}
}

void NFCLoginLogicModule::OnHandleAccountLoginFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcaccountlogin gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	auto pInfo = mPlayerProxyInfoMap.GetElement(playerId);
	if (pInfo == nullptr)
	{
		pInfo = NF_SHARE_PTR<PlayerGameServerInfo>(NF_NEW PlayerGameServerInfo());
		mPlayerProxyInfoMap.AddElement(playerId, pInfo);
	}

	pInfo->mPlayerId = playerId;
	auto pProxyServer = mProxyMap.GetElement(unLinkId);
	if (pProxyServer)
	{
		pInfo->mProxyId = pProxyServer->GetServerId();
		pInfo->mProxyUnlinkId = pProxyServer->GetUnlinkId();
	}

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_CONNECTED, NF_ST_LOGIN, unLinkId, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} login login server!", pInfo->mPlayerId);
}

void  NFCLoginLogicModule::OnHandlePlayerDisconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyPlayerDisconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	auto pInfo = mPlayerProxyInfoMap.GetElement(cgMsg.user_id());
	if (pInfo == nullptr)
	{
		return;
	}

	pInfo->mProxyId = 0;
	pInfo->mProxyUnlinkId = 0;

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_DISCONNECTED, NF_ST_LOGIN, 0, pInfo);
	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} disconnect login server!", pInfo->mPlayerId);
}

void NFCLoginLogicModule::OnHandlePlayerReconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcreconnect gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	auto pInfo = mPlayerProxyInfoMap.GetElement(playerId);
	if (pInfo == nullptr)
	{
		return;
	}

	auto pProxyData = mProxyMap.GetElement(unLinkId);
	if (pProxyData)
	{
		pInfo->mProxyId = pProxyData->GetServerId();
		pInfo->mProxyUnlinkId = pProxyData->GetUnlinkId();
	}

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_RECONNECTED, NF_ST_LOGIN, unLinkId, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} reconnect login server!", pInfo->mPlayerId);
}