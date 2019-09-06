

#include "NFCWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFCore/NFMD5.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"
#include "NFMessageDefine/st_human_packet_code.pb.h"
#include "NFMessageDefine/msg_human.pb.h"
#include "NFMessageDefine/server_to_server_msg.pb.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFCore/NFTime.h"

enum BIND_TYPE_ENUM
{
	BindType_None = 0,     //未绑定，这个是游客登录的
	BindType_TianYu = 1,   //自己
	BindType_QQ = 2,      //QQ登入
	BindType_WeiXin = 3,   //微信登入
	BindType_phone = 4,   //手机号登入
};

NFCWorldLogicModule::NFCWorldLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCWorldLogicModule::~NFCWorldLogicModule()
{

}

bool NFCWorldLogicModule::Init()
{
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, this, &NFCWorldLogicModule::OnHandleMessageFromServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, ::EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN, this, &NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, ::EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_DISCONNECT, this, &NFCWorldLogicModule::OnHandlePlayerDisconnectFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, NFMsg::Client_Msg_ReConnect, this, &NFCWorldLogicModule::OnHandlePlayerReconnectFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, NFMsg::Server_Msg_ReConnect, this, &NFCWorldLogicModule::OnHandlePlayerReconnectFromGameServer);

	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_WORLD, NF_ST_PROXY, this, &NFCWorldLogicModule::OnHandleProxyEventCallBack);
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_WORLD, NF_ST_GAME, this, &NFCWorldLogicModule::OnHandleGameEventCallBack);

	return true;
}

void NFCWorldLogicModule::OnHandleGameEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
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

void NFCWorldLogicModule::OnHandleProxyEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
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

bool NFCWorldLogicModule::AfterInit()
{
	return true;
}

bool NFCWorldLogicModule::Execute()
{
	return true;
}

bool NFCWorldLogicModule::BeforeShut()
{
	return true;
}

bool NFCWorldLogicModule::Shut()
{
	mGameMap.ClearAll();
	mProxyMap.ClearAll();
	return true;
}

NF_SHARE_PTR<NFServerData> NFCWorldLogicModule::FindGameServerByServerId(uint32_t serverId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		if (pServerData->GetServerId() == serverId)
		{
			return pServerData;
		}
		pServerData = mGameMap.Next();
	}

	return nullptr;
}

NF_SHARE_PTR<NFServerData> NFCWorldLogicModule::FindProxyServerByServerId(uint32_t serverId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (pServerData->GetServerId() == serverId)
		{
			return pServerData;
		}
		pServerData = mProxyMap.Next();
	}

	return nullptr;
}

void NFCWorldLogicModule::OnHandleMessageFromServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(unLinkId);
	if (pServerData)
	{
		NFLogWarning(NF_LOG_WORLD_RECV_MSG_LOG, 0, "recv msg from server -- serverType:{}, serverId:{}, playerId:{}, msgId:{}, msglen:{}, not handled!", GetServerName((NF_SERVER_TYPES)pServerData->GetServerType()), pServerData->GetServerId(), playerId, nMsgId, nLen);
		return;
	}

	pServerData = mGameMap.GetElement(unLinkId);
	if (pServerData)
	{
		NFLogWarning(NF_LOG_WORLD_RECV_MSG_LOG, 0, "recv msg from server -- serverType:{}, serverId:{}, playerId:{}, msgId:{}, msglen:{}, not handled!", GetServerName((NF_SERVER_TYPES)pServerData->GetServerType()), pServerData->GetServerId(), playerId, nMsgId, nLen);
		return;
	}
}

void NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcaccountlogin gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	auto pProxyServer = mProxyMap.GetElement(unLinkId);
	if (pProxyServer == nullptr)
	{
		NFLogWarning(NF_LOG_SYSTEMLOG, 0, "account login worldserver, but proxy disconnect! account:{}, playerid:{}", gcMsg.pinfo().account(), gcMsg.pinfo().userid());
		return;
	}

	auto pPlayerInfo = mPlayerInfoByPlayerId.GetElement(gcMsg.pinfo().userid());
	if (pPlayerInfo == nullptr)
	{
		pPlayerInfo = NF_SHARE_PTR<PlayerWorldServerInfo>(NF_NEW PlayerWorldServerInfo());
		pPlayerInfo->mAccount = gcMsg.pinfo().account();
		pPlayerInfo->mPlayerId = gcMsg.pinfo().userid();
		pPlayerInfo->mProxyServerId = pProxyServer->GetServerId();

		mPlayerInfoByPlayerId.AddElement(gcMsg.pinfo().userid(), pPlayerInfo);
	}
	else
	{
		pPlayerInfo->mAccount = gcMsg.pinfo().account();
		pPlayerInfo->mPlayerId = gcMsg.pinfo().userid();
		pPlayerInfo->mProxyServerId = pProxyServer->GetServerId();
	}

	auto pGameServer = FindGameServerByServerId(pPlayerInfo->mGameServerId);
	if (pGameServer)
	{
		pPlayerInfo->mGameServerId = pGameServer->GetServerId();
		FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->GetUnlinkId(), EGMI_NET_WORLD_NOTIFY_GAME_PLAYER_LOGIN, gcMsg, pPlayerInfo->mProxyServerId);
	}
	else
	{
		pGameServer = mGameMap.First();
		if (pGameServer)
		{
			pPlayerInfo->mGameServerId = pGameServer->GetServerId();
			FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->GetUnlinkId(), EGMI_NET_WORLD_NOTIFY_GAME_PLAYER_LOGIN, gcMsg, pPlayerInfo->mProxyServerId);
		}
		else
		{
			NFLogWarning(NF_LOG_SYSTEMLOG, 0, "account login worldserver, but no game server! account:{}, playerid:{}", gcMsg.pinfo().account(), gcMsg.pinfo().userid());
			return;
		}
	}
}

void  NFCWorldLogicModule::OnHandlePlayerDisconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyWorldPlayerDisconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NF_SHARE_PTR<PlayerWorldServerInfo> pInfo = mPlayerInfoByPlayerId.GetElement(cgMsg.user_id());
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "player disconect, but can't find the account info, account:{}, userid:{}", cgMsg.account(), cgMsg.user_id());
		return;
	}

	pInfo->mIPAddr = "";
	pInfo->mProxyServerId = 0;

	auto pGameServer = FindGameServerByServerId(pInfo->mGameServerId);
	if (pGameServer)
	{
		NFMsg::NotifyGamePlayerDisconnect gcMsg;
		gcMsg.set_user_id(pInfo->mPlayerId);
		gcMsg.set_account(pInfo->mAccount);

		FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->GetUnlinkId(), EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_DISCONNECT, gcMsg, pInfo->mPlayerId);
	}
}

void NFCWorldLogicModule::OnHandlePlayerReconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgreconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NF_SHARE_PTR<PlayerWorldServerInfo> pInfo = mPlayerInfoByPlayerId.GetElement(cgMsg.userid());
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "player reconnect, but can't find the player info, userid:{}", cgMsg.userid());
		return;
	}

	NF_SHARE_PTR<NFServerData> pProxyServerData = mProxyMap.GetElement(unLinkId);
	if (pProxyServerData)
	{
		pInfo->mProxyServerId = pProxyServerData->GetServerId();
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "player:{} reconnect, but proxy disconnect!", cgMsg.userid());
		return;
	}

	if (pInfo->mGameServerId > 0)
	{
		auto pGameServer = FindGameServerByServerId(pInfo->mGameServerId);
		if (pGameServer)
		{
			NFMsg::NotifyGamePlayerReconnect gcMsg;
			gcMsg.set_user_id(cgMsg.userid());
			gcMsg.set_proxy_id(pInfo->mProxyServerId);
			FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->GetUnlinkId(), EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_RECONNECT, gcMsg, pInfo->mPlayerId);

			FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->GetUnlinkId(), nMsgId, cgMsg, pInfo->mPlayerId);
		}

		NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} reconnect world server!", pInfo->mPlayerId);
	}
}

void NFCWorldLogicModule::OnHandlePlayerReconnectFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcreconnect gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	NF_SHARE_PTR<PlayerWorldServerInfo> pPlayerInfo = mPlayerInfoByPlayerId.GetElement(playerId);
	if (pPlayerInfo)
	{
		auto pProxyServer = FindProxyServerByServerId(pPlayerInfo->mProxyServerId);
		if (pProxyServer)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pProxyServer->GetUnlinkId(), nMsgId, gcMsg, playerId);
		}
	}
}