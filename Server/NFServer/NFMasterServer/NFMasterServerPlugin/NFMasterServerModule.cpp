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
#include "NFComm/NFPluginModule/NFIMysqlModule.h"

#define NF_MASTER_TIMER_SAVE_SERVER_DATA 0
#define NF_MASTER_TIMER_SAVE_SERVER_DATA_TIME 30000
#define NF_MASTER_TIMER_CLEAR_SERVER_DATA 1
#define NF_MASTER_TIMER_CLEAR_SERVER_DATA_TIME 600000

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
	this->SetFixTimer(NF_MASTER_TIMER_CLEAR_SERVER_DATA, 0, 8 * 60 * 60, INFINITY_CALL);
	//this->SetTimer(NF_MASTER_TIMER_CLEAR_SERVER_DATA, NF_MASTER_TIMER_CLEAR_SERVER_DATA_TIME, INFINITY_CALL);

	FindModule<NFIHttpServerModule>()->AddRequestHandler(NF_ST_MASTER, "/GM", NFHttpType::NF_HTTP_REQ_POST, this, &NFCMasterServerModule::HttpHandleHttpGm);
	FindModule<NFIHttpServerModule>()->AddRequestHandler(NF_ST_MASTER, NFHttpType::NF_HTTP_REQ_POST, this, &NFCMasterServerModule::HttpHandleHttpMsg);

	FindModule<NFINetServerModule>()->AddEventCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnProxySocketEvent);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, this, &NFCMasterServerModule::OnHandleOtherMessage);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnLoginServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnWorldServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnProxyServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnGameServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOCATION_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnLocationServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOG_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnLogServerRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WEB_TO_MASTER_REGISTER, this, &NFCMasterServerModule::OnWebServerRegisterProcess);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnLoginServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnWorldServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnProxyServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnGameServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOCATION_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnLocationServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOG_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnLogServerUnRegisterProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WEB_TO_MASTER_UNREGISTER, this, &NFCMasterServerModule::OnWebServerUnRegisterProcess);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOGIN_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnLoginServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WORLD_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnWorldServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_PROXY_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnProxyServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_GAME_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnGameServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOCATION_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnLocationServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_LOG_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnLogServerRefreshProcess);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_WEB_TO_MASTER_REFRESH, this, &NFCMasterServerModule::OnWebServerRefreshProcess);
	
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_SERVER_REPORT, this, &NFCMasterServerModule::OnServerReport);
	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_HTTP_MSG_RET, this, &NFCMasterServerModule::OnServerHttpMsgRet);

	FindModule<NFINetServerModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_ERROR_MSG, this, &NFCMasterServerModule::OnServerErrorMsg);
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_MASTER);
	if (pConfig)
	{
		if (!m_pPluginManager->IsLoadAllServer())
		{
			if (pConfig->mServerType != NF_ST_MASTER)
			{
				NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
				exit(0);
			}
		}
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

		ret = FindModule<NFIMysqlModule>()->AddMysqlServer(NF_ST_MASTER, pConfig->mMysqlIp, pConfig->mMysqlPort, pConfig->mMysqlDbName, pConfig->mMysqlUser, pConfig->mMysqlPassword);
		if (ret == false)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "NFIMysqlModule AddMysqlServer failed!");
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
#if NF_PLATFORM == NF_PLATFORM_LINUX
		SaveServerDataToDB();
#endif
	}
	else if (nTimerID == NF_MASTER_TIMER_CLEAR_SERVER_DATA)
	{
#if NF_PLATFORM == NF_PLATFORM_LINUX
		ClearServerDataToDB();
#endif
	}
}

void NFCMasterServerModule::SendMsgToServer(uint32_t serverId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID)
{
	NF_SHARE_PTR<NFServerData> pServerData = GetServerByServerId(serverId);
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
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
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mWorldMap.Next();
	}

	pServerData = mProxyMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mProxyMap.Next();
	}

	pServerData = mGameMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mGameMap.Next();
	}

	pServerData = mLoginMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mLoginMap.Next();
	}

	pServerData = mLocationMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mLocationMap.Next();
	}

	pServerData = mLogMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mLogMap.Next();
	}

	pServerData = mWebMap.First();
	if (pServerData)
	{
		if (pServerData->mServerInfo.server_state() != NFMsg::EST_CRASH)
		{
			FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, nMsgID, xData, nPlayerID, 0);
		}
		pServerData = mWebMap.Next();
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

	pServerData = mLocationMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	pServerData = mLogMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	pServerData = mWebMap.GetElement(serverId);
	if (pServerData)
	{
		return pServerData;
	}

	return nullptr;
}

