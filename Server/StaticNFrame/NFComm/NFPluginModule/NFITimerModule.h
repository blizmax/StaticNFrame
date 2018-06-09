// -------------------------------------------------------------------------
//    @FileName         :    NFITimerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFIModule.h"
#include "NFComm/NFPluginModule/NFTimerAxis.h"

class NFITimerModule : public NFIModule
{
public:
	//设置定时器
	virtual bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL) = 0;

	//关闭定时器
	virtual bool KillTimer(uint32_t nTimerID, NFTimerObj* handler) = 0;

	//关闭所有定时器
	virtual bool KillAllTimer(NFTimerObj* handler) = 0;

	//设置固定时间的定时器
	virtual bool SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL) = 0;

	//关闭固定时间定时器
	virtual bool KillFixTimer(uint32_t nTimerID, NFTimerObj* handler) = 0;

	//关闭所有固定时间定时器
	virtual bool KillAllFixTimer(NFTimerObj* handler) = 0;
};

