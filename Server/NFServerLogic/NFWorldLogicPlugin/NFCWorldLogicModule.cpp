

#include "NFCWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFCore/NFMD5.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"
#include "NFMessageDefine/st_human_packet_code.pb.h"
#include "NFMessageDefine/msg_human.pb.h"
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
	m_pServerNetEventModule = m_pPluginManager->FindModule<NFIServerNetEventModule>();

	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
	m_pNetProxyServerModule = m_pPluginManager->FindModule<NFIProxyServerModule>();

	m_pNetServerModule->AddReceiveCallBack(NF_ST_WORLD, this, &NFCWorldLogicModule::OnHandleMessageFromServer);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_WORLD, ::NFMsg::Client_Msg_AccountLogin, this, &NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_WORLD, ::NFMsg::Server_Msg_AccountLogin, this, &NFCWorldLogicModule::OnHandleAccountLoginFromGameServer);

	m_pServerNetEventModule->AddEventCallBack(NF_ST_WORLD, NF_ST_PROXY, this, &NFCWorldLogicModule::OnHandleProxyEventCallBack);
	m_pServerNetEventModule->AddEventCallBack(NF_ST_WORLD, NF_ST_GAME, this, &NFCWorldLogicModule::OnHandleGameEventCallBack);

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

void NFCWorldLogicModule::OnHandleMessageFromServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(unLinkId);
	if (pServerData)
	{
		NFLogInfo(NF_LOG_WORLD_RECV_MSG_LOG, 0, "recv msg from server -- serverType:{}, serverId:{}, playerId:{}, msgId:{}, msglen:{}, not handled!", GetServerName((NF_SERVER_TYPES)pServerData->GetServerType()), pServerData->GetServerId(), playerId, nMsgId, nLen);
		return;
	}

	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(unLinkId);
	if (pServerData)
	{
		NFLogInfo(NF_LOG_WORLD_RECV_MSG_LOG, 0, "recv msg from server -- serverType:{}, serverId:{}, playerId:{}, msgId:{}, msglen:{}, not handled!", GetServerName((NF_SERVER_TYPES)pServerData->GetServerType()), pServerData->GetServerId(), playerId, nMsgId, nLen);
		return;
	}
}

void NFCWorldLogicModule::OnHandleAccountLoginFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::cgaccountlogin cgMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, cgMsg);

	uint32_t proxyUnlinkId = (uint32_t)(playerId);
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

	pInfo->mProxyUnlinkId = proxyUnlinkId;
	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "account login:{} world server!", pInfo->mAccount);

	if (pInfo->mGameServerId > 0)
	{

	}
}

void NFCWorldLogicModule::OnHandleAccountLoginFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::gcaccountlogin gcMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, gcMsg);

	uint32_t clientLinkId = (uint32_t)playerId;
	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(clientLinkId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_SYSTEMLOG, 0, "clientLinkId:{} not exist, client maybe disconnect!", clientLinkId);
		return;
	}

	if (gcMsg.result() == 0)
	{
		pLinkInfo->mIsLogin = true;
		pLinkInfo->mAccount = gcMsg.mutable_pinfo()->account();
	}
	pLinkInfo->mSendMsgCount++;
	m_pNetServerModule->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
}

void NFCWorldLogicModule::OnHandleMessageFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(unLinkId);
	if (pServerData)
	{
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg from gameserver:{} -- playerId:{}, msgId:{}, msglen:{}", pServerData->GetServerName(), playerId, nMsgId, nLen);
	}
	else
	{
		NFLogInfo(NF_LOG_PROXY_RECV_MSG_LOG, 0, "recv msg from gameserver:(unknown disconnect) -- playerId:{}, msgId:{}, msglen:{}", playerId, nMsgId, nLen);
	}

	uint32_t clientLinkId = (uint32_t)playerId;
	NF_SHARE_PTR<ProxyLinkInfo> pLinkInfo = mClientLinkInfo.GetElement(clientLinkId);
	if (pLinkInfo == nullptr)
	{
		NFLogWarning(NF_LOG_SYSTEMLOG, 0, "clientLinkId:{} not exist, client maybe disconnect!", clientLinkId);
		return;
	}

	pLinkInfo->mSendMsgCount++;
	m_pNetServerModule->SendByServerID(clientLinkId, nMsgId, msg, nLen, pLinkInfo->mSendMsgCount);
}