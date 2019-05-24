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
#include "NFComm/NFCore/NFMD5.h"
#include "NFMessageDefine/db_server.pb.h"
#include "NFComm/NFPluginModule/NFIMonitorModule.h"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFMessageDefine/msg_gm.pb.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

#define NF_MASTER_TIMER_SAVE_SERVER_DATA 0
#define NF_MASTER_TIMER_SAVE_SERVER_DATA_TIME 60000

NFCMasterServerModule::NFCMasterServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCMasterServerModule::~NFCMasterServerModule()
{
}

bool NFCMasterServerModule::Init()
{
	this->SetTimer(NF_MASTER_TIMER_SAVE_SERVER_DATA, NF_MASTER_TIMER_SAVE_SERVER_DATA_TIME, INFINITY_CALL);

	FindModule<NFIHttpServerModule>()->AddRequestHandler(NF_ST_MASTER, "/gm", NFHttpType::NF_HTTP_REQ_POST, this, &NFCMasterServerModule::HttpHandleHttpGm);
	FindModule<NFIHttpServerModule>()->AddRequestHandler(NF_ST_MASTER, "/GM", NFHttpType::NF_HTTP_REQ_POST, this, &NFCMasterServerModule::HttpHandleHttpGm);

	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnProxySocketEvent);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnHandleOtherMessage);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnLoginServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnWorldServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnProxyServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnGameServerRegisterProcess);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnLoginServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnWorldServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnProxyServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnGameServerUnRegisterProcess);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnLoginServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnWorldServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnProxyServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnGameServerRefreshProcess);
	
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_SERVER_REPORT, this, &NFCMasterServerModule::OnServerReport);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		uint32_t unlinkId = FindModule<NFINetServerModule>()->AddServer(NF_ST_MASTER, pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		if (unlinkId != 0)
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "master server listen success, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
		}
		else
		{
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "master server listen failed!, serverId:{}, maxConnectNum:{}, port:{}", pConfig->mServerId, pConfig->mMaxConnectNum, pConfig->mServerPort);
			return false;
		}

		if (pConfig->mHttpPort > 0)
		{
			unlinkId = (uint32_t)FindModule<NFIHttpServerModule>()->InitServer(NF_ST_MASTER, pConfig->mHttpPort);
			if (unlinkId == 0)
			{
				NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Open Http Port:{} Failed!", pConfig->mHttpPort);
				return false;
			}
			NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Open Http Port:{} Success!", pConfig->mHttpPort);
		}

		bool ret = FindModule<NFIAsyMysqlModule>()->AddMysqlServer(NF_ST_MASTER, pConfig->mMysqlIp, pConfig->mMysqlPort, pConfig->mMysqlDbName, pConfig->mMysqlUser, pConfig->mMysqlPassword);
		if (ret == false)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "NFIAsyMysqlModule AddMysqlServer failed!");
			return false;
		}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Master Server config!");
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

void NFCMasterServerModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == NF_MASTER_TIMER_SAVE_SERVER_DATA)
	{
		SaveServerDataToDB();
	}
}

void NFCMasterServerModule::SendMsgToServer(uint32_t serverId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	NF_SHARE_PTR<NFServerData> pServerData = GetServerByServerId(serverId);
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID);
		}
	}
}

void NFCMasterServerModule::SendMsgToAllServer(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	NF_SHARE_PTR<NFServerData> pServerData = nullptr;
	pServerData = mWorldMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID);
		}
		pServerData = mWorldMap.Next();
	}

	pServerData = mProxyMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID);
		}
		pServerData = mProxyMap.Next();
	}

	pServerData = mGameMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID);
		}
		pServerData = mGameMap.Next();
	}

	pServerData = mLoginMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID);
		}
		pServerData = mLoginMap.Next();
	}
}

NF_SHARE_PTR<NFServerData> NFCMasterServerModule::GetServerByServerId(uint32_t serverId)
{
	NF_SHARE_PTR<NFServerData> pServerData = nullptr;
	pServerData = mWorldMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	pServerData = mProxyMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	pServerData = mGameMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	pServerData = mLoginMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	return nullptr;
}

