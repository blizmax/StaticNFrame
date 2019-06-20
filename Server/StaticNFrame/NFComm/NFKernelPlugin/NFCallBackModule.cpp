// -------------------------------------------------------------------------
//    @FileName         :    NFCCallBackModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-3-30
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFCallBackModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#define CALL_BACK_TIME_OUT_TIMER 1000		//»Øµ÷³¬Ê±

NFCCallBackModule::NFCCallBackModule(NFIPluginManager* p)
{
}

NFCCallBackModule::~NFCCallBackModule()
{
}

bool NFCCallBackModule::Init()
{
	//this->SetTimer(0, CALL_BACK_TIME_OUT_TIMER, INFINITY_CALL);
	return true;
}

bool NFCCallBackModule::Execute()
{
	return true;
}

bool NFCCallBackModule::Finalize()
{
	return true;
}

void NFCCallBackModule::HandleTimeout()
{

}

bool NFCCallBackModule::ProcessTimeout(const NFCallBack& back)
{
	NFLogWarning(NF_LOG_NET_PLUGIN, 0, "call back:{}ms, desc:{}", NFTime::Tick() - back.m_timeOut, back.m_desc);
	return true;
}

void NFCCallBackModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == 0)
	{
		HandleTimeout();
	}
}
