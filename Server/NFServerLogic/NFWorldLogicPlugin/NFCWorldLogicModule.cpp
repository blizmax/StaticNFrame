

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
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, ::NFMsg::Client_Msg_AccountLogin, this, &NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, ::NFMsg::Server_Msg_AccountLogin, this, &NFCWorldLogicModule::OnHandleAccountLoginFromGameServer);
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
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	uint32_t clientUnlinkId = (uint32_t)(playerId);
	if (cgMsg.cid() == "")
	{
		std::string cid = NFRandomString(8);
		cgMsg.set_cid(cid);

		NFLogInfo(NF_LOG_SYSTEMLOG, 0, "account login, 自动创建随机的CID={}", cid);
	}

	bool quickLogin = false;
	if (cgMsg.account() == "")
	{
		quickLogin = true;
		cgMsg.set_account(cgMsg.cid());
		cgMsg.set_password(cgMsg.cid());
		NFLogInfo(NF_LOG_SYSTEMLOG, 0, "account login, 帐号为空， 快速登录，account={}", cgMsg.cid());
	}

	NF_SHARE_PTR<PlayerWorldServerInfo> pInfo = mPlayerInfoByAccount.GetElement(cgMsg.account());
	if (pInfo == nullptr)
	{
		pInfo = NF_SHARE_PTR<PlayerWorldServerInfo>(NF_NEW PlayerWorldServerInfo());
		pInfo->mAccount = cgMsg.account();

		mPlayerInfoByAccount.AddElement(pInfo->mAccount, pInfo);
	}

	pInfo->mClientUnlinkId = clientUnlinkId;

	NF_SHARE_PTR<NFServerData> pProxyServerData = mProxyMap.GetElement(unLinkId);
	if (pProxyServerData)
	{
		if (pInfo->mProxyServerUnlinkId != 0 && pInfo->mProxyServerUnlinkId != unLinkId)
		{
			NFLogWarning(NF_LOG_SYSTEMLOG, 0, "account login, player:{} switch proxy server!", cgMsg.account());
		}

		pInfo->mProxyServerUnlinkId = unLinkId;
		pInfo->mProxyServerId = pProxyServerData->GetServerId();
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "account login, but proxy disconnect!");
		return;
	}
	
	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "account login:{} world server!", pInfo->mAccount);

	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(pInfo->mGameServerUnlinkId);
	if (pServerData)
	{
		if (pServerData->GetServerId() == pInfo->mGameServerId)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, nMsgId, cgMsg, pInfo->mProxyServerId);
			return;
		}
		else
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "player game server link error! the gameserverid:{} != player game serverId:{}", pServerData->GetServerId(), pInfo->mGameServerId);
			pServerData = FindGameServerByServerId(pInfo->mGameServerId);
			if (pServerData)
			{
				pInfo->mGameServerUnlinkId = pServerData->GetUnlinkId();
				FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, nMsgId, cgMsg, pInfo->mProxyServerId);
				return;
			}
		}
	}
	else
	{
		pServerData = FindGameServerByServerId(pInfo->mGameServerId);
		if (pServerData)
		{
			pInfo->mGameServerUnlinkId = pServerData->GetUnlinkId();
			FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, nMsgId, cgMsg, pInfo->mProxyServerId);
			return;
		}
		else
		{
			pServerData = mGameMap.First();
			if (pServerData)
			{
				pInfo->mGameServerId = pServerData->GetServerId();
				pInfo->mGameServerUnlinkId = pServerData->GetUnlinkId();
				FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, nMsgId, cgMsg, pInfo->mProxyServerId);
				return;
			}
		}
	}
}

void  NFCWorldLogicModule::OnHandlePlayerDisconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyWorldPlayerDisconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NF_SHARE_PTR<PlayerWorldServerInfo> pInfo = mPlayerInfoByAccount.GetElement(cgMsg.account());
	if (pInfo == nullptr)
	{
		pInfo = mPlayerInfoByPlayerId.GetElement(cgMsg.user_id());
		if (pInfo == nullptr)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "player disconect, but can't find the account info, account:{}, userid:{}", cgMsg.account(), cgMsg.user_id());
			return;
		}
	}

	pInfo->mClientUnlinkId = 0;
	pInfo->mIPAddr = "";
	pInfo->mProxyServerId = 0;
	pInfo->mProxyServerUnlinkId = 0;

	if (pInfo->mGameServerUnlinkId > 0)
	{
		NFMsg::NotifyGamePlayerDisconnect gcMsg;
		gcMsg.set_user_id(pInfo->mPlayerId);
		gcMsg.set_account(pInfo->mAccount);

		FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_DISCONNECT, gcMsg, pInfo->mPlayerId);
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
		pInfo->mProxyServerUnlinkId = unLinkId;
		pInfo->mProxyServerId = pProxyServerData->GetServerId();
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "player:{} reconnect, but proxy disconnect!", cgMsg.userid());
		return;
	}

	if (pInfo->mGameServerUnlinkId > 0)
	{
		NFMsg::NotifyGamePlayerReconnect gcMsg;
		gcMsg.set_user_id(cgMsg.userid());
		gcMsg.set_proxy_id(pInfo->mProxyServerId);
		FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_RECONNECT, gcMsg, pInfo->mPlayerId);

		FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mGameServerUnlinkId, nMsgId, cgMsg, pInfo->mPlayerId);

		NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} reconnect world server!", pInfo->mPlayerId);
	}
}

