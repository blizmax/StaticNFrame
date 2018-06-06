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
#include "NFMessageDefine/NFMsgDefine.h"

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
	std::string c;
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

	virtual void Dump()
	{
		std::cout << "dump" << std::endl;
	}
	int a;
};

NFCTestActorModule::NFCTestActorModule(NFIPluginManager* p)
{
	pPluginManager = p;
	NFTask* pTask = new DBTask();
	DBTask* pDBTask = static_cast<DBTask*>(pTask);
	DBXTask* pXTask = static_cast<DBXTask*>(pTask);
	if (pXTask)
	{
		pXTask->a = 1;
	}

	proto::message::PlayerInfo m_proData;

	std::string typeName = m_proData.GetDescriptor()->full_name();

	const google::protobuf::Descriptor* pDesc =
    google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);

	const google::protobuf::Message* prototype =
    google::protobuf::MessageFactory::generated_factory()->GetPrototype(pDesc);

	proto::message::PlayerInfo* new_obj = dynamic_cast<proto::message::PlayerInfo*>(prototype->New());

	assert(m_proData.GetDescriptor() == pDesc);
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