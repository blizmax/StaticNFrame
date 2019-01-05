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
#include "NFComm/NFCore/NFJson.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIAsynMongoModule.h"


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

class NFXXTimer : public NFTimerObj, NFIModule
{
public:
	NFXXTimer(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual void OnTimer(uint32_t nTimerID);
};

void NFXXTimer::OnTimer(uint32_t nTimerID)
{
	NFDateTime dateTime = NFDateTime::Now();

	NFLogError("now:{}", dateTime.GetLongTimeString());
	NFLogError("xxxxxxxxxxxxxxxxx");
}

NFCTestActorModule::NFCTestActorModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestActorModule::~NFCTestActorModule()
{
}

void NFCTestActorModule::TestTimer()
{
	NFXXTimer* pTimer = new NFXXTimer(pPluginManager);
	pTimer->SetFixTimer(0, 3, 10, 1000);
}

void NFCTestActorModule::TestMongo()
{
	NFIAsynMongoModule* pMongoModule = pPluginManager->FindModule<NFIAsynMongoModule>();

	pMongoModule->AddMongoServer(NF_ST_GAME, "45.32.39.90", 27017, "test");

	pMongoModule->CreateCollection(NF_ST_GAME, "gaoyi", "uid");
	pMongoModule->CreateCollection(NF_ST_GAME, "test", "uid");

	{
		uint32_t xx = NFRandomUInt(0, 2);
		NFMsg::test_gaoyi_table message;
		message.set_uid(1);
		message.set_name("gaoyi");
		message.set_sex("man");
		message.set_age(29 + xx);
		auto pData = message.mutable_dd();
		pData->set_num(11);
		pData->set_sb(12);

		std::string jsonStr;
		NFServerCommon::MessageToJsonString(message, jsonStr);
		pMongoModule->UpdateOneByKey(NF_ST_GAME, "gaoyi", message, 1);
	}

	{
		//std::string json = "{\"dd.sb\":33}";

		//pMongoModule->UpdateFieldByKey(NF_ST_GAME, "gaoyi", json, "1");
	}

	{
		//std::string fieldPath = "dd.sb";
		//pMongoModule->FindFieldByKey(NF_ST_GAME, "gaoyi", fieldPath, "1");
	}
}

bool NFCTestActorModule::Init()
{
	//TestTimer();
	//NFITaskModule* pTaskModule = pPluginManager->FindModule<NFITaskModule>();

	//for(int i = 0; i < 10000; i++)
	//	pTaskModule->AddTask(new NFLogTask());

	//TestMongo();
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