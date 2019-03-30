// -------------------------------------------------------------------------
//    @FileName         :    NFMonitorModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#include "NFMonitorModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

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
void NFCMonitorModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{

}