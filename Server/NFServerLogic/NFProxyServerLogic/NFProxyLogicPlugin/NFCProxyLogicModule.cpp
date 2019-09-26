

#include "NFCProxyLogicModule.h"
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
#include "NFComm/NFCore/NFStringUtility.h"

NFCProxyLogicModule::NFCProxyLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCProxyLogicModule::~NFCProxyLogicModule()
{

}

bool NFCProxyLogicModule::Init()
{
	/*
	** 处理来自客户端的连接和消息
	*/
	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnProxySocketEvent);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnHandleMessageFromClient);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_PROXY, ::NFMsg::Client_Msg_AccountLogin, this, &NFCProxyLogicModule::OnHandleAccountLoginFromClient);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_PROXY, ::NFMsg::Client_Msg_HeartBeat, this, &NFCProxyLogicModule::OnHandleHeartBeat);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_PROXY, ::NFMsg::Client_Msg_ReConnect, this, &NFCProxyLogicModule::OnHandleReconnectFromClient);
	/*
	** 处理来自客户端的连接和消息
	*/
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleMessageFromGameServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleMessageFromWorldServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, this, &NFCProxyLogicModule::OnHandleMessageFromLoginServer);

	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, ::NFMsg::Server_Msg_AccountLogin, this, &NFCProxyLogicModule::OnHandleAccountLoginFromLoginServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_WORLD_NOTIFY_PROXY_CHANGE_GAME, this, &NFCProxyLogicModule::OnHandleNotifyChangeGameFromWorldServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, NFMsg::Server_Msg_ReConnect, this, &NFCProxyLogicModule::OnHandleReconnectFromWorldServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_GAME_NOTIFY_PROXY_PLAYER_LOGIN, this, &NFCProxyLogicModule::OnHandleAccountLoginFromGameServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_GAME_SEND_PACKET_TO_PROXY, this, &NFCProxyLogicModule::OnHandlePacketMsgFromGameServer);
	
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_PROXY, NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleGameEventCallBack);
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_PROXY, NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleWorldEventCallBack);
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_PROXY, NF_ST_LOGIN, this, &NFCProxyLogicModule::OnHandleLoginEventCallBack);

	//10秒钟检查一次心跳包
	this->SetTimer(0, 1000, INFINITY_CALL);
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

void NFCProxyLogicModule::OnHandleLoginEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		mLoginMap.AddElement(unLinkId, pServerData);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		mLoginMap.RemoveElement(unLinkId);
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

NF_SHARE_PTR<NFServerData> NFCProxyLogicModule::GetGameServerByServerId(uint32_t serverId)
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
	mLoginMap.ClearAll();
	mGameMap.ClearAll();
	mWorldMap.ClearAll();
	return true;
}

void NFCProxyLogicModule::OnTimer(uint32_t nTimerID)
{
	auto pLinkInfo = mClientLinkInfo.First();
	while (pLinkInfo)
	{
		if (pLinkInfo->mRecvHeartBeatTime > 0)
		{
			if (NFGetSecondTime() - pLinkInfo->mRecvHeartBeatTime > 10)
			{
				NFMsg::gcheartbeat gcMsg;
				gcMsg.set_result(0);
				FindModule<NFINetServerModule>()->SendToServerByPB(pLinkInfo->mUnlinkId, NFMsg::Server_Msg_HeartBeat, gcMsg, 0);
			}
		}
		pLinkInfo = mClientLinkInfo.Next();
	}
}

void NFCProxyLogicModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
		if (pLinkInfo == nullptr)
		{
			//NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "client disconnect, but can not find linkId, unLinkId:{}, player maybe not login!", unLinkId);
			return;
		}

		if (pLinkInfo->mPlayerId > 0)
		{
			auto pPlayerInfo = mPlayerLinkInfo.GetElement(pLinkInfo->mPlayerId);
			if (pPlayerInfo)
			{
				pPlayerInfo->mUnlinkId = 0;
				NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
				if (pServerData)
				{
					NFMsg::NotifyWorldPlayerDisconnect msg;
					msg.set_user_id(pLinkInfo->mPlayerId);
					msg.set_account(pLinkInfo->mAccount);
					FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_DISCONNECT, msg, pLinkInfo->mPlayerId);
				}
			}
		}

		mClientLinkInfo.RemoveElement(unLinkId);
	}
}

