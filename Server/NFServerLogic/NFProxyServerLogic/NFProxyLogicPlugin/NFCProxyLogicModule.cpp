

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
	/*
	** 处理来自客户端的连接和消息
	*/
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleMessageFromGameServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleMessageFromWorldServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, ::NFMsg::Server_Msg_AccountLogin, this, &NFCProxyLogicModule::OnHandleAccountLoginFromWorldServer);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_WORLD_NOTIFY_PROXY_CHANGE_GAME, this, &NFCProxyLogicModule::OnHandleNotifyChangeGameFromWorldServer);

	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_PROXY, NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleGameEventCallBack);
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_PROXY, NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleWorldEventCallBack);

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
			NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "client disconnect, but can not find linkId, unLinkId:{}, system error!", unLinkId);
			return;
		}
		mClientLinkInfo.RemoveElement(unLinkId);

		pLinkInfo->mUnlinkId = 0;
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

	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(pLinkInfo->mGameServerUnlinkId);
	if (pServerData)
	{
		FindModule<NFINetClientModule>()->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, unLinkId);
	}
	else
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} ,send msg:{}, can not find game server link, some thing wrong", ip, nMsgId);
		return;
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg -- ip:{}, operateId:{}, msgId:{}, msglen:{}", ip, playerId, nMsgId, nLen);
	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
	if (pServerData)
	{
		NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(unLinkId);
		if (pLinkInfo == nullptr)
		{
			pLinkInfo = NF_SHARE_PTR<ProxyLinkInfo>(NF_NEW ProxyLinkInfo());
			pLinkInfo->mUnlinkId = unLinkId;
			pLinkInfo->mIsLogin = false;
			pLinkInfo->mIPAddr = ip;
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
		FindModule<NFINetClientModule>()->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, unLinkId);
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
				if (pPlayerInfo->mUnlinkId != pLinkInfo->mUnlinkId)
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
		pLinkInfo->mGameServerUnlinkId = pGameServer->GetUnlinkId();
	}
	else
	{
		NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "notify change game, but proxy disconnect the game server id:{}", pLinkInfo->mGameServerId);
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromWorldServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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
		pLinkInfo->mIsLogin = true;
		pLinkInfo->mAccount = gcMsg.mutable_pinfo()->account();
		pLinkInfo->mPlayerId = gcMsg.mutable_pinfo()->userid();
		if (mPlayerLinkInfo.GetElement(pLinkInfo->mPlayerId) == nullptr)
		{
			NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "player:{} login, but not find the game server....", pLinkInfo->mPlayerId);
		}

		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
	}
	else
	{
		pLinkInfo->mIsLogin = false;

		pLinkInfo->mSendMsgCount++;
		FindModule<NFINetServerModule>()->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);

		if (pLinkInfo->mPlayerId > 0)
		{
			mPlayerLinkInfo.RemoveElement(pLinkInfo->mPlayerId);
		}
		FindModule<NFINetServerModule>()->CloseLinkId(clientLinkId);
	}
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

	pLinkInfo->mSendMsgCount++;
	FindModule<NFINetServerModule>()->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
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