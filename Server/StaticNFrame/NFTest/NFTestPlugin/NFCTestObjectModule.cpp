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

	pObject->GetNodeManager()->AddNode("string", NFCData(DT_STRING, "gaoyi"), 0);
	pObject->GetNodeManager()->AddNode("string1", NFCData(DT_STRING, std::string("gaoyi")), 0);
	pObject->GetNodeManager()->AddNode("bool", NFCData(DT_BOOLEAN, true), 0);
	pObject->GetNodeManager()->AddNode("int", NFCData(DT_INT, -1), 0);
	pObject->GetNodeManager()->AddNode("float", NFCData(DT_DOUBLE, 0.001), 0);
	pObject->GetNodeManager()->AddNode("array", NFCData(DT_ARRAY, NFCData::Array()), 0);
	pObject->GetNodeManager()->AddNode("list", NFCData(DT_LIST, NFCData::List()), 0);
	pObject->GetNodeManager()->AddNode("mapstring", NFCData(DT_MAPSTRING, NFCData::MapStringData()), 0);
	pObject->GetNodeManager()->AddNode("mapint", NFCData(DT_MAPINT, NFCData::MapIntData()), 0);

	pObject->SetNodeInt("int", 0);
	pObject->SetNodeInt32("int", 0);
	pObject->SetNodeInt64("int", 0);
	pObject->SetNodeUInt32("int", 0);
	pObject->SetNodeUInt64("int", 0);
	pObject->SetNodeString("string", nullptr);
	pObject->SetNodeFloat("float", 0.001);
	pObject->SetNodeDouble("float", 0.002);
	pObject->AddArrayItem("array", NFCData(DT_INT, 100));

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