void NFCProxyLogicModule::OnHandleMessageFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
	if (pLinkInfo == nullptr || pLinkInfo->mIsLogin == false)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} not login,send msg:{}, the msg will not send to gameserver", ip, nMsgId);
		return;
	}

	NF_SHARE_PTR<NFServerData> pServerData = GetGameServerByServerId(pLinkInfo->mGameServerId);
	if (pServerData)
	{
		FindModule<NFINetClientModule>()->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mPlayerId);
	}
	else
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} ,send msg:{}, can not find game server link, some thing wrong", ip, nMsgId);
		return;
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);
	NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.First();
	if (pServerData)
	{
		NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
		if (pLinkInfo == nullptr)
		{
			pLinkInfo = NF_SHARE_PTR<ProxyLinkInfo>(NF_NEW ProxyLinkInfo());
			pLinkInfo->mUnlinkId = unLinkId;
			pLinkInfo->mIsLogin = false;
			pLinkInfo->mIPAddr = ip;
			pLinkInfo->mAccount = cgMsg.account();
			mClientLinkInfo.AddElement(unLinkId, pLinkInfo);
		}
		else
		{
			if (pLinkInfo->mIsLogin && pLinkInfo->mPlayerId > 0)
			{
				NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "IpAddr:{} playerId:{}, account:{} 同一个连接重复登入!", pLinkInfo->mIPAddr, pLinkInfo->mPlayerId, pLinkInfo->mAccount);
			}
		}

		pLinkInfo->mRecvMsgCount++;
		FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgId, cgMsg, unLinkId);
	}
}

void NFCProxyLogicModule::OnHandleReconnectFromWorldServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcreconnect gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	NF_SHARE_PTR<ProxyLinkInfo> pPlayerLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pPlayerLinkInfo)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "Player:{} reocnnect success!", playerId);
		pPlayerLinkInfo->mIsLogin = true;
		NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(pPlayerLinkInfo->mUnlinkId);
		if (pLinkInfo)
		{
			pLinkInfo->mIsLogin = true;
			FindModule<NFINetServerModule>()->SendToServerByPB(pLinkInfo->mUnlinkId, NFMsg::Server_Msg_ReConnect, gcMsg, playerId);
		}
	}
}

void NFCProxyLogicModule::OnHandleReconnectFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgreconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pPlayerLinkInfo = mPlayerLinkInfo.GetElement(cgMsg.userid());
	if (pPlayerLinkInfo == nullptr)
	{
		NFMsg::gcreconnect gcMsg;
		gcMsg.set_result(-1);
		FindModule<NFINetServerModule>()->SendToServerByPB(unLinkId, NFMsg::Server_Msg_ReConnect, gcMsg, cgMsg.userid());
		return;
	}

	if (pPlayerLinkInfo->mUnlinkId > 0)
	{
		if (pPlayerLinkInfo->mUnlinkId == unLinkId)
		{
			NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "Player:{} reconnect proxy server, but unlink not change! some wrong!", cgMsg.userid());
		}
		else
		{
			if (mClientLinkInfo.GetElement(pPlayerLinkInfo->mUnlinkId))
			{
				//NFMsg::gckitplayer kitMsg;
				//kitMsg.set_result(0);
				//kitMsg.set_kittype(1);
				//kitMsg.set_msg("您的账号已在别处登入，若非本人操作，请联系系统管理员！");
				//FindModule<NFINetServerModule>()->SendToServerByPB(pPlayerLinkInfo->mUnlinkId, NFMsg::Server_Msg_KitPlayer, kitMsg, 0);
				FindModule<NFINetServerModule>()->CloseLinkId(pPlayerLinkInfo->mUnlinkId);
			}

			mClientLinkInfo.RemoveElement(pPlayerLinkInfo->mUnlinkId);
		}
	}

	pPlayerLinkInfo->mUnlinkId = unLinkId;
	pPlayerLinkInfo->mIsLogin = false;
	pPlayerLinkInfo->mIPAddr = ip;
	pPlayerLinkInfo->mPlayerId = cgMsg.userid();

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = NF_SHARE_PTR<ProxyLinkInfo>(NF_NEW ProxyLinkInfo());
	if (pLinkInfo)
	{
		pLinkInfo->mUnlinkId = unLinkId;
		pLinkInfo->mIsLogin = false;
		pLinkInfo->mIPAddr = ip;
		pLinkInfo->mPlayerId = pPlayerLinkInfo->mPlayerId;
		pLinkInfo->mAccount = pPlayerLinkInfo->mAccount;
		pLinkInfo->mGameServerId = pPlayerLinkInfo->mGameServerId;
	}
	mClientLinkInfo.AddElement(unLinkId, pLinkInfo);

	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
	if (pServerData)
	{
		NFLogInfo(NF_LOG_PROXY_LOGIC_PLUGIN, cgMsg.userid(), "Player:{} reconnect proxy server!", cgMsg.userid());
		FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgId, cgMsg, cgMsg.userid());
	}
}