void NFCWorldLogicModule::OnHandleAccountLoginFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcaccountlogin gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	std::string account = gcMsg.mutable_pinfo()->account();
	NF_SHARE_PTR<PlayerWorldServerInfo> pLinkInfo = mPlayerInfoByAccount.GetElement(account);
	if (pLinkInfo == nullptr)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "account login return, but can't find the account info:{}", account);
		return;
	}

	if (gcMsg.result() == 0)
	{
		pLinkInfo->mIsLogin = true;
		pLinkInfo->mPlayerId = gcMsg.mutable_pinfo()->userid();

		NF_SHARE_PTR<PlayerWorldServerInfo> pPlayerInfo = mPlayerInfoByPlayerId.GetElement(pLinkInfo->mPlayerId);
		if (pPlayerInfo)
		{
			if (pPlayerInfo->mAccount != pLinkInfo->mAccount)
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "account:{} and account:{} has a same playerid:{}, some wrong error", pPlayerInfo->mAccount, pLinkInfo->mAccount);
			}
			//处理以前的网关, 和游戏服务器, 这里本事就出现错误了

			mPlayerInfoByPlayerId.RemoveElement(pLinkInfo->mPlayerId);
			pPlayerInfo = nullptr;
		}
		mPlayerInfoByPlayerId.AddElement(pLinkInfo->mPlayerId, pLinkInfo);

		NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(pLinkInfo->mProxyServerUnlinkId);
		if (pServerData == nullptr)
		{
			NFLogWarning(NF_LOG_SYSTEMLOG, 0, "account login return, proxy disconnet! account:{}", account);
			return;
		}

		NFMsg::NotifyGameChangeProxy msgChangeProxy;
		msgChangeProxy.set_user_id(pLinkInfo->mPlayerId);
		msgChangeProxy.set_proxy_id(pLinkInfo->mProxyServerId);
		FindModule<NFINetServerModule>()->SendToServerByPB(unLinkId, EGMI_NET_WORLD_NOTIFY_GAME_CHANGE_PROXY, msgChangeProxy, pLinkInfo->mPlayerId);

		NFMsg::NotifyProxyChangeGame msgChangeGame;
		msgChangeGame.set_user_id(pLinkInfo->mPlayerId);
		msgChangeGame.set_game_id(pLinkInfo->mGameServerId);
		msgChangeGame.set_client_link_id(pLinkInfo->mClientUnlinkId);
		FindModule<NFINetServerModule>()->SendToServerByPB(pLinkInfo->mProxyServerUnlinkId, EGMI_NET_WORLD_NOTIFY_PROXY_CHANGE_GAME, msgChangeGame, pLinkInfo->mClientUnlinkId);

		FindModule<NFINetServerModule>()->SendToServerByPB(pLinkInfo->mProxyServerUnlinkId, nMsgId, gcMsg, pLinkInfo->mClientUnlinkId);
	}
	else
	{
		pLinkInfo->mIsLogin = false;
		pLinkInfo->mPlayerId = gcMsg.mutable_pinfo()->userid();

		if (pLinkInfo->mPlayerId > 0)
		{
			mPlayerInfoByPlayerId.RemoveElement(pLinkInfo->mPlayerId);
		}
		mPlayerInfoByAccount.RemoveElement(account);

		FindModule<NFINetServerModule>()->SendToServerByPB(pLinkInfo->mProxyServerUnlinkId, nMsgId, gcMsg, pLinkInfo->mClientUnlinkId);
	}
}

void NFCWorldLogicModule::OnHandlePlayerReconnectFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcreconnect gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	NF_SHARE_PTR<PlayerWorldServerInfo> pPlayerInfo = mPlayerInfoByPlayerId.GetElement(playerId);
	if (pPlayerInfo)
	{
		FindModule<NFINetServerModule>()->SendToServerByPB(pPlayerInfo->mProxyServerUnlinkId, nMsgId, gcMsg, playerId);
	}
}