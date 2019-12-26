// -------------------------------------------------------------------------
//    @FileName         :    NFCCommonClient_MasterModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCCommonClient_MasterModule
//
// -------------------------------------------------------------------------

#include "NFCCommonClient_MasterModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include <NFComm/NFPluginModule/NFEventMgr.h>
#include <NFComm/NFPluginModule/NFEventDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFCpu.h"
#include "NFComm/NFPluginModule/NFIMonitorModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFMessageDefine/msg_gm.pb.h"
#include "NFMessageDefine/server_msg.pb.h"
#include "NFComm/NFCore/NFStringUtility.h"

NFCCommonClient_MasterModule::NFCCommonClient_MasterModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mServerType = NF_ST_NONE;
}

NFCCommonClient_MasterModule::~NFCCommonClient_MasterModule()
{
}

void NFCCommonClient_MasterModule::SetServerType(NF_SERVER_TYPES serverType)
{
	mServerType = serverType;
}

NF_SHARE_PTR<NFServerData> NFCCommonClient_MasterModule::GetMasterServerData()
{
	return m_pMasterServerData;
}

bool NFCCommonClient_MasterModule::Init()
{
	m_pMasterServerData = NF_SHARE_PTR<NFServerData>(NF_NEW NFServerData());
	return true;
}

bool NFCCommonClient_MasterModule::AfterInit()
{
	FindModule<NFINetClientModule>()->AddEventCallBack(NF_ST_MASTER, this, &NFCCommonClient_MasterModule::OnProxySocketEvent);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_MASTER, this, &NFCCommonClient_MasterModule::OnHandleOtherMessage);
	
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_SEND_OTHERS_TO_SERVER, this, &NFCCommonClient_MasterModule::OnHandleServerReport);

	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_HTTP_MSG, this, &NFCCommonClient_MasterModule::OnHandleHttpMsg);
	FindModule<NFINetClientModule>()->AddReceiveCallBack(NF_ST_MASTER, EGMI_STS_GM_MSG, this, &NFCCommonClient_MasterModule::OnHandleGmMsg);

	if (FindModule<NFILuaScriptModule>())
	{
		this->Subscribe(NFEVENT_LUA_ERROR_LOG, 0, 0, __FUNCTION__);
		this->Subscribe(NFEVENT_LUA_FINISH_LOAD, 0, 0, __FUNCTION__);
	}
	else
	{
		NFServerConfig* pConfig = NFServerCommon::GetServerConfig(m_pPluginManager, NF_ST_MASTER);
		if (pConfig)
		{
			//AddServer���Զ��������Ͽ�����ʱ��m_pMasterServerData->mUnlinkId������������
			m_pMasterServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_MASTER, pConfig->mServerIp, pConfig->mServerPort);
			m_pMasterServerData->mServerInfo.set_server_id(pConfig->mServerId);
			m_pMasterServerData->mServerInfo.set_server_ip(pConfig->mServerIp);
			m_pMasterServerData->mServerInfo.set_server_port(pConfig->mServerPort);
			m_pMasterServerData->mServerInfo.set_server_name(pConfig->mServerName);
			m_pMasterServerData->mServerInfo.set_server_type(pConfig->mServerType);
			m_pMasterServerData->mServerInfo.set_server_state(NFMsg::EST_NARMAL);
		}
		else
		{
			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Master Server config!");
			return false;
		}
	}

	return true;
}

bool NFCCommonClient_MasterModule::Execute()
{
	ServerReport();
	return true;
}

bool NFCCommonClient_MasterModule::BeforeShut()
{
	return true;
}

bool NFCCommonClient_MasterModule::Shut()
{
	return true;
}

void NFCCommonClient_MasterModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	if (nEvent == eMsgType_CONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Server:{} Server Connect Master Server Success!", GetServerName(mServerType));

		//���ӳɹ���������Ϸ������IP�Լ����ݸ����������
		RegisterServer();
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		NFLogDebug(NF_LOG_SERVER_CONNECT_SERVER, 0, "Server:{} Server DisConnect Master Server!", GetServerName(mServerType));

		FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_DISCONNECTED, mServerType, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
	}
}

