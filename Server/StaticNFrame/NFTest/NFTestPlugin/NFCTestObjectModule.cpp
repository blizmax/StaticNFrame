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
#include <set>

NFCTestObjectModule::NFCTestObjectModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestObjectModule::~NFCTestObjectModule()
{
}

bool NFCTestObjectModule::Init()
{
	NFIKernelModule* pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	NFIObject* pObject = pKernelModule->CreateObject();
	NF_ASSERT(pObject != nullptr);

	pObject->AddNode("string", NFCData(DT_STRING, "gaoyi"), 0);
	pObject->AddNode("string1", NFCData(DT_STRING, std::string("gaoyi")), 0);
	pObject->AddNode("bool", NFCData(DT_BOOLEAN, true), 0);
	pObject->AddNode("bool2", NFCData(DT_BOOLEAN, false), 0);
	pObject->AddNode("int", NFCData(DT_INT, -1), 0);

	pObject->SetNodeInt64("int", 0);
	pObject->SetNodeUInt64("int", 0);

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