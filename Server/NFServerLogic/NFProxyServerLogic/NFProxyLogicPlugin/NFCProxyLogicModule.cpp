

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
#include "NFComm/NFPluginModule/NFIMonitorModule.h"

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
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_PROXY, ::NFMsg::Client_Msg_EnterTable, this, &NFCProxyLogicModule::OnHandleEnterTableFromClient);
	
	/*
	** 处理来自客户端的连接和消息
	*/
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleMessageFromGameServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleMessageFromWorldServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, this, &NFCProxyLogicModule::OnHandleMessageFromLoginServer);

	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, ::NFMsg::Server_Msg_AccountLogin, this, &NFCProxyLogicModule::OnHandleAccountLoginFromLoginServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, NFMsg::Server_Msg_ReConnect, this, &NFCProxyLogicModule::OnHandleReconnectFromLoginServer);
	
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_GAME_SEND_PACKET_TO_PROXY, this, &NFCProxyLogicModule::OnHandlePacketMsgFromGameServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_GAME_SEND_PACKET_TO_PROXY, this, &NFCProxyLogicModule::OnHandlePacketMsgFromGameServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_LOGIN, EGMI_NET_GAME_SEND_PACKET_TO_PROXY, this, &NFCProxyLogicModule::OnHandlePacketMsgFromGameServer);

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

		auto pPlayerInfo = mPlayerLinkInfo.First();
		while (pPlayerInfo)
		{
			if (pPlayerInfo->mUnlinkId > 0)
			{
				NFMsg::gcaccountlogin gcMsg;
				FindModule<NFINetClientModule>()->SendToServerByPB(unLinkId, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_LOGIN, gcMsg, pPlayerInfo->mPlayerId, 0);
			}

			pPlayerInfo = mPlayerLinkInfo.Next();
		}
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

		auto pPlayerInfo = mPlayerLinkInfo.First();
		while (pPlayerInfo)
		{
			if (pPlayerInfo->mUnlinkId > 0)
			{
				NFMsg::gcaccountlogin gcMsg;
				FindModule<NFINetClientModule>()->SendToServerByPB(unLinkId, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_LOGIN, gcMsg, pPlayerInfo->mPlayerId, 0);
			}

			pPlayerInfo = mPlayerLinkInfo.Next();
		}
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

		auto pPlayerInfo = mPlayerLinkInfo.First();
		while (pPlayerInfo)
		{
			if (pPlayerInfo->mUnlinkId > 0)
			{
				NFMsg::gcaccountlogin gcMsg;
				FindModule<NFINetClientModule>()->SendToServerByPB(unLinkId, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN, gcMsg, pPlayerInfo->mPlayerId, 0);
			}

			pPlayerInfo = mPlayerLinkInfo.Next();
		}
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