void NFCCommonClient_MasterModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;
}

void NFCCommonClient_MasterModule::RegisterServer()
{
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, mServerType);
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

		FindModule<NFINetClientModule>()->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_NET_REGISTER_MASTER, xMsg, 0, 0);
	}

	FindModule<NFIServerNetEventModule>()->OnServerNetEvent(eMsgType_CONNECTED, mServerType, NF_ST_MASTER, m_pMasterServerData->mUnlinkId, m_pMasterServerData);
}

void NFCCommonClient_MasterModule::ServerReport()
{
	static uint64_t mLastReportTime = m_pPluginManager->GetNowTime();
	if (mLastReportTime + 10000 > m_pPluginManager->GetNowTime())
	{
		return;
	}

	mLastReportTime = m_pPluginManager->GetNowTime();

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, mServerType);
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
			pData->set_server_cur_online(systemInfo.GetUserCount());
		}

		if (m_pMasterServerData->mUnlinkId > 0 && pData->proc_cpu() > 0 && pData->proc_mem() > 0)
		{
			FindModule<NFINetClientModule>()->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_STS_SERVER_REPORT, xMsg, 0, 0);
		}
	}
}

void NFCCommonClient_MasterModule::OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	NFMsg::ServerInfoReportList xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);
}

void NFCCommonClient_MasterModule::OnHandleHttpMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	NFMsg::http_msg xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		std::vector<std::string> vecPath;
		NFStringUtility::Split(xMsg.cmd(), "/", &vecPath);
		if (vecPath.size() >= 2)
		{
			pLuaScriptModule->RunHttpRecvLuaFunc("LuaNFrame.DispatchMasterHttp", unLinkId, xMsg.request_id(), vecPath[0], vecPath[1], xMsg.data());
		}
	}
	else
	{
		NFLogWarning(NF_LOG_SERVER_NOT_HANDLE_MESSAGE, 0, "msg:{} not handled!", nMsgId);
	}
}

void NFCCommonClient_MasterModule::OnHandleGmMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	if (unLinkId != m_pMasterServerData->mUnlinkId) return;

	NFMsg::http_msg_gm xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, playerId, msg, nLen, xMsg);

	NFEventMgr::Instance()->FireExecute(NFEVENT_GM, 0, 0, xMsg);
}

void NFCCommonClient_MasterModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message)
{
	if (nEventID == NFEVENT_LUA_ERROR_LOG)
	{
		const NFMsg::ServerErrorLogMsg* msg_gm = dynamic_cast<const NFMsg::ServerErrorLogMsg*>(&message);
		if (msg_gm)
		{
			FindModule<NFINetClientModule>()->SendToServerByPB(m_pMasterServerData->mUnlinkId, EGMI_STS_ERROR_MSG, message, 0, 0);
		}
	}

	if (nEventID == NFEVENT_LUA_FINISH_LOAD)
	{
		NFServerConfig* pConfig = NFServerCommon::GetServerConfig(m_pPluginManager, NF_ST_MASTER);
		if (pConfig)
		{
			//AddServer���Զ��������Ͽ�����ʱ��m_pMasterServerData->mUnlinkId������������
			m_pMasterServerData->mUnlinkId = FindModule<NFINetClientModule>()->AddServer(NF_ST_MASTER, pConfig->mServerIp, pConfig->mServerPort);
			m_pMasterServerData->mServerInfo.set_server_id(pConfig->mServerId);
			m_pMasterServerData->mServerInfo.set_server_ip(pConfig->mServerIp);
			m_pMasterServerData->mServerInfo.set_server_port(pConfig->mServerPort);
			m_pMasterServerData->mServerInfo.set_server_name(pConfig->mServerName);
			m_pMasterServerData->mServerInfo.set_server_type(pConfig->mServerType);
			m_pMasterServerData->mServerInfo.set_server_state(NFMsg::EST_NARMAL);
		}
		else
		{
			NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Master Server config!");
		}
	}
}