void NFCMasterServerModule::SaveServerDataToDB()
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		NFMsg::ServerInfoReportList xMsg;
		NFMsg::ServerInfoReport* pData = xMsg.add_server_list();
		pData->set_server_id(pConfig->mServerId);
		pData->set_server_name(pConfig->mServerName);
		pData->set_server_ip(pConfig->mServerIp);
		pData->set_server_port(pConfig->mServerPort);
		pData->set_server_http_port(pConfig->mHttpPort);
		pData->set_server_type(pConfig->mServerType);
		pData->set_server_max_online(pConfig->mMaxConnectNum);
		pData->set_server_state(NFMsg::EST_NARMAL);

		NFIMonitorModule* pMonitorModule = (NFIMonitorModule*)m_pPluginManager->FindModule(typeid(NFIMonitorModule).name());
		if (pMonitorModule)
		{
			const NFSystemInfo& systemInfo = pMonitorModule->GetSystemInfo();

			pData->set_system_info(systemInfo.GetOsInfo().mOsDescription);
			pData->set_total_mem(systemInfo.GetMemInfo().mTotalMem);
			pData->set_free_mem(systemInfo.GetMemInfo().mFreeMem);
			pData->set_used_mem(systemInfo.GetMemInfo().mUsedMem);

			pData->set_proc_cpu(systemInfo.GetProcessInfo().mCpuUsed);
			pData->set_proc_mem(systemInfo.GetProcessInfo().mMemUsed);
			pData->set_proc_thread(systemInfo.GetProcessInfo().mThreads);
			pData->set_proc_name(systemInfo.GetProcessInfo().mName);
			pData->set_proc_cwd(systemInfo.GetProcessInfo().mCwd);
			pData->set_proc_pid(systemInfo.GetProcessInfo().mPid);
		}

		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = *pData;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserver, pData->server_id());

		NFMsg::db_query_server_detail dbserverDetail;
		dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
		dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
		dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
		dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
		dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
		dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
		dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
		dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
		dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pData->server_id());
	}

	NF_SHARE_PTR<NFServerData> pServerData = nullptr;
	pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserver, pServerData->mServerInfo.server_id());

		NFMsg::db_query_server_detail dbserverDetail;
		dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
		dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
		dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
		dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
		dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
		dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
		dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
		dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
		dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mWorldMap.Next();
	}

	pServerData = mProxyMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserver, pServerData->mServerInfo.server_id());

		NFMsg::db_query_server_detail dbserverDetail;
		dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
		dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
		dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
		dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
		dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
		dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
		dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
		dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
		dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mProxyMap.Next();
	}

	pServerData = mGameMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserver, pServerData->mServerInfo.server_id());

		NFMsg::db_query_server_detail dbserverDetail;
		dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
		dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
		dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
		dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
		dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
		dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
		dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
		dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
		dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mGameMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	int nServerID = 0;
	pServerData = mLoginMap.First();
	while (pServerData)
	{

		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserver, pServerData->mServerInfo.server_id());

		NFMsg::db_query_server_detail dbserverDetail;
		dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
		dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
		dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
		dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
		dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
		dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
		dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
		dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
		dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mLoginMap.Next();
	}
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
	NF_SHARE_PTR<NFServerData> pServerData = nullptr;
	pServerData = mWorldMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;
			pServerData->mServerInfo.set_proc_cpu(0);
			pServerData->mServerInfo.set_proc_mem(0);
			pServerData->mServerInfo.set_proc_thread(0);
			pServerData->mServerInfo.set_proc_pid(0);
			NFMsg::db_query_server dbserver;
			*dbserver.mutable_db_fields() = pServerData->mServerInfo;
			dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserver);

			NFMsg::db_query_server_detail dbserverDetail;
			dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
			dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
			dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
			dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
			dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
			dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
			dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
			dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
			dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the world server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			return;
		}

		pServerData = mWorldMap.Next();
	}

	pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;
			pServerData->mServerInfo.set_proc_cpu(0);
			pServerData->mServerInfo.set_proc_mem(0);
			pServerData->mServerInfo.set_proc_thread(0);
			pServerData->mServerInfo.set_proc_pid(0);
			NFMsg::db_query_server dbserver;
			*dbserver.mutable_db_fields() = pServerData->mServerInfo;
			dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserver);

			NFMsg::db_query_server_detail dbserverDetail;
			dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
			dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
			dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
			dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
			dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
			dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
			dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
			dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
			dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the proxy server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			return;
		}

		pServerData = mProxyMap.Next();
	}

	pServerData = mGameMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			pServerData->mServerInfo.set_server_state(NFMsg::EST_CRASH);
			pServerData->mUnlinkId = 0;
			pServerData->mServerInfo.set_proc_cpu(0);
			pServerData->mServerInfo.set_proc_mem(0);
			pServerData->mServerInfo.set_proc_thread(0);
			pServerData->mServerInfo.set_proc_pid(0);
			NFMsg::db_query_server dbserver;
			*dbserver.mutable_db_fields() = pServerData->mServerInfo;
			dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserver);

			NFMsg::db_query_server_detail dbserverDetail;
			dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
			dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
			dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
			dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
			dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
			dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
			dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
			dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
			dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the game server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			return;
		}

		pServerData = mGameMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	int nServerID = 0;
	pServerData = mLoginMap.First();
	while (pServerData)
	{
		if (unLinkId == pServerData->mUnlinkId)
		{
			nServerID = pServerData->mServerInfo.server_id();
			pServerData->mServerInfo.set_proc_cpu(0);
			pServerData->mServerInfo.set_proc_mem(0);
			pServerData->mServerInfo.set_proc_thread(0);
			pServerData->mServerInfo.set_proc_pid(0);
			NFMsg::db_query_server dbserver;
			*dbserver.mutable_db_fields() = pServerData->mServerInfo;
			dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserver);

			NFMsg::db_query_server_detail dbserverDetail;
			dbserverDetail.mutable_db_fields()->set_server_id(dbserver.mutable_db_fields()->server_id());
			dbserverDetail.mutable_db_fields()->set_server_cur_online(dbserver.mutable_db_fields()->server_cur_online());
			dbserverDetail.mutable_db_fields()->set_server_state(dbserver.mutable_db_fields()->server_state());
			dbserverDetail.mutable_db_fields()->set_total_mem(dbserver.mutable_db_fields()->total_mem());
			dbserverDetail.mutable_db_fields()->set_used_mem(dbserver.mutable_db_fields()->used_mem());
			dbserverDetail.mutable_db_fields()->set_proc_cpu(dbserver.mutable_db_fields()->proc_cpu());
			dbserverDetail.mutable_db_fields()->set_proc_mem(dbserver.mutable_db_fields()->proc_mem());
			dbserverDetail.mutable_db_fields()->set_proc_thread(dbserver.mutable_db_fields()->proc_thread());
			dbserverDetail.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
			dbserverDetail.mutable_db_fields()->set_last_time(NFGetSecondTime());
			FindModule<NFIAsyMysqlModule>()->Update(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the login server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
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
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Login Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnLoginServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mLoginMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Login Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnLoginServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		//NFLogInfo("Login Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

//游戏服务器注册协议回调
void NFCMasterServerModule::OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);
		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mGameMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Game Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		//NFLogInfo("Game Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);
		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Proxy Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		//NFLogInfo("Proxy Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);
		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "World Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnWorldServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mWorldMap.RemoveElement(xData.server_id());

		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "World Server UnRegister Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnWorldServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		//NFLogInfo("World Server Refresh Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", xData.server_name(), xData.server_id(), xData.server_ip(), xData.server_port());
	}
}

