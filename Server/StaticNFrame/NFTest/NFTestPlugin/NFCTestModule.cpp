// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFCore/NFTime.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"



NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

int testfunc()
{
	std::cout << "pre func" << std::endl;
	return 0;
}

int testfunc2()
{
	std::cout << "post func" << std::endl;
	return 0;
}

void testrun()
{
	std::cout << "run" << std::endl;
	NFSLEEP(1);
}

bool NFCTestModule::Init()
{
	SetTimer(0, 30, INFINITY_CALL);
	SetTimer(1, 10000, 1);
	NFEventLoop* loopThread = new NFEventLoop();
	NFEventLoopThreadPool* loopThreadPool = new NFEventLoopThreadPool(loopThread, 5);
	loopThreadPool->Start(true);
	loopThreadPool->Stop(true);
	return true;
}

void NFCTestModule::OnTimer(uint32_t nTimerID)
{

}

bool NFCTestModule::AfterInit()
{
	return true;
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