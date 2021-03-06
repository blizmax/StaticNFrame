// -------------------------------------------------------------------------
//    @FileName         :    NFGameClient_WorldModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFGameClient_WorldModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCGameClient_WorldModule::NFCGameClient_WorldModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCGameClient_WorldModule::~NFCGameClient_WorldModule()
{
}

bool NFCGameClient_WorldModule::Init()
{
	return true;
}

bool NFCGameClient_WorldModule::AfterInit()
{
	FindModule<NFINetClientModule>()->AddEventCallBack(NF_ST_WORLD, this, &NFCGameClient_WorldModule::OnProxySocketEvent);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_WORLD, this, &NFCGameClient_WorldModule::OnHandleOtherMessage);

	return true;
}

bool NFCGameClient_WorldModule::Execute()
{
	return true;
}

bool NFCGameClient_WorldModule::BeforeShut()
{
	return true;
}

bool NFCGameClient_WorldModule::Shut()
{
	mUnlinkWorldMap.ClearAll();
	mWorldMap.ClearAll();
	return true;
}

void NFCGameClient_WorldModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server Connect World Server Success!");
		
		//连接成功，发送游戏服务器IP以及数据给世界服务器
		RegisterServer(unLinkId);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server DisConnect World Server!");

		OnClientDisconnect(unLinkId);
	}
}

void NFCGameClient_WorldModule::OnClientDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkWorldMap.GetElement(unLinkId);
	if (pServerData)
	{
		//game server 与 world server 一旦断开，不需要主动重新连接, 清理后将关闭这个unLinkId
		pServerData->mUnlinkId = 0;
		pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);

		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the world server disconnect, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
			, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, NF_ST_GAME, NF_ST_WORLD, unLinkId, pServerData);

		mUnlinkWorldMap.RemoveElement(unLinkId);

		FindModule<NFINetClientModule>()->CloseServer(unLinkId);
	}
}

void NFCGameClient_WorldModule::OnHandleWorldReport(const NFMsg::ServerInfoReport& xData)
{
	if (xData.server_type() != NF_ST_WORLD) return;
	//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "game will connect world server");
	NF_SHARE_PTR<NFServerData> pServerData = GetServerByServerId(xData.server_id());
	if (pServerData == nullptr)
	{
		pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
		mWorldMap.AddElement(xData.server_id(), pServerData);

		pServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_WORLD, xData.server_ip(), xData.server_port());
		mUnlinkWorldMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	if (pServerData->mUnlinkId <= 0)
	{
		pServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_WORLD, xData.server_ip(), xData.server_port());
		mUnlinkWorldMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	pServerData->mServerInfo = xData;
}

void NFCGameClient_WorldModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		std::string strMsg(msg, nLen);
		pLuaScriptModule->RunNetRecvLuaFunc("LuaNFrame.DispatchWorldTcp", unLinkId, playerId, nMsgId, strMsg);
	}
	else
	{
		NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "msg:{} not handled!", nMsgId);
	}
}

void NFCGameClient_WorldModule::RegisterServer(uint32_t linkId)
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_GAME);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		FindModule<NFINetClientModule>()->SendToServerByPB(linkId, EGMI_NET_GAME_TO_WORLD_REGISTER, xMsg, 0);
	}

	NF_SHARE_PTR<NFServerData> pServerData = mUnlinkWorldMap.GetElement(linkId);
	if (pServerData)
	{
		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, NF_ST_GAME, NF_ST_WORLD, linkId, pServerData);
	}
}


