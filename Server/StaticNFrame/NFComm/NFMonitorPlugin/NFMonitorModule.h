// -------------------------------------------------------------------------
//    @FileName         :    NFMonitorModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIMonitorModule.h"
#include "NFComm/NFPluginModule/NFSystemInfo.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"

/**
* @brief 服务器监控模块
*/
class NFCMonitorModule : public NFIMonitorModule, public NFTimerObj
{
public:
	NFCMonitorModule(NFIPluginManager* p);

	virtual ~NFCMonitorModule();
public:
	virtual bool Init();

	virtual bool Execute();

	virtual bool Finalize();

	/**
	* @brief 处理定时器功能
	*
	* @return void
	*/
	virtual void OnTimer(uint32_t nTimerID);
private:
	NFSystemInfo mSystemInfo;
};
