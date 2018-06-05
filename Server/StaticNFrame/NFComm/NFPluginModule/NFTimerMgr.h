// -------------------------------------------------------------------------
//    @FileName         :    NFTimerMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFITimerModule.h"
#include "NFComm/NFCore/NFSingleton.hpp"

class NFTimerMgr : public NFSingleton<NFTimerMgr>
{
public:
	NFTimerMgr();
	virtual ~NFTimerMgr();
public:
	virtual bool Init(NFITimerModule* pTimerModule);
	virtual void UnInit();
public:
	//设置定时器
	virtual bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL);

	//关闭定时器
	virtual bool KillTimer(uint32_t nTimerID, NFTimerObj* handler);

	//关闭所有定时器
	virtual bool KillAllTimer(NFTimerObj* handler);

	//设置固定时间的定时器
	virtual bool SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL);

	//关闭固定时间定时器
	virtual bool KillFixTimer(uint32_t nTimerID, NFTimerObj* handler);

	//关闭所有固定时间定时器
	virtual bool KillAllFixTimer(NFTimerObj* handler);
protected:
	NFITimerModule* m_pTimerModule;
};