NF_SHARE_PTR<NFServerData> NFCProxyLogicModule::GetWorldServerByServerId(uint32_t serverId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		if (pServerData->GetServerId() == serverId)
		{
			return pServerData;
		}
		pServerData = mWorldMap.Next();
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
			if (NFGetSecondTime() - pLinkInfo->mRecvHeartBeatTime > 5)
			{
				NFMsg::gcheartbeat gcMsg;
				gcMsg.set_result(0);
				FindModule<NFINetServerModule>()->SendToServerByPB(pLinkInfo->mUnlinkId, NFMsg::Server_Msg_HeartBeat, gcMsg, 0, 0);
			}
		}
		pLinkInfo = mClientLinkInfo.Next();
	}

	NFIMonitorModule* pMonitorModule = FindModule<NFIMonitorModule>();
	if (pMonitorModule)
	{
		pMonitorModule->SetUserCount(mClientLinkInfo.Count());
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

				{
					NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.First();
					while (pServerData)
					{
						NFMsg::NotifyPlayerDisconnect msg;
						msg.set_user_id(pLinkInfo->mPlayerId);
						msg.set_account(pLinkInfo->mAccount);
						FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_DISCONNECT, msg, pLinkInfo->mPlayerId, 0);

						pServerData = mLoginMap.Next();
					}
				}

				{
					NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
					while (pServerData)
					{
						NFMsg::NotifyPlayerDisconnect msg;
						msg.set_user_id(pLinkInfo->mPlayerId);
						msg.set_account(pLinkInfo->mAccount);
						FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_DISCONNECT, msg, pLinkInfo->mPlayerId, 0);

						pServerData = mWorldMap.Next();
					}
				}

				{
					NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
					while (pServerData)
					{
						NFMsg::NotifyPlayerDisconnect msg;
						msg.set_user_id(pLinkInfo->mPlayerId);
						msg.set_account(pLinkInfo->mAccount);
						FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_DISCONNECT, msg, pLinkInfo->mPlayerId, 0);

						pServerData = mGameMap.Next();
					}
				}

				{
					
					NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElementBySuitRandom();
					if (pServerData)
					{
						NFMsg::NotifyPlayerDisconnect msg;
						msg.set_user_id(pLinkInfo->mPlayerId);
						msg.set_account(pLinkInfo->mAccount);
						FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_PROXY_NOTIFY_LOGIN_HANDLE_PLAYER_DISCONNECT, msg, pLinkInfo->mPlayerId, 0);
					}
				}
			}
		}

		mClientLinkInfo.RemoveElement(unLinkId);
	}
}

void NFCProxyLogicModule::OnHandleMessageFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
	if (pLinkInfo == nullptr || pLinkInfo->mIsLogin == false)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} not login,send msg:{}, the msg will not send to gameserver", ip, nMsgId);
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, pLinkInfo->mPlayerId, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);

	//human msg 1000-1100  //shop msg 1100-1200  //misson msg 1200-1300 //agent msg 1300-1400 //rank msg 400 - 1500
	if (nMsgId >= 1000 && nMsgId < 1500)
	{
		NF_SHARE_PTR<NFServerData> pServerData = GetWorldServerByServerId(pLinkInfo->mWorldServerId);
		if (pServerData)
		{
			FindModule<NFINetClientModule>()->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mPlayerId, operateId);
		}
		else
		{
			NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} ,send msg:{}, can not find world server link, some thing wrong", ip, nMsgId);
			return;
		}
	}
	else
	{
		NF_SHARE_PTR<NFServerData> pServerData = GetGameServerByServerId(pLinkInfo->mGameServerId);
		if (pServerData)
		{
			FindModule<NFINetClientModule>()->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mPlayerId, operateId);
		}
		else
		{
			NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} ,send msg:{}, can not find game server link, some thing wrong", ip, nMsgId);
			return;
		}
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);
	NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElementBySuitRandom();
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
		FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgId, cgMsg, unLinkId, operateId);
	}
}