void NFCMasterServerModule::OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

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

				if (xData.server_ip().empty())
				{
					std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
					pServerData->mServerInfo.set_server_ip(ip);
				}
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

				if (xData.server_ip().empty())
				{
					std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
					pServerData->mServerInfo.set_server_ip(ip);
				}
			}
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

				if (xData.server_ip().empty())
				{
					std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
					pServerData->mServerInfo.set_server_ip(ip);
				}
			}
			//SynReportToLogin(pServerData);
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

				if (xData.server_ip().empty())
				{
					std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
					pServerData->mServerInfo.set_server_ip(ip);
				}
			}
		}
		break;
		}
	}
}

bool NFCMasterServerModule::HttpHandleHttpGm(uint32_t linkId, const NFHttpHandle& req)
{
	std::string jsonMsg = req.bodySlice.ToString();
	NFMsg::http_msg_gm gm;
	NFProtobufCommon::JsonStringToMessage(jsonMsg, gm);

	if (gm.server_id() == 0)
	{
		SendMsgToAllServer(EGMI_STS_GM_MSG, gm, 0);
		NFEventMgr::Instance()->FireExecute(NFEVENT_GM, 0, 0, gm);
	}
	else if (gm.server_id() == 1)
	{
		NFEventMgr::Instance()->FireExecute(NFEVENT_GM, 0, 0, gm);
	}
	else
	{
		SendMsgToServer(gm.server_id(), EGMI_STS_GM_MSG, gm, 0);
	}

	NFMsg::http_msg_gm_ret msg_gm_ret;
	msg_gm_ret.set_code("success");
	msg_gm_ret.set_msg("success");
	std::string strMsg;
	NFProtobufCommon::MessageToJsonString(msg_gm_ret, strMsg);

	FindModule<NFIHttpServerModule>()->ResponseMsg(NF_ST_MASTER, req, strMsg, NFWebStatus::WEB_OK, "OK");

	return true;
}

