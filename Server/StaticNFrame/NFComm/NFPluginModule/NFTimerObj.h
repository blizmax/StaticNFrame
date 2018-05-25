// -------------------------------------------------------------------------
//    @FileName         :    NFTimerObj.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once
#include <stdint.h>

struct NFTimerObj
{
public:
	NFTimerObj();
	virtual ~NFTimerObj();
public:
	virtual void OnTimer(uint32_t nTimerID) = 0;

	virtual bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, uint32_t nCallCount);

	virtual bool KillTimer(uint32_t nTimerID);

	virtual bool KillAllTimer();

	virtual bool SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount);

	virtual bool KillFixTimer(uint32_t nTimerID);

	virtual bool KillAllFixTimer();
public:
	virtual void** GetTimerInfoPtr()
	{
		return &m_pTimerInfoPtr;
	}

	virtual void** GetFixTimerInfoPtr()
	{
		return &m_pFixTimerInfoPtr;
	}

private:
	void* m_pTimerInfoPtr;
	void* m_pFixTimerInfoPtr;
};

