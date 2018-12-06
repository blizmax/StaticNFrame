// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestActorModule.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include <NFComm/NFPluginModule/NFIMysqlModule.h>
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFPluginModule/NFIMongoModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"


class NFLogTask : public NFTask
{
public:
	NFLogTask()
	{
		count = 0;
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess()
	{
		NFLogError("thread process........... pid:{}, thread_id:{}", NFGetPID(), ThreadId());
		count++;
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		if (count >= 10)
		{
			return NFTask::TPTASK_STATE_COMPLETED;
		}
		return TPTASK_STATE_CONTINUE_CHILDTHREAD;
	}

	std::atomic_int count;
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
	//NFITaskModule* pTaskModule = pPluginManager->FindModule<NFITaskModule>();

	//for(int i = 0; i < 10000; i++)
	//	pTaskModule->AddTask(new NFLogTask());

	NFIMongoModule* pMongoModule = pPluginManager->FindModule<NFIMongoModule>();

	pMongoModule->AddMongoServer(NF_ST_GAME, "45.32.39.90", 27017, "test");

	pMongoModule->IsExistCollection(NF_ST_GAME, "gaoyi");
	pMongoModule->IsExistCollection(NF_ST_GAME, "test");

	pMongoModule->CreateCollection(NF_ST_GAME, "gaoyi", "uid");
	pMongoModule->CreateCollection(NF_ST_GAME, "test", "uid");

	pMongoModule->DropCollection(NF_ST_GAME, "test");
	pMongoModule->DropCollection(NF_ST_GAME, "test");
	pMongoModule->DropCollection(NF_ST_GAME, "gaoyi");
	return true;
}

bool NFCTestActorModule::AfterInit()
{
	return true;
}

bool NFCTestActorModule::Execute()
{
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