void NFCMasterServerModule::ClearServerDataToDB()
{
	uint64_t now = NFGetSecondTime() - 24 * 60 * 60;
	FindModule<NFIAsyMysqlModule>()->ExecuteOne("delete from dy_server_detail where last_time < " + NFCommon::tostr(now));
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

		NFIMonitorModule* pMonitorModule = m_pPluginManager->FindModule<NFIMonitorModule>();
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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pData->server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pData->server_id());
	}

	NF_SHARE_PTR<NFServerData> pServerData = nullptr;
	pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mWorldMap.Next();
	}

	pServerData = mProxyMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mProxyMap.Next();
	}

	pServerData = mGameMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mGameMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mLoginMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mLoginMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mLocationMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mLocationMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mLogMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mLogMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mWebMap.First();
	while (pServerData)
	{
		NFMsg::db_query_server dbserver;
		*dbserver.mutable_db_fields() = pServerData->mServerInfo;
		dbserver.mutable_db_fields()->set_last_date(NFDateTime::Now().GetDbTimeString());
		dbserver.mutable_db_fields()->set_last_time(NFGetSecondTime());
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver, pServerData->mServerInfo.server_id());

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
		FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

		pServerData = mWebMap.Next();
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

void NFCMasterServerModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
	NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "other message not handled:playerId:{},msgId:{},ip:{}", playerId, nMsgId, ip);
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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the game server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());

			return;
		}

		pServerData = mGameMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mLoginMap.First();
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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the login server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
			break;
		}

		pServerData = mLoginMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mLocationMap.First();
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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the location server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
			break;
		}

		pServerData = mLocationMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mLogMap.First();
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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the log server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
			break;
		}

		pServerData = mLogMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	pServerData = mWebMap.First();
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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserver);

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
			FindModule<NFIAsyMysqlModule>()->UpdateOne(dbserverDetail, pServerData->mServerInfo.server_id());

			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "the web server disconnect from master server, serverName:{}, serverId:{}, serverIp:{}, serverPort:{}"
				, pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
			break;
		}

		pServerData = mWebMap.Next();
	}
}

void NFCMasterServerModule::OnLoginServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnLoginServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnLoginServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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
void NFCMasterServerModule::OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnWorldServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnWorldServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCMasterServerModule::OnServerHttpMsgRet(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	std::string strMsg(msg, nLen);
	uint32_t requestId = (uint32_t)playerId;

	FindModule<NFIHttpServerModule>()->ResponseMsg(NF_ST_MASTER, requestId, strMsg, NFWebStatus::WEB_OK, "OK");
}

