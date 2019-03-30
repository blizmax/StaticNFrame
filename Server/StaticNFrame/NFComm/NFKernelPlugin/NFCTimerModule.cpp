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
#include "NFComm/NFPluginModule/NFTimerMgr.h"

NFCTimerModule::NFCTimerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	NFTimerMgr::Instance()->Init(this);
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

//���ö�ʱ��
bool NFCTimerModule::SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount /*= INFINITY_CALL*/)
{
	return mTimerAxis.SetTimer(nTimerID, nInterVal, handler, nCallCount);
}

//�رն�ʱ��
bool NFCTimerModule::KillTimer(uint32_t nTimerID, NFTimerObj* handler)
{
	return mTimerAxis.KillTimer(nTimerID, handler);
}

//�ر����ж�ʱ��
bool NFCTimerModule::KillAllTimer(NFTimerObj* handler)
{
	return mTimerAxis.KillAllTimer(handler);
}

//���ù̶�ʱ��Ķ�ʱ��
bool NFCTimerModule::SetClocker(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, NFTimerObj* handler, uint32_t nCallCount/* = INFINITY_CALL */)
{
	return mTimerAxis.SetClocker(nTimerID, nStartTime, nInterDays, handler, nCallCount);
}

bool NFCTimerModule::SetCalender(uint32_t nTimerID, const std::string& timeStr, NFTimerObj* handler, uint32_t nCallCount/* = INFINITY_CALL */)
{
	return mTimerAxis.SetCalender(nTimerID, timeStr, handler, nCallCount);
}