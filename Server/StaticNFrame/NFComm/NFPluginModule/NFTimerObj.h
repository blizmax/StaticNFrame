#pragma once
#include <stdint.h>

//定时器接口
struct NFTimerObj
{
public:
	NFTimerObj();
	virtual ~NFTimerObj();
public:
	virtual void OnTimer(uint32_t nTimerID) = 0;

	//设置定时器
	virtual bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, uint32_t nCallCount);

	//关闭定时器
	virtual bool KillTimer(uint32_t nTimerID);

	//关闭所有定时器
	virtual bool KillAllTimer();

	//设置固定时间的定时器
	virtual bool SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount);

	//关闭固定时间定时器
	virtual bool KillFixTimer(uint32_t nTimerID);

	//关闭所有固定时间定时器
	virtual bool KillAllFixTimer();
public:
	virtual void **GetTimerInfoPtr() { return &m_pTimerInfoPtr; }
	virtual void **GetFixTimerInfoPtr() { return &m_pFixTimerInfoPtr; }
private:
	void *m_pTimerInfoPtr;			//临时保存回调指针对应的定时器信息
	void *m_pFixTimerInfoPtr;		//临时保存回调指针对应的固定时间的定时器信息
};