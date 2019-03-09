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
#include "NFComm/NFCore/NFMemHashMap2.h"
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

	NFMemHashMap2<int, int> memHashMap;
	memHashMap.create(shm.getPointer(), 1024);

	NFMemHashMap2<int, int> memHashMap2;
	memHashMap2.connect(shm.getPointer(), 1024);
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