// -------------------------------------------------------------------------
//    @FileName         :    NFCTimerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#include "NFCTimerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

NFCTimerModule::NFCTimerModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

bool NFCTimerModule::Init()
{
	mTimerAxis.Init();
	return true;
}

bool NFCTimerModule::AfterInit()
{
	return true;
}

bool NFCTimerModule::Execute()
{
	mTimerAxis.Update();
	return true;
}

bool NFCTimerModule::BeforeShut()
{
	return true;
}

bool NFCTimerModule::Shut()
{
	mTimerAxis.UnInit();
	return true;
}

//设置定时器
bool NFCTimerModule::SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount /*= INFINITY_CALL*/)
{
	return mTimerAxis.SetTimer(nTimerID, nInterVal, handler, nCallCount);
}

//关闭定时器
bool NFCTimerModule::KillTimer(uint32_t nTimerID, NFTimerObj* handler)
{
	return mTimerAxis.KillTimer(nTimerID, handler);
}

//关闭所有定时器
bool NFCTimerModule::KillAllTimer(NFTimerObj* handler)
{
	return mTimerAxis.KillAllTimer(handler);
}

//设置固定时间的定时器
bool NFCTimerModule::SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, NFTimerObj* handler, uint32_t nCallCount/* = INFINITY_CALL */)
{
	return mTimerAxis.SetFixTimer(nTimerID, nStartTime, nInterDays, handler, nCallCount);
}