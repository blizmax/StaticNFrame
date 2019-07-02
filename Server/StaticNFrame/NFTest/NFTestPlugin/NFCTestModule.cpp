// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "libgo/libgo/coroutine.h"
#include "libgo/libgo/libgo.h"
#include "NFComm/NFCore/NFTime.h"


NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

bool NFCTestModule::Init()
{
	//this->SetTimer(0, 1000, 0);
	return true;
}

void NFCTestModule::OnTimer(uint32_t nTimerID)
{
	static uint64_t startTime = 0;
	if (startTime == 0)
	{
		startTime = NFTime::Now().UnixMSec();
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "test {} msec", NFTime::Now().UnixMSec() - startTime);
		startTime = NFTime::Now().UnixMSec();
	}
	
}

bool NFCTestModule::AfterInit()
{
	return true;
}

void NFCTestModule::test(bool result)
{
	NFLogError(NF_LOG_SYSTEMLOG, 0, "result:{}, xxxxxxxxxxxx", result);
}


bool NFCTestModule::Execute()
{
	static bool flag = true;
	
	if (flag)
	{
		flag = false;
	}
	return true;
}

bool NFCTestModule::BeforeShut()
{
	return true;
}

bool NFCTestModule::Shut()
{
	return true;
}