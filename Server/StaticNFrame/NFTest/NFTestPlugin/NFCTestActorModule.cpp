// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestActorModule.h"
#include "NFComm/NFPluginModule/NFTask.h"

static std::map<int, int> testMap;

class DBTask : public NFTask
{
public:
	virtual bool db_thread_process()
	{
		int j = 0;
		for (int i = 0; i < 100; i++)
		{
			j += i;
			testMap[i] = j;
		}
		return true;
	}
};

class DBXTask : public NFTask
{
public:
	virtual bool db_thread_process()
	{
		int j = 0;
		for (int i = 0; i < 100; i++)
		{
			j += i;
			testMap[i] = j;
		}
		NFDBActorMgr::Instance()->AddTask(new DBTask());
		return true;
	}
};

NFCTestActorModule::NFCTestActorModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestActorModule::~NFCTestActorModule()
{
}

bool NFCTestActorModule::Init()
{
	return true;
}

bool NFCTestActorModule::AfterInit()
{
	return true;
}

bool NFCTestActorModule::Execute()
{
	for (int i = 0; i < 10000; i++)
	{
		NFDBActorMgr::Instance()->AddTask(new DBXTask());
	}
	NFDBActorMgr::Instance()->OnMainThreadTick();
	return true;
}

bool NFCTestActorModule::BeforeShut()
{
	return true;
}

bool NFCTestActorModule::Shut()
{
	return true;
}