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
#include "NFComm/NFCore/NFTime.h"
#include "NFThreadPool.h"

NFThreadPool* g_threadPool = nullptr;

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
	g_threadPool = new NFThreadPool(1);
	std::vector< std::future<int> > results;
	for (int i = 0; i < 8; ++i) {
		results.emplace_back(
			g_threadPool->enqueue([i] {
			std::cout << "hello " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "world " << i << std::endl;
			return i * i;
		})
		);
	}

	for (auto && result : results)
		std::cout << result.get() << ' ';
	std::cout << std::endl;
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
	delete g_threadPool;
	return true;
}

bool NFCTestModule::Shut()
{
	return true;
}