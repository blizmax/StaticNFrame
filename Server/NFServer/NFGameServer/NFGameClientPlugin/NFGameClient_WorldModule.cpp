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

NFCGameClient_WorldModule::NFCGameClient_WorldModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCGameClient_WorldModule::~NFCGameClient_WorldModule()
{
}

bool NFCGameClient_WorldModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCGameClient_WorldModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_WORLD, this, &NFCGameClient_WorldModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, this, &NFCGameClient_WorldModule::OnHandleOtherMessage);

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
	return true;
}

void NFCGameClient_WorldModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Game Server Connect World Server Success!");
		
		//连接成功，发送游戏服务器IP以及数据给世界服务器
		RegisterServer(unLinkId);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Game Server DisConnect World Server!");

		OnClientDisconnect(unLinkId);
	}
}

void NFCGameClient_WorldModule::OnClientDisconnect(uint32_t unLinkId)
{
	NFServerData* pServerData = mUnlinkWorldMap.GetElement(unLinkId);
	if (pServerData)
	{
		pServerData->mUnlinkId = 0;
		pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);

		NFLogError("the world server disconnect, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
			, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

		mUnlinkWorldMap.RemoveElement(unLinkId);
	}
}

void NFCGameClient_WorldModule::OnHandleWorldReport(const NFMsg::ServerInfoReport& xData)
{
	if (xData.server_type() != NF_ST_WORLD) return;

	NFServerData* pServerData = GetServerByServerId(xData.server_id());
	if (pServerData == nullptr)
	{
		pServerData = NF_NEW NFServerData();
		mWorldMap.AddElement(xData.server_id(), pServerData);

		pServerData->mUnlinkId = m_pNetClientModule->AddServer(NF_ST_WORLD, xData.server_ip(), xData.server_port());
		mUnlinkWorldMap.AddElement(pServerData->mUnlinkId, pServerData);
	}

	pServerData->mServerInfo = xData;
}

void NFCGameClient_WorldModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (GetServerByUnlinkId(unLinkId) == nullptr) return;

	NFLogWarning("msg:{} not handled!", nMsgId);
}

void NFCGameClient_WorldModule::RegisterServer(uint32_t linkId)
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_GAME);
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

		m_pNetClientModule->SendToServerByPB(linkId, EGMI_NET_GAME_TO_WORLD_REGISTER, xMsg, 0);
	}
}


