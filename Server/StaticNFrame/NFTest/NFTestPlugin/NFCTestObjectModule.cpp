// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestObjectModule.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include <NFComm/NFPluginModule/NFCObject.h>
#include <NFComm/NFPluginModule/NFIDataNodeManager.h>
#include <NFComm/NFPluginModule/NFCData.h>
#include <NFComm/NFPluginModule/NFIKernelModule.h>
#include <NFComm/NFPluginModule/NFIPluginManager.h>
#include <unordered_set>

NFCTestObjectModule::NFCTestObjectModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestObjectModule::~NFCTestObjectModule()
{
}

bool NFCTestObjectModule::Init()
{
	NFCObject* pObject = new NFCObject(0, pPluginManager);
	NFCData data;
	data.SetUInt64((uint64_t)NFGetTime());
	pObject->GetNodeManager()->AddNode("gaoyi", data, 0);
	pObject->SetNodeUInt64("gaoyi", 0);

	NFIKernelModule* pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	std::unordered_set<uint64_t> objectIdSet;
	while (true)
	{
		uint64_t objectId = pKernelModule->CreateObjectId();
		if (objectIdSet.find(objectId) != objectIdSet.end())
			break;

		objectIdSet.emplace(objectId);
	}
	return true;
}

bool NFCTestObjectModule::AfterInit()
{
	return true;
}

bool NFCTestObjectModule::Execute()
{
	return true;
}

bool NFCTestObjectModule::BeforeShut()
{
	return true;
}

bool NFCTestObjectModule::Shut()
{
	return true;
}