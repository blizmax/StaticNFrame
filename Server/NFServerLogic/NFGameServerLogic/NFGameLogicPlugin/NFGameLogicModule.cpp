

#include "NFGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFISqliteModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFMessageDefine/server_to_server_msg.pb.h"
#include "NFMessageDefine/st_human_packet_code.pb.h"
#include "NFMessageDefine/msg_human.pb.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCGameLogicModule::NFCGameLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCGameLogicModule::~NFCGameLogicModule()
{

}

bool NFCGameLogicModule::Awake()
{
	bool ret = true;
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_GAME);
	if (pConfig == nullptr)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "NFServerCommon::GetAppConfig:NF_ST_GAME failed!");
		return false;
	}

	//ret = FindModule<NFIMysqlModule>()->AddMysqlServer(NF_ST_GAME, pConfig->mMysqlIp, pConfig->mMysqlPort, pConfig->mMysqlDbName, pConfig->mMysqlUser, pConfig->mMysqlPassword);
	//if (ret == false)
	//{
	//	NFLogError(NF_LOG_SYSTEMLOG, 0, "NFIMysqlModule AddMysqlServer failed!");
	//	return false;
	//}

	//ret = FindModule<NFIAsyMysqlModule>()->AddMysqlServer(NF_ST_GAME, pConfig->mMysqlIp, pConfig->mMysqlPort, pConfig->mMysqlDbName, pConfig->mMysqlUser, pConfig->mMysqlPassword);
	//if (ret == false)
	//{
	//	NFLogError(NF_LOG_SYSTEMLOG, 0, "NFIAsyMysqlModule AddMysqlServer failed!");
	//	return false;
	//}

	//ret = FindModule<NFINoSqlModule>()->AddConnectSql("nosql", pConfig->mNosqlIp, pConfig->mNosqlPort);
	//if (ret == false)
	//{
	//	NFLogError(NF_LOG_SYSTEMLOG, 0, "redis connect server failed, ip:{}, port:{}", pConfig->mNosqlIp, pConfig->mNosqlPort);
	//	return false;
	//}

	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_GAME, NF_ST_PROXY, this, &NFCGameLogicModule::OnHandleProxyEventCallBack);
	FindModule<NFIServerNetEventModule>()->AddEventCallBack(NF_ST_GAME, NF_ST_WORLD, this, &NFCGameLogicModule::OnHandleWorldEventCallBack);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_LOGIN, this, &NFCGameLogicModule::OnHandlePlayerLogin);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_DISCONNECT, this, &NFCGameLogicModule::OnHandlePlayerDisconnect);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_GAME, EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_RECONNECT, this, &NFCGameLogicModule::OnHandlePlayerReconnect);
	
	return true;
}

bool NFCGameLogicModule::Init()
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
	mProxyMap.ClearAll();
	mWorldMap.ClearAll();
	mPlayerProxyInfoMap.ClearAll();
	return true;
}

void NFCGameLogicModule::OnHandlePlayerLogin(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_CONNECTED, NF_ST_GAME, unLinkId, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} login game server!", pInfo->mPlayerId);
}

void NFCGameLogicModule::OnHandlePlayerDisconnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_DISCONNECTED, NF_ST_GAME, 0, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} disconnect game server!", pInfo->mPlayerId);
}

void NFCGameLogicModule::OnHandlePlayerReconnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

	FindModule<NFIServerNetEventModule>()->OnAccountNetEvent(eAccountEventType_RECONNECTED, NF_ST_GAME, unLinkId, pInfo);

	NFLogInfo(NF_LOG_SYSTEMLOG, pInfo->mPlayerId, "Player:{} reconnect game server!", pInfo->mPlayerId);
}

void NFCGameLogicModule::OnHandleProxyEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
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

void NFCGameLogicModule::OnHandleWorldEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
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

void NFCGameLogicModule::SendMsgToClientByPlayerId(uint64_t playerId, uint32_t nMsgId, const google::protobuf::Message& xData)
{
	auto pInfo = mPlayerProxyInfoMap.GetElement(playerId);
	if (pInfo)
	{
		FindModule<NFINetServerModule>()->SendToServerByPB(pInfo->mProxyUnlinkId, nMsgId, xData, playerId, 0);
	}
	else
	{
		NFLogWarning(NF_LOG_SYSTEMLOG, 0, "playerId:{} not exist, can't find send message!", playerId);
	}
}