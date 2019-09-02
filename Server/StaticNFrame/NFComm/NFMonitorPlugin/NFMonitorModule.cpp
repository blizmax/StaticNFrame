// -------------------------------------------------------------------------
//    @FileName         :    NFMonitorModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#include "NFMonitorModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFMessageDefine/msg_gm.pb.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFCore/NFStringUtility.h"

enum MonitorTimerEnum
{
	MonitorTimer_SYSTEMINFO = 1,
};

#define MONITOR_TIMER_SYSTEIMINFO_INTERNAL_TIME 10000		//10s

NFCMonitorModule::NFCMonitorModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCMonitorModule::~NFCMonitorModule()
{

}

bool NFCMonitorModule::Init()
{
	this->Subscribe(NFEVENT_GM, 0, 0, __FUNCTION__);
	this->SetTimer(MonitorTimer_SYSTEMINFO, MONITOR_TIMER_SYSTEIMINFO_INTERNAL_TIME, INFINITY_CALL);
	mSystemInfo.Init();
	return true;
}

bool NFCMonitorModule::Execute()
{
	return true;
}

bool NFCMonitorModule::Finalize()
{
	return true;
}

/**
* @brief 处理定时器功能
*
* @return void
*/
void NFCMonitorModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == MonitorTimer_SYSTEMINFO)
	{
		mSystemInfo.CountSystemInfo();
		NFLogInfo(NF_LOG_MONITOR_PLUGIN, 0, "cpu:%{}, mem:{}M", mSystemInfo.GetProcessInfo().mCpuUsed, mSystemInfo.GetProcessInfo().mMemUsed /(double)1024 / (double)1024);
	}
}

/**
* @brief 处理事件
*
* @return void
*/
void NFCMonitorModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message)
{
	if (nEventID == NFEVENT_GM)
	{
		const NFMsg::http_msg_gm* msg_gm = dynamic_cast<const NFMsg::http_msg_gm*>(&message);
		if (msg_gm)
		{
			if (msg_gm->cmd() == "reload")
			{
				m_pPluginManager->OnReloadPlugin();
			}
			else if (msg_gm->cmd() == "exit")
			{
				m_pPluginManager->SetExitApp(true);
			}
			else if (msg_gm->cmd() == "profiler")
			{
				m_pPluginManager->SetOpenProfiler(!m_pPluginManager->IsOpenProfiler());
			}
			else if (msg_gm->cmd() == "dynamic")
			{
				m_pPluginManager->DynamicLoadPluginLibrary(msg_gm->data());
			}
			else if (msg_gm->cmd() == "hotfixalllua")
			{
				NFILuaScriptModule* pLuaModule = FindModule<NFILuaScriptModule>();
				if (pLuaModule)
				{
					pLuaModule->ReloadAllLuaFiles();
				}
			}
			else if (msg_gm->cmd() == "hotfixlua")
			{
				NFILuaScriptModule* pLuaModule = FindModule<NFILuaScriptModule>();
				if (pLuaModule)
				{
					std::string data = msg_gm->data();
					NFStringUtility::Trim(data);
					std::vector<std::string> vecStr;
					if (data.find(';') != std::string::npos)
					{
						NFStringUtility::Split(vecStr, data, ";");
					}
					else
					{
						NFStringUtility::Split(vecStr, data, "|");
					}

					for (size_t i = 0; i < vecStr.size(); i++)
					{
						NFStringUtility::Trim(vecStr[i]);
					}
					
					pLuaModule->ReloadLuaFiles(vecStr);
				}
			}
		}
	}
}

const NFSystemInfo& NFCMonitorModule::GetSystemInfo() const
{
	return mSystemInfo;
}