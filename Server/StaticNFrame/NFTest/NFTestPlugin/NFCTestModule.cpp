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

NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

bool NFCTestModule::Init()
{
	return true;
}

bool NFCTestModule::AfterInit()
{
	return true;
}

void foo()
{
	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "foo.........");
}

bool NFCTestModule::Execute()
{
	static bool flag = true;
	if (flag)
	{
		flag = false;
		go []
		{
			NFLogInfo(NF_LOG_SYSTEMLOG, 0, "{}", co::CoDebugger::getInstance().GetAllInfo().c_str());
		};

		std::thread([] {co_sleep(200); co_sched.Stop(); }).detach();

		co_sched.Start();

		NFLogInfo(NF_LOG_SYSTEMLOG, 0, "go.........");
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