// -------------------------------------------------------------------------
//    @FileName         :    NFCMasterServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCMasterServerModule
//
// -------------------------------------------------------------------------

#include "NFMasterServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCMasterServerModule::NFCMasterServerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCMasterServerModule::~NFCMasterServerModule()
{
}

bool NFCMasterServerModule::Init()
{
	m_pNetServerModule = pPluginManager->FindModule<NFINetServerModule>();
	m_pNetServerModule->AddEventCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnProxySocketEvent);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnHandleOtherMessage);


	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnLoginServerRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnWorldServerRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnProxyServerRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnGameServerRegisterProcess);

	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnLoginServerUnRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnWorldServerUnRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnProxyServerUnRegisterProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnGameServerUnRegisterProcess);

	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnLoginServerRefreshProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnWorldServerRefreshProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnProxyServerRefreshProcess);
	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnGameServerRefreshProcess);

	m_pNetServerModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_SERVER_REPORT, this, &NFCMasterServerModule::OnServerReport);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		uint32_t unlinkId = m_pNetServerModule->AddServer(NF_ST_MASTER, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo("master server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo("master server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
	}
	else
	{
		NFLogError("I Can't get the Master Server config!");
		return false;
	}

	return true;
}

bool NFCMasterServerModule::AfterInit()
{
	return true;
}

bool NFCMasterServerModule::Execute()
{
	return true;
}

bool NFCMasterServerModule::BeforeShut()
{
	return true;
}

bool NFCMasterServerModule::Shut()
{
	return true;
}

void NFCMasterServerModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{

	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		OnClientDisconnect(unLinkId);
	}
}

void NFCMasterServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
}

void NFCMasterServerModule::OnClientDisconnect(uint32_t unLinkId)
{
	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;

			NFLogError("the world server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			return;
		}

		pServerData = mWorldMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	int nServerID = 0;
	pServerData = mLoginMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			nServerID = pServerData->mServerInfo.server_id();

			NFLogError("the login server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
			break;
		}

		pServerData = mLoginMap.Next();
	}

	mLoginMap.RemoveElement(nServerID);
}

void NFCMasterServerModule::OnLoginServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mLoginMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		NFLogInfo("Login Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
	SynReportToLogin();
}

void NFCMasterServerModule::OnLoginServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mLoginMap.RemoveElement(xData.server_id());

		NFLogInfo("Login Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnLoginServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mLoginMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		//NFLogInfo("Login Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

//游戏服务器注册协议回调
void NFCMasterServerModule::OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mGameMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		SynProxyToGame(unLinkId);
		SynWorldToGame(unLinkId);
		SynReportToLogin(pServerData);
		NFLogInfo("Game Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mGameMap.RemoveElement(xData.server_id());

		NFLogInfo("Game Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mGameMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mGameMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		//NFLogInfo("Game Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		SynReportToLogin(pServerData);
		NFLogInfo("Proxy Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}

	SynProxyToGame();
	SynProxyToWorld();
}

void NFCMasterServerModule::OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());

		NFLogInfo("Proxy Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		//NFLogInfo("Proxy Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		SynProxyToWorld(unLinkId);
		SynReportToLogin(pServerData);
		NFLogInfo("World Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
	SynWorldToGame();
}

void NFCMasterServerModule::OnWorldServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mWorldMap.RemoveElement(xData.server_id());

		NFLogInfo("World Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnWorldServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		//NFLogInfo("World Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::SynProxyToWorld()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		SynProxyToWorld(pServerData->mUnlinkId);

		pServerData = mWorldMap.Next();
	}
}

void NFCMasterServerModule::SynProxyToWorld(uint32_t unlinkId)
{
	if (mProxyMap.Count() <= 0) return;

	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = pServerData->mServerInfo;

		pServerData = mProxyMap.Next();
	}

	m_pNetServerModule->SendToServerByPB(unlinkId, EGMI_NET_MASTER_SEND_PROXY_TO_WORLD, xData, 0);
}

void NFCMasterServerModule::SynWorldToGame()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		SynWorldToGame(pServerData->mUnlinkId);

		pServerData = mGameMap.Next();
	}
}