void NFCMasterServerModule::SynServerToOthers(NF_SHARE_PTR<NFServerData> pServerData)
{
	NFMsg::ServerInfoReportList xData;

	NFMsg::ServerInfoReportList xSelfData;
	NFMsg::ServerInfoReport* pSelfData = xSelfData.add_server_list();
	*pSelfData = pServerData->mServerInfo;

	do 
	{
		NF_SHARE_PTR<NFServerData> pLoginServer = mLoginMap.First();
		while (pLoginServer)
		{
			if (pServerData->mServerInfo.server_id() != pLoginServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pLoginServer->mServerInfo;

				FindModule<NFINetServerModule>()->SendToServerByPB(pLoginServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN, xSelfData, 0);
			}

			pLoginServer = mLoginMap.Next();
		}
	} while(0);

	do 
	{
		NF_SHARE_PTR<NFServerData> pWorldServer = mWorldMap.First();
		while (pWorldServer)
		{
			if (pServerData->mServerInfo.server_id() != pWorldServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pWorldServer->mServerInfo;

				FindModule<NFINetServerModule>()->SendToServerByPB(pWorldServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD, xSelfData, 0);
			}

			pWorldServer = mWorldMap.Next();
		}
	} while(0);

	do 
	{
		NF_SHARE_PTR<NFServerData> pGameServer = mGameMap.First();
		while (pGameServer)
		{
			if (pServerData->mServerInfo.server_id() != pGameServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pGameServer->mServerInfo;

				FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_GAME, xSelfData, 0);
			}

			pGameServer = mGameMap.Next();
		}
	} while(0);

	do 
	{
		NF_SHARE_PTR<NFServerData> pProxyServer = mProxyMap.First();
		while (pProxyServer)
		{
			if (pServerData->mServerInfo.server_id() != pProxyServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pProxyServer->mServerInfo;

				FindModule<NFINetServerModule>()->SendToServerByPB(pProxyServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY, xSelfData, 0);
			}

			pProxyServer = mProxyMap.Next();
		}
	} while(0);



	if (pServerData->mServerInfo.server_type() == NF_ST_GAME)
	{
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_GAME, xData, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_WORLD)
	{
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD, xData, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_PROXY)
	{
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY, xData, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_LOGIN)
	{
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN, xData, 0);
	}
}