void NFCProxyLogicModule::OnHandleReconnectFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcreconnect gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	NF_SHARE_PTR<ProxyLinkInfo> pPlayerLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pPlayerLinkInfo)
	{
		if (gcMsg.result() == 0)
		{
			NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, playerId, "Player:{} reocnnect success!", playerId);
			
			NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(pPlayerLinkInfo->mUnlinkId);
			if (pLinkInfo == nullptr)
			{
				NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "clientLinkId:{} not exist, client maybe disconnect!", pPlayerLinkInfo->mUnlinkId);
				pPlayerLinkInfo->mUnlinkId = 0;
				pPlayerLinkInfo->mIsLogin = false;
				return;
			}

			pLinkInfo->mAccount = pPlayerLinkInfo->mAccount;
			pLinkInfo->mPlayerId = pPlayerLinkInfo->mPlayerId;
			pLinkInfo->mGameServerId = pPlayerLinkInfo->mGameServerId;
			pLinkInfo->mWorldServerId = pPlayerLinkInfo->mWorldServerId;
			pPlayerLinkInfo->mIsLogin = true;
			pLinkInfo->mIsLogin = true;
			FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, playerId, 0);

			NF_SHARE_PTR<NFServerData> pLoginServer = mLoginMap.First();
			while (pLoginServer)
			{
				pLinkInfo->mSendMsgCount++;
				FindModule<NFINetClientModule>()->SendToServerByPB(pLoginServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_RECONNECT, gcMsg, pLinkInfo->mPlayerId, 0);

				pLoginServer = mLoginMap.Next();
			}

			NF_SHARE_PTR<NFServerData> pWorldServer = mWorldMap.First();
			while (pWorldServer)
			{
				pLinkInfo->mSendMsgCount++;
				FindModule<NFINetClientModule>()->SendToServerByPB(pWorldServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_RECONNECT, gcMsg, pLinkInfo->mPlayerId, 0);

				pWorldServer = mWorldMap.Next();
			}

			pWorldServer = GetWorldServerByServerId(pPlayerLinkInfo->mWorldServerId);
			if (!pWorldServer)
			{
				pWorldServer = mWorldMap.GetElementBySuit(pPlayerLinkInfo->mPlayerId);
				if (pWorldServer)
				{
					pPlayerLinkInfo->mWorldServerId = pWorldServer->GetServerId();
					pLinkInfo->mWorldServerId = pWorldServer->GetServerId();
				}
			}


			NF_SHARE_PTR<NFServerData> pGameServer = mGameMap.First();
			while (pGameServer)
			{
				pLinkInfo->mSendMsgCount++;
				FindModule<NFINetClientModule>()->SendToServerByPB(pGameServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_RECONNECT, gcMsg, pLinkInfo->mPlayerId, 0);

				pGameServer = mGameMap.Next();
			}

			pGameServer = GetGameServerByServerId(pPlayerLinkInfo->mGameServerId);
			if (!pGameServer)
			{
				pGameServer = mGameMap.GetElementBySuit(pPlayerLinkInfo->mPlayerId);
				if (pGameServer)
				{
					pPlayerLinkInfo->mGameServerId = pGameServer->GetServerId();
					pLinkInfo->mGameServerId = pGameServer->GetServerId();
				}
			}
		}
		else
		{
			NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "Player:{} reocnnect failed!", playerId);
			pPlayerLinkInfo->mIsLogin = false;
			NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(pPlayerLinkInfo->mUnlinkId);
			if (pLinkInfo)
			{
				pLinkInfo->mIsLogin = false;
				FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, playerId, 0);
			}
		}
	}
}

void NFCProxyLogicModule::OnHandleEnterTableFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgentertable cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
	if (pLinkInfo == nullptr || pLinkInfo->mIsLogin == false)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} not login,send msg:{}, the msg will not send to gameserver", ip, nMsgId);
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, pLinkInfo->mPlayerId, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);

	NF_SHARE_PTR<NFServerData> pServerData = GetGameServerByServerId(pLinkInfo->mGameServerId);
	if (pServerData)
	{
		FindModule<NFINetClientModule>()->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mPlayerId, operateId);
	}
	else
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} ,send msg:{}, can not find game server link, some thing wrong", ip, nMsgId);
		return;
	}
}

