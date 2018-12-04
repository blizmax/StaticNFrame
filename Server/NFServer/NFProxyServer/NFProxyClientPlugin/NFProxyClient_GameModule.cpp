// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClient_GameModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCProxyClient_GameModule::NFCProxyClient_GameModule(NFIPluginManager* p)
{
	pPluginManager = p;
	m_pNetClientModule = nullptr;
}

NFCProxyClient_GameModule::~NFCProxyClient_GameModule()
{
}

bool NFCProxyClient_GameModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	return true;
}

bool NFCProxyClient_GameModule::AfterInit()
{
	m_pNetClientModule->AddEventCallBack(NF_ST_GAME, this, &NFCProxyClient_GameModule::OnProxySocketEvent);
	m_pNetClientModule->AddReceiveCallBack(NF_ST_GAME, this, &NFCProxyClient_GameModule::OnHandleOtherMessage);

	//等待世界服务器消息返回， 处理游戏服务器连接
	m_pNetClientModule->AddReceiveCallBack(NF_ST_WORLD, EGMI_NET_WORLD_TO_PROXY_SEND_GAME, this, &NFCProxyClient_GameModule::OnHandleWorldSendGameMessage);
	return true;
}

bool NFCProxyClient_GameModule::Execute()
{
	return true;
}

bool NFCProxyClient_GameModule::BeforeShut()
{
	return true;
}

bool NFCProxyClient_GameModule::Shut()
{
	return true;
}

void NFCProxyClient_GameModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (mLinkIdMap.find(unLinkId) == mLinkIdMap.end()) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug("Proxy Server Connect Game Server Success!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_GAME_SUCCESS, unLinkId, NF_ST_GAME, nullptr);

		RegisterServer(unLinkId);
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug("Proxy Server DisConnect Game Server!");
		NFEventMgr::Instance()->FireExecute(NFEVENT_PROXY_CONNECT_GAME_FAIL, unLinkId, NF_ST_GAME, nullptr);
		OnClientDisconnect(unLinkId);
	}
}

void NFCProxyClient_GameModule::OnClientDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);

			NFLogError("the game server disconnect, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
			return;
		}

		pServerData = mGameMap.Next();
	}
}

void NFCProxyClient_GameModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (mLinkIdMap.find(unLinkId) == mLinkIdMap.end()) return;
}

void NFCProxyClient_GameModule::RegisterServer(uint32_t unLinkId)
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_PROXY);
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

		m_pNetClientModule->SendToServerByPB(unLinkId, EGMI_NET_PROXY_TO_GAME_REGISTER, xMsg, 0);
	}
}

void NFCProxyClient_GameModule::OnHandleWorldSendGameMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xServerData = xMsg.server_list(i);

		switch (xServerData.server_type())
		{
		case NF_SERVER_TYPES::NF_ST_GAME:
		{
			NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(xServerData.server_id());
			if (!pServerData)
			{
				pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
				mGameMap.AddElement(xServerData.server_id(), pServerData);

				pServerData->mServerInfo = xServerData;

				pServerData->mUnlinkId = m_pNetClientModule->AddServer(NF_SERVER_TYPES::NF_ST_GAME, xServerData.server_ip(), xServerData.server_port());
				mLinkIdMap.emplace(pServerData->mUnlinkId, pServerData->mServerInfo.server_id());
			}
			else
			{
				pServerData->mServerInfo = xServerData;
			}
		}
		break;
		default:
			break;
		}
	}
}

NF_SHARE_PTR<NFServerData> NFCProxyClient_GameModule::GetServerData(uint32_t serverId)
{
	return mGameMap.GetElement(serverId);
}