void NFCMasterServerModule::OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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
		case NF_SERVER_TYPES::NF_ST_LOCATION:
		{
			pServerData = mLocationMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mLocationMap.AddElement(xData.server_id(), pServerData);
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
		case NF_SERVER_TYPES::NF_ST_LOG:
		{
			pServerData = mLogMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mLogMap.AddElement(xData.server_id(), pServerData);
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
		case NF_SERVER_TYPES::NF_ST_WEB:
		{
			pServerData = mWebMap.GetElement(xData.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<NFServerData>(new NFServerData());
				mWebMap.AddElement(xData.server_id(), pServerData);
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

bool NFCMasterServerModule::HttpHandleHttpMsg(uint32_t linkId, const NFIHttpHandle& req)
{
	std::string jsonMsg = req.GetBody();
	NFMsg::http_msg msg;
	msg.set_cmd(req.GetPath());
	msg.set_data(jsonMsg);
	msg.set_request_id(req.GetRequestId());

	SendMsgToAllServer(EGMI_STS_HTTP_MSG, msg, 0);

	return true;
}

bool NFCMasterServerModule::HttpHandleHttpGm(uint32_t linkId, const NFIHttpHandle& req)
{
	std::string jsonMsg = req.GetBody();
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

				//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send Server To LoginServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
				FindModule<NFINetServerModule>()->SendToServerByPB(pLoginServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN, xSelfData, 0, 0);
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

				//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send Server To WorldServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
				FindModule<NFINetServerModule>()->SendToServerByPB(pWorldServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD, xSelfData, 0, 0);
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

				//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send Server To GameServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
				FindModule<NFINetServerModule>()->SendToServerByPB(pGameServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_GAME, xSelfData, 0, 0);
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

				//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send Server To ProxyServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
				FindModule<NFINetServerModule>()->SendToServerByPB(pProxyServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY, xSelfData, 0, 0);
			}

			pProxyServer = mProxyMap.Next();
		}
	} while(0);

	do
	{
		NF_SHARE_PTR<NFServerData> pLocationServer = mLocationMap.First();
		while (pLocationServer)
		{
			if (pServerData->mServerInfo.server_id() != pLocationServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pLocationServer->mServerInfo;

				//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send Server To ProxyServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
				FindModule<NFINetServerModule>()->SendToServerByPB(pLocationServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOCATION, xSelfData, 0, 0);
			}

			pLocationServer = mLocationMap.Next();
		}
	} while (0);

	do
	{
		NF_SHARE_PTR<NFServerData> pLogServer = mLogMap.First();
		while (pLogServer)
		{
			if (pServerData->mServerInfo.server_id() != pLogServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pLogServer->mServerInfo;

				FindModule<NFINetServerModule>()->SendToServerByPB(pLogServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOG, xSelfData, 0, 0);
			}

			pLogServer = mLogMap.Next();
		}
	} while (0);


	do
	{
		NF_SHARE_PTR<NFServerData> pWebServer = mWebMap.First();
		while (pWebServer)
		{
			if (pServerData->mServerInfo.server_id() != pWebServer->mServerInfo.server_id())
			{
				NFMsg::ServerInfoReport* pData = xData.add_server_list();
				*pData = pWebServer->mServerInfo;

				FindModule<NFINetServerModule>()->SendToServerByPB(pWebServer->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_WEB, xSelfData, 0, 0);
			}

			pWebServer = mWebMap.Next();
		}
	} while (0);

	if (pServerData->mServerInfo.server_type() == NF_ST_GAME)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to GameServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_GAME, xData, 0, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_WORLD)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to WorldServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD, xData, 0, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_PROXY)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to ProxyServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY, xData, 0, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_LOGIN)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to LoginServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN, xData, 0, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_LOCATION)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to LoginServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOCATION, xData, 0, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_LOG)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to LoginServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_LOG, xData, 0, 0);
	}
	else if (pServerData->mServerInfo.server_type() == NF_ST_WEB)
	{
		//NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Master Server Send others to LoginServer, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
		FindModule<NFINetServerModule>()->SendToServerByPB(pServerData->mUnlinkId, EGMI_NET_MASTER_SEND_OTHERS_TO_WEB, xData, 0, 0);
	}
}

void NFCMasterServerModule::OnServerErrorMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerErrorLogMsg xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

#if NF_PLATFORM == NF_PLATFORM_LINUX
	static uint32_t error_log_id = 0;
	if (error_log_id == 0)
	{
		std::map<std::string, std::string> result;
		FindModule<NFIMysqlModule>()->ExecuteOne("select max(id) from dy_error_msg;", result);
		error_log_id = NFCommon::strto<uint32_t>(result["max(id)"]);
	}
	error_log_id++;
	//std::string sqlcase = "insert into dy_error_msg (playerid, funclog, errorlog) values (" + NFCommon::tostr(xMsg.player_id()) + ",'" + xMsg.func_log() + "','" + xMsg.error_log() + "');";
	
	std::map<std::string, std::string> data;
	data.emplace("playerid", NFCommon::tostr(xMsg.player_id()));
	data.emplace("funclog", xMsg.func_log());
	data.emplace("errorlog", xMsg.error_log());
	data.emplace("count", NFCommon::tostr(xMsg.count()));
	FindModule<NFIAsyMysqlModule>()->UpdateOne("dy_error_msg", "id", NFCommon::tostr(error_log_id), data);
#endif
}

//Location服务器注册协议回调
void NFCMasterServerModule::OnLocationServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mLocationMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mLocationMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);
		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Location Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnLocationServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

void NFCMasterServerModule::OnLocationServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

//Log服务器注册协议回调
void NFCMasterServerModule::OnLogServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mLogMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mLogMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);
		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Log Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnLogServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

void NFCMasterServerModule::OnLogServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

//Web服务器注册协议回调
void NFCMasterServerModule::OnWebServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<NFServerData> pServerData = mWebMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
			mWebMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->mUnlinkId = unLinkId;
		pServerData->mServerInfo = xData;

		if (xData.server_ip().empty())
		{
			std::string ip = FindModule<NFINetServerModule>()->GetLinkIp(unLinkId);
			pServerData->mServerInfo.set_server_ip(ip);
		}

		SynServerToOthers(pServerData);
		NFLogInfo(NF_LOG_SERVER_CONNECT_SERVER, 0, "Web Server Register Master Server Success, serverName:{}, serverId:{}, ip:{}, port:{}", pServerData->mServerInfo.server_name(), pServerData->mServerInfo.server_id(), pServerData->mServerInfo.server_ip(), pServerData->mServerInfo.server_port());
	}
}

void NFCMasterServerModule::OnWebServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}

void NFCMasterServerModule::OnWebServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{

}