void NFCProxyLogicModule::OnHandleReconnectFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgreconnect cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, cgMsg.userid(), "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pPlayerLinkInfo = mPlayerLinkInfo.GetElement(cgMsg.userid());
	if (pPlayerLinkInfo == nullptr)
	{
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, cgMsg.userid(), "Player:{} reconnect proxy server, reconnect failed!", cgMsg.userid());
		NFMsg::gcreconnect gcMsg;
		gcMsg.set_result(-1);
		FindModule<NFINetServerModule>()->SendToServerByPB(unLinkId, NFMsg::Server_Msg_ReConnect, gcMsg, cgMsg.userid(), 0);
		return;
	}

	if (pPlayerLinkInfo->mUnlinkId > 0)
	{
		if (pPlayerLinkInfo->mUnlinkId == unLinkId)
		{
			NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, cgMsg.userid(), "Player:{} reconnect proxy server, but unlink not change! some wrong!", cgMsg.userid());
		}
		else
		{
			NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, cgMsg.userid(), "Player:{} reconnect proxy server, but unlink exist", cgMsg.userid());
			//if (mClientLinkInfo.GetElement(pPlayerLinkInfo->mUnlinkId))
			//{
			//	FindModule<NFINetServerModule>()->CloseLinkId(pPlayerLinkInfo->mUnlinkId);
			//}

			//mClientLinkInfo.RemoveElement(pPlayerLinkInfo->mUnlinkId);
			auto pOtherInfo = mClientLinkInfo.GetElement(pPlayerLinkInfo->mUnlinkId);
			if (pOtherInfo)
			{
				pOtherInfo->mIsLogin = false;
				pOtherInfo->mAccount = "";
				pOtherInfo->mPlayerId = 0;
			}
		}
	}

	pPlayerLinkInfo->mUnlinkId = unLinkId;
	pPlayerLinkInfo->mIsLogin = true;
	pPlayerLinkInfo->mIPAddr = ip;
	pPlayerLinkInfo->mPlayerId = cgMsg.userid();

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = NF_SHARE_PTR<ProxyLinkInfo>(NF_NEW ProxyLinkInfo());
	if (pLinkInfo)
	{
		pLinkInfo->mUnlinkId = unLinkId;
		pLinkInfo->mIsLogin = true;
		pLinkInfo->mIPAddr = ip;
		pLinkInfo->mPlayerId = pPlayerLinkInfo->mPlayerId;
		pLinkInfo->mAccount = pPlayerLinkInfo->mAccount;
		pLinkInfo->mGameServerId = pPlayerLinkInfo->mGameServerId;
		pLinkInfo->mWorldServerId = pPlayerLinkInfo->mWorldServerId;
	}
	mClientLinkInfo.AddElement(unLinkId, pLinkInfo);

	NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElementBySuitRandom();
	if (pServerData)
	{
		NFLogInfo(NF_LOG_PROXY_LOGIC_PLUGIN, cgMsg.userid(), "Player:{} reconnect proxy server!", cgMsg.userid());
		FindModule<NFINetClientModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgId, cgMsg, cgMsg.userid(), operateId);
	}
}