void NFCMasterServerModule::SynWorldToGame(uint32_t unlinkId)
{
	if (mWorldMap.Count() <= 0) return;

	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = pServerData->mServerInfo;

		pServerData = mWorldMap.Next();
	}

	m_pNetServerModule->SendToServerByPB(unlinkId, EGMI_NET_MASTER_SEND_WORLD_TO_GAME, xData, 0);
}

void NFCMasterServerModule::SynProxyToGame()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
	while (pServerData)
	{
		SynProxyToGame(pServerData->mUnlinkId);

		pServerData = mGameMap.Next();
	}
}

void NFCMasterServerModule::SynProxyToGame(uint32_t unlinkId)
{
	if (mProxyMap.Count() <= 0) return;

	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = pServerData->mServerInfo;

		pServerData = mProxyMap.Next();
	}

	m_pNetServerModule->SendToServerByPB(unlinkId, EGMI_NET_MASTER_SEND_PROXY_TO_GAME, xData, 0);
}

void NFCMasterServerModule::SynReportToLogin()
{
	NFMsg::ServerInfoReportList xData;

	{
		NF_SHARE_PTR<NFServerData> pServerData = mProxyMap.First();
		while (pServerData)
		{
			NFMsg::ServerInfoReport* pData = xData.add_server_list();
			*pData = pServerData->mServerInfo;

			pServerData = mProxyMap.Next();
		}
	}

	{
		NF_SHARE_PTR<NFServerData> pServerData = mGameMap.First();
		while (pServerData)
		{
			NFMsg::ServerInfoReport* pData = xData.add_server_list();
			*pData = pServerData->mServerInfo;

			pServerData = mGameMap.Next();
		}
	}

	{
		NF_SHARE_PTR<NFServerData> pServerData = mWorldMap.First();
		while (pServerData)
		{
			NFMsg::ServerInfoReport* pData = xData.add_server_list();
			*pData = pServerData->mServerInfo;

			pServerData = mWorldMap.Next();
		}
	}

	{
		NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.First();
		while (pServerData)
		{
			m_pNetServerModule->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_SERVER_TO_LOGIN, xData, 0);

			pServerData = mLoginMap.Next();
		}
	}
}

void NFCMasterServerModule::SynReportToLogin(NF_SHARE_PTR<NFServerData> pServerReport)
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<NFServerData> pServerData = mLoginMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReportList xData;

		if (pServerReport)
		{
			NFMsg::ServerInfoReport* pData = xData.add_server_list();
			*pData = pServerReport->mServerInfo;
		}

		m_pNetServerModule->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_SERVER_TO_LOGIN, xData, 0);

		pServerData = mLoginMap.Next();
	}
}

void NFCMasterServerModule::OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData;
		switch (xData.server_type())
		{
		case NF_SERVER_TYPES::NF_ST_LOGIN:
		{
			pServerData = mLoginMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mLoginMap.AddElement(xData.server_id(), pServerData);
			}
			if (pServerData)
			{
				pServerData->mUnlinkId = unLinkId;
				pServerData->mServerInfo = xData;
			}
		}
		break;
		case NF_SERVER_TYPES::NF_ST_WORLD:
		{
			pServerData = mWorldMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mWorldMap.AddElement(xData.server_id(), pServerData);
			}
			if (pServerData)
			{
				pServerData->mUnlinkId = unLinkId;
				pServerData->mServerInfo = xData;
			}
			SynReportToLogin(pServerData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_PROXY:
		{
			pServerData = mProxyMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mProxyMap.AddElement(xData.server_id(), pServerData);
			}
			if (pServerData)
			{
				pServerData->mUnlinkId = unLinkId;
				pServerData->mServerInfo = xData;
			}
			SynReportToLogin(pServerData);
		}
		break;
		case NF_SERVER_TYPES::NF_ST_GAME:
		{
			pServerData = mGameMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mGameMap.AddElement(xData.server_id(), pServerData);
			}
			if (pServerData)
			{
				pServerData->mUnlinkId = unLinkId;
				pServerData->mServerInfo = xData;
			}
			SynReportToLogin(pServerData);
		}
		break;
		}
	}
}
