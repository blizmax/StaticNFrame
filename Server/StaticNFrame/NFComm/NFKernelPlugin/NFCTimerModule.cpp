// -------------------------------------------------------------------------
//    @FileName         :    {Filename}
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Module           :   {Filename}
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
	mTimerAxis.UpdateFix();
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
bool NFCTimerModule::SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj *handler, uint32_t nCallCount /*= INFINITY_CALL*/)
{
	return mTimerAxis.SetTimer(nTimerID, nInterVal, handler, nCallCount);
}

//�رն�ʱ��
bool NFCTimerModule::KillTimer(uint32_t nTimerID, NFTimerObj *handler)
{
	return mTimerAxis.KillTimer(nTimerID, handler);
}

//�ر����ж�ʱ��
bool NFCTimerModule::KillAllTimer(NFTimerObj *handler)
{
	return mTimerAxis.KillAllTimer(handler);
}

//���ù̶�ʱ��Ķ�ʱ��
bool NFCTimerModule::SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, NFTimerObj *handler, uint32_t nCallCount/* = INFINITY_CALL */)
{
	return mTimerAxis.SetFixTimer(nTimerID, nStartTime, nInterDays, handler, nCallCount);
}

//�رչ̶�ʱ�䶨ʱ��
bool NFCTimerModule::KillFixTimer(uint32_t nTimerID, NFTimerObj *handler)
{
	return mTimerAxis.KillFixTimer(nTimerID, handler);
}

//�ر����й̶�ʱ�䶨ʱ��
bool NFCTimerModule::KillAllFixTimer(NFTimerObj *handler)
{
	return mTimerAxis.KillAllFixTimer(handler);
}