void NFCProxyLogicModule::OnHandlePacketMsgFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::NotifyProxyPacketMsg gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	for (int i = 0; i < gcMsg.user_id_size(); i++)
	{
		uint64_t userid = gcMsg.user_id(i);
		
		auto pPlayerInfo = mPlayerLinkInfo.GetElement(userid);
		if (pPlayerInfo)
		{
			NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(pPlayerInfo->mUnlinkId);
			if (pLinkInfo == nullptr)
			{
				NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv packet msg  -- but player disconnect, playerId:{}, msgId:{}, msglen:{}", userid, gcMsg.msg_id(), gcMsg.msg().length());
				continue;
			}
			FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, gcMsg.msg_id(), gcMsg.msg().data(), gcMsg.msg().length(), pLinkInfo->mSendMsgCount);
		}
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, gcMsg.mutable_pinfo()->userid(), "player login success -- playerid:{} account:{} ip:{}", gcMsg.mutable_pinfo()->userid(), gcMsg.mutable_pinfo()->account(), pLinkInfo->mIPAddr);

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
				//if (mClientLinkInfo.GetElement(pPlayerInfo->mUnlinkId))
				//{
				//	FindModule<NFINetServerModule>()->CloseLinkId(pPlayerInfo->mUnlinkId);
				//}
				//mClientLinkInfo.RemoveElement(pPlayerInfo->mUnlinkId);
				auto pOtherInfo = mClientLinkInfo.GetElement(pPlayerInfo->mUnlinkId);
				if (pOtherInfo)
				{
					pOtherInfo->mIsLogin = false;
					pOtherInfo->mAccount = "";
					pOtherInfo->mPlayerId = 0;
				}
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

		NF_SHARE_PTR<NFServerData> pLoginServer = mLoginMap.First();
		while (pLoginServer)
		{
			pLinkInfo->mSendMsgCount++;
			FindModule<NFINetClientModule>()->SendToServerByPB(pLoginServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_LOGIN, gcMsg, pLinkInfo->mPlayerId, 0);

			pLoginServer = mLoginMap.Next();
		}

		NF_SHARE_PTR<NFServerData> pWorldServer = mWorldMap.First();
		while (pWorldServer)
		{
			pLinkInfo->mSendMsgCount++;
			FindModule<NFINetClientModule>()->SendToServerByPB(pWorldServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN, gcMsg, pLinkInfo->mPlayerId, 0);

			pWorldServer = mWorldMap.Next();
		}

		pWorldServer = mWorldMap.GetElementBySuit(pPlayerInfo->mPlayerId);
		if (pWorldServer)
		{
			pPlayerInfo->mWorldServerId = pWorldServer->GetServerId();
			pLinkInfo->mWorldServerId = pWorldServer->GetServerId();
		}

		NF_SHARE_PTR<NFServerData> pGameServer = mGameMap.First();
		while (pGameServer)
		{
			pLinkInfo->mSendMsgCount++;
			FindModule<NFINetClientModule>()->SendToServerByPB(pGameServer->mUnlinkId, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_LOGIN, gcMsg, pLinkInfo->mPlayerId, 0);

			pGameServer = mGameMap.Next();
		}

		pGameServer = mGameMap.GetElementBySuit(pPlayerInfo->mPlayerId);
		if (pGameServer)
		{
			pPlayerInfo->mGameServerId = pGameServer->GetServerId();
			pLinkInfo->mGameServerId = pGameServer->GetServerId();
		}

		pLinkInfo->mIsLogin = true;
		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(pPlayerInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount, 0);

		NFMsg::NotifyPlayerReport reportMsg;
		reportMsg.set_user_id(pPlayerInfo->mPlayerId);
		reportMsg.set_ip(pPlayerInfo->mIPAddr);

		FindModule<NFINetClientModule>()->SendToServerByPB(unLinkId, ::EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_REPORT, reportMsg, pPlayerInfo->mPlayerId, 0);
	}
	else
	{
		pLinkInfo->mIsLogin = false;
		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount, 0);
		FindModule<NFINetServerModule>()->CloseLinkId(clientLinkId);
	}
}

void NFCProxyLogicModule::OnHandleMessageFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElement(unLinkId);
	if (pServerData == nullptr)
	{
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, playerId, "recv msg from loginserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv msg from loginserver, but can't find playerId:{} link info!", playerId);
		return;
	}

	if (pLinkInfo->mUnlinkId > 0)
	{
		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount, 0);
	}
}

void NFCProxyLogicModule::OnHandleMessageFromWorldServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.GetElement(unLinkId);
	if (pServerData == nullptr)
	{
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, playerId, "recv msg from worldserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mPlayerLinkInfo.GetElement(playerId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "recv msg from worldserver, but can't find playerId:{} link info!", playerId);
		return;
	}

	if (pLinkInfo->mUnlinkId > 0)
	{
		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount, 0);
	}
}

void NFCProxyLogicModule::OnHandleMessageFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(unLinkId);
	if (pServerData == nullptr)
	{
		return;
	}
	else
	{
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, playerId, "recv msg from gameserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);
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
		FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount, 0);
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
void NFCProxyLogicModule::OnHandleHeartBeat(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgheartbeat cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);

	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv hearbeat, but can't find linkinfo:{}", unLinkId);

		NFMsg::gcheartbeat gcMsg;
		gcMsg.set_result(0);
		FindModule<NFINetServerModule>()->SendToServerByPB(unLinkId, NFMsg::Server_Msg_HeartBeat, gcMsg, 0, 0);
		return;
	}

	NFLogInfo(NF_LOG_PROXY_RECV_HEART_LOG, pLinkInfo->mPlayerId, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);

	pLinkInfo->mRecvHeartBeatTime = NFGetSecondTime();

	NFMsg::gcheartbeat gcMsg;
	gcMsg.set_result(0);
	FindModule<NFINetServerModule>()->SendToServerByPB(unLinkId, NFMsg::Server_Msg_HeartBeat, gcMsg, 0, 0);
}