void NFCProxyLogicModule::OnHandleNotifyChangeGameFromWorldServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyProxyChangeGame gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(gcMsg.client_link_id());
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "clientLinkId:{} not exist, client maybe disconnect!", gcMsg.client_link_id());
		return;
	}

	do 
	{
		NF_SHARE_PTR<ProxyLinkInfo> pPlayerInfo = mPlayerLinkInfo.GetElement(gcMsg.user_id());
		if (pPlayerInfo)
		{
			if (pPlayerInfo != pLinkInfo)
			{
				if (pPlayerInfo->mUnlinkId != 0 && pPlayerInfo->mUnlinkId != pLinkInfo->mUnlinkId)
				{
					NFMsg::gckitplayer kitMsg;
					kitMsg.set_result(0);
					kitMsg.set_kittype(1);
					kitMsg.set_msg("您的账号已在别处登入，若非本人操作，请联系系统管理员！");
					FindModule<NFINetServerModule>()->SendToServerByPB(pPlayerInfo->mUnlinkId, NFMsg::Server_Msg_KitPlayer, kitMsg, 0);
					FindModule<NFINetServerModule>()->CloseLinkId(pPlayerInfo->mUnlinkId);
				}

				mPlayerLinkInfo.RemoveElement(gcMsg.user_id());
				pPlayerInfo = nullptr;

				mPlayerLinkInfo.AddElement(gcMsg.user_id(), pLinkInfo);
			}
		}
		else
		{
			mPlayerLinkInfo.AddElement(gcMsg.user_id(), pLinkInfo);
		}
	} while (false);

	pLinkInfo->mPlayerId = gcMsg.user_id();
	pLinkInfo->mGameServerId = gcMsg.game_id();
	NF_SHARE_PTR<NFServerData> pGameServer = GetGameServerByServerId(pLinkInfo->mGameServerId);
	if (pGameServer)
	{

	}
	else
	{
		NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "notify change game, but proxy disconnect the game server id:{}", pLinkInfo->mGameServerId);
	}
}

void NFCProxyLogicModule::OnHandlePacketMsgFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyProxyPacketMsg gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	for (int i = 0; i < gcMsg.user_id_size(); i++)
	{
		uint64_t playerId = gcMsg.user_id(i);
		auto pPlayerInfo = mPlayerLinkInfo.GetElement(playerId);
		if (pPlayerInfo)
		{
			NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(pPlayerInfo->mUnlinkId);
			if (pLinkInfo == nullptr)
			{
				return;
			}

			FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, gcMsg.msg_id(), gcMsg.msg().data(), gcMsg.msg().length(), pLinkInfo->mSendMsgCount);
		}
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcaccountlogin gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	uint32_t clientLinkId = (uint32_t)playerId;
	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(clientLinkId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "clientLinkId:{} not exist, client maybe disconnect!", clientLinkId);
		return;
	}

	if (gcMsg.result() == 0)
	{
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "player login success -- playerid:{} account:{} ip:{}", gcMsg.mutable_pinfo()->userid(), gcMsg.mutable_pinfo()->account(), pLinkInfo->mIPAddr);

		auto pPlayerInfo = mPlayerLinkInfo.GetElement(gcMsg.mutable_pinfo()->userid());
		if (pPlayerInfo == nullptr)
		{
			pPlayerInfo = NF_SHARE_PTR<ProxyLinkInfo>(NF_NEW ProxyLinkInfo());

			pPlayerInfo->mUnlinkId = clientLinkId;
			pPlayerInfo->mIPAddr = pLinkInfo->mIPAddr;
			pPlayerInfo->mIsLogin = true;
			pPlayerInfo->mAccount = gcMsg.mutable_pinfo()->account();
			pPlayerInfo->mPlayerId = gcMsg.mutable_pinfo()->userid();

			mPlayerLinkInfo.AddElement(pPlayerInfo->mPlayerId, pPlayerInfo);
		}
		else
		{
			if (pPlayerInfo->mUnlinkId > 0 && pPlayerInfo->mUnlinkId != clientLinkId)
			{
				if (mClientLinkInfo.GetElement(pPlayerInfo->mUnlinkId))
				{
					FindModule<NFINetServerModule>()->CloseLinkId(pPlayerInfo->mUnlinkId);
				}
				mClientLinkInfo.RemoveElement(pPlayerInfo->mUnlinkId);
			}
			else if (pPlayerInfo->mUnlinkId > 0 && pPlayerInfo->mUnlinkId == clientLinkId)
			{
				NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "account repeat login, account:{}, playerId:{}", pLinkInfo->mAccount, pLinkInfo->mPlayerId);
			}
			pPlayerInfo->mUnlinkId = clientLinkId;
			pPlayerInfo->mIPAddr = pLinkInfo->mIPAddr;
			pPlayerInfo->mIsLogin = true;
			pPlayerInfo->mAccount = gcMsg.mutable_pinfo()->account();
			pPlayerInfo->mPlayerId = gcMsg.mutable_pinfo()->userid();
		}

		pLinkInfo->mAccount = gcMsg.mutable_pinfo()->account();
		pLinkInfo->mPlayerId = gcMsg.mutable_pinfo()->userid();
		pLinkInfo->mIsLogin = true;
		pLinkInfo->mSendMsgCount++;

		NF_SHARE_PTR<NFServerData> pWorldServer = mWorldMap.First();
		if (pWorldServer)
		{
			pLinkInfo->mRecvMsgCount++;
			FindModule<NFINetClientModule>()->SendToServerByPB(pWorldServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN, gcMsg, pLinkInfo->mPlayerId);
		}
	}
	else
	{
		pLinkInfo->mIsLogin = false;

		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);

		FindModule<NFINetServerModule>()->CloseLinkId(clientLinkId);
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcaccountlogin gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	auto pPlayerInfo = mPlayerLinkInfo.GetElement(gcMsg.mutable_pinfo()->userid());
	if (pPlayerInfo)
	{
		NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(pPlayerInfo->mUnlinkId);
		if (pLinkInfo == nullptr)
		{
			NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "clientLinkId:{} not exist, client maybe disconnect!", pPlayerInfo->mUnlinkId);
			return;
		}

		auto pGameServer = mGameMap.GetElement(unLinkId);
		if (pGameServer)
		{
			pPlayerInfo->mGameServerId = pGameServer->GetServerId();
			pLinkInfo->mGameServerId = pGameServer->GetServerId();

			NFMsg::NotifyGamePlayerReport reportMsg;
			reportMsg.set_user_id(pPlayerInfo->mPlayerId);
			reportMsg.set_ip(pPlayerInfo->mIPAddr);

			FindModule<NFINetClientModule>()->SendToServerByPB(pGameServer->GetUnlinkId(), ::EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_REPORT, reportMsg, pPlayerInfo->mPlayerId);
		}

		FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, ::NFMsg::Server_Msg_AccountLogin, msg, nLen, pLinkInfo->mSendMsgCount);
	}
}

void NFCProxyLogicModule::OnHandleMessageFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElement(unLinkId);
	if (pServerData == nullptr)
	{
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg from loginserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv msg from loginserver, but can't find playerId:{} link info!", playerId);
		return;
	}

	pLinkInfo->mSendMsgCount++;
	FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
}

void NFCProxyLogicModule::OnHandleMessageFromWorldServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.GetElement(unLinkId);
	if (pServerData == nullptr)
	{
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg from worldserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv msg from worldserver, but can't find playerId:{} link info!", playerId);
		return;
	}

	pLinkInfo->mSendMsgCount++;
	FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
}

void NFCProxyLogicModule::OnHandleMessageFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(unLinkId);
	if (pServerData == nullptr)
	{
		return;
	}
	else
	{
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg from gameserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);
	}

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv msg from gameserver, but can't find playerId:{} link info!", playerId);
		return;
	}

	if (pLinkInfo->mUnlinkId > 0)
	{
		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
	}
	else
	{
		NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv msg from gameserver, but can't find link, player:{} disconnect!", playerId);
	}
}

/**
* @brief 处理心跳
*
* @return void
*/
void NFCProxyLogicModule::OnHandleHeartBeat(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgheartbeat cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv hearbeat, but can't find linkinfo:{}", unLinkId);
		return;
	}

	pLinkInfo->mRecvHeartBeatTime = NFGetSecondTime();

	NFMsg::gcheartbeat gcMsg;
	gcMsg.set_result(0);
	FindModule<NFINetServerModule>()->SendToServerByPB(unLinkId, NFMsg::Server_Msg_HeartBeat, gcMsg, 0);
}