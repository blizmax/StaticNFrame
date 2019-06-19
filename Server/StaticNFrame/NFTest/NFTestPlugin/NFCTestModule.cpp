// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFMemChunk.h"
#include "NFComm/NFCore/NFMallocChunk.h"
#include "NFComm/NFCore/NFSimpleBuffer.h"
//#include "NFComm/NFCore/NFMemHashMap.h"
#include "NFComm/NFCore/NFMemHashMap2.h"
#include "NFComm/NFCore/NFShm.h"
#include "sqlite3/sqlite3.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFSqlitePlugin/NFCSqliteDriver.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFCore/NFTime.h"

struct Test
{
	Test()
	{
		a = 0;
		b = 0;
		c = 0;
	}

	Test(uint32_t _a , uint32_t _b, uint32_t _c)
	{
		a = _a;
		b = _b;
		c = _c;
	}

	void print()
	{
		NFLogDebug(0,0,"a={},b={},c={}", a, b, c);
	}
	uint32_t a;
	uint32_t b;
	uint32_t c;
};


NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

void testHashMap()
{
	NFShm shm;
	shm.init(1024 * 1024 * 32, 8888, true);

	NFMemHashMap<int, Test> memHashMap;
	memHashMap.create(shm.getPointer(), 1024 * 1024 * 32);

	{
		memHashMap.insert(1, Test(1, 2, 3));
		memHashMap.insert(2, Test(4, 5, 6));
		memHashMap.insert(3, Test(7, 8, 9));
	}


	NFMemHashMap<int, Test> memHashMap2;
	memHashMap2.connect(shm.getPointer(), 1024 * 1024 * 32);

	{
		for (auto iter = memHashMap2.begin(); iter != memHashMap2.end(); iter++)
		{
			iter->getValue().print();
		}
	}
}


void testLog()
{
	uint64_t startTime = NFTime::Tick();

	for (int i = 0; i < 1000; i++)
	{
		std::string str;
		for (uint32_t i = 0; i < 1000; i++)
		{
			str += "fuck your mother.................................................";
		}
		NFLogDebug(0,0,str.c_str());
	}

	uint64_t endTime = NFTime::Tick();
	std::cout << "log 100000 cost time: " << (endTime - startTime) << "ms" << std::endl;
}

bool NFCTestModule::Init()
{
	std::string str = NFCommon::tostr(true);
	return true;
}

bool NFCTestModule::AfterInit()
{
	return true;
}

bool NFCTestModule::Execute()
{
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