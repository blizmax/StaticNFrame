// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFCore/NFMemChunk.h"
#include "NFComm/NFCore/NFMallocChunk.h"
#include "NFComm/NFCore/NFSimpleBuffer.h"
#include "NFComm/NFCore/NFMemHashMap.h"
#include "NFComm/NFCore/NFShm.h"


NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

bool NFCTestModule::Init()
{
	NFShm shm;
	shm.init(1024 * 1024 * 32, 8888, true);

	NFMemHashMap memHashMap;
	memHashMap.initDataBlockSize(666, 666, 2);
	memHashMap.initHashRadio(2);
	if (shm.iscreate())
	{
		memHashMap.create(shm.getPointer(), shm.size());

		std::string str;
		memHashMap.get("gaoyi", str);

		std::vector<NFMemHashMap::BlockData> vtData;
		memHashMap.set("gaoyi", "sb", false, vtData);
	}
	else
	{
		memHashMap.connect(shm.getPointer(), shm.size());
	}

	std::string str;
	memHashMap.get("gaoyi", str);
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