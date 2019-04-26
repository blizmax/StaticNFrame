

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
	m_pServerNetEventModule = m_pPluginManager->FindModule<NFIServerNetEventModule>();

	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
	m_pNetProxyServerModule = m_pPluginManager->FindModule<NFIProxyServerModule>();

	/*
	** 处理来自客户端的连接和消息
	*/
	m_pNetServerModule->AddEventCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, this, &NFCProxyLogicModule::OnHandleMessageFromClient);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_PROXY, ::NFMsg::Client_Msg_AccountLogin, this, &NFCProxyLogicModule::OnHandleAccountLoginFromClient);

	/*
	** 处理来自客户端的连接和消息
	*/
	m_pNetClientModule->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleMessageFromGameServer);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleMessageFromWorldServer);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, ::NFMsg::Server_Msg_AccountLogin, this, &NFCProxyLogicModule::OnHandleAccountLoginFromWorldServer);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_WORLD_NOTIFY_PROXY_CHANGE_GAME, this, &NFCProxyLogicModule::OnHandleNotifyChangeGameFromWorldServer);

	m_pServerNetEventModule->AddEventCallBack(NF_ST_PROXY, NF_ST_GAME, this, &NFCProxyLogicModule::OnHandleGameEventCallBack);
	m_pServerNetEventModule->AddEventCallBack(NF_ST_PROXY, NF_ST_WORLD, this, &NFCProxyLogicModule::OnHandleWorldEventCallBack);

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
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
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
		m_pNetClientModule->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, unLinkId);
	}
	else
	{
		NFLogWarning(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "ip:{} ,send msg:{}, can not find game server link, some thing wrong", ip, nMsgId);
		return;
	}
}

void NFCProxyLogicModule::OnHandleAccountLoginFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = m_pNetServerModule->GetLinkIp(unLinkId);
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
		m_pNetClientModule->SendByServerID(pServerData->mUnlinkId, nMsgId, msg, nLen, unLinkId);
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
		uint64_t realPlayerId = gcMsg.mutable_pinfo()->userid();
		pLinkInfo->mPlayerId = realPlayerId;
		if (mPlayerLinkInfo.GetElement(pLinkInfo->mPlayerId) == nullptr)
		{
			NFLogError(NF_LOG_PROXY_LOGIC_PLUGIN, 0, "player:{} login, but not find the game server....", pLinkInfo->mPlayerId);
		}
	}
	pLinkInfo->mSendMsgCount++;
	m_pNetServerModule->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
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
	m_pNetServerModule->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
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
	m_pNetServerModule->SendByServerID(pLinkInfo->mUnlinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
}