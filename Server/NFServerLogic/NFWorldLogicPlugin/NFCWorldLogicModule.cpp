

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
#include <NFComm/NFPluginModule/NFILuaScriptModule.h>

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
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN, this, &NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_DISCONNECT, this, &NFCWorldLogicModule::OnHandlePlayerDisconnectFromProxyServer);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_RECONNECT, this, &NFCWorldLogicModule::OnHandlePlayerReconnectFromProxyServer);

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

void NFCWorldLogicModule::OnHandleMessageFromServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		std::string strMsg(msg, nLen);
		pLuaScriptModule->RunNetRecvLuaFunc("LuaNFrame.DispatchWorldTcp", unLinkId, playerId, operateId, nMsgId, strMsg);
	}
	else
	{
		NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, playerId, "msg:{} not handled!", nMsgId);
	}
}

void NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_CONNECTED, NF_ST_WORLD, unLinkId, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} login world server!", pInfo->mPlayerId);
}

void  NFCWorldLogicModule::OnHandlePlayerDisconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_DISCONNECTED, NF_ST_WORLD, 0, pInfo);
	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} disconnect world server!", pInfo->mPlayerId);
}

void NFCWorldLogicModule::OnHandlePlayerReconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_RECONNECTED, NF_ST_WORLD, unLinkId, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} reconnect world server!", pInfo->mPlayerId);
}