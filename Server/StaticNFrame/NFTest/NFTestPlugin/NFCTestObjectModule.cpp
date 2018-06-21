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
#include "NFMessageDefine/NFDataDefine.hpp"
#include <unordered_set>
#include <set>
#include <NFComm/NFPluginModule/NFDataTable.h>

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
	NFIObject* pObject = pKernelModule->CreateObject(0, NFrame::IObject::ThisName());
	NF_ASSERT(pObject != nullptr);

	pObject->SetNodeInt(NFrame::IObject::Enum_Node_ConfigID, 100);
	pObject->SetNodeString(NFrame::IObject::Enum_Node_ClassName, "IObject");
	pObject->SetNodeInt(NFrame::IObject::Enum_Node_SceneID, 999);
	pObject->SetNodeInt(NFrame::IObject::Enum_Node_GroupID, 111);

	NFDataTable* pTable = pObject->GetTableManager()->GetTableByIndex(NFrame::IObject::Enum_Table_HateTable);
	pTable->AddRow();
	pObject->SetTableInt(NFrame::IObject::Enum_Table_HateTable, 0, NFrame::IObject::HateTable_ConfigID, 1000);
	pObject->SetTableString(NFrame::IObject::Enum_Table_HateTable, 0, NFrame::IObject::HateTable_ClassName, "gaoyi");
	pObject->SetTableInt(NFrame::IObject::Enum_Table_HateTable, 0, NFrame::IObject::HateTable_SceneID, 1000);
	pObject->SetTableInt(NFrame::IObject::Enum_Table_HateTable, 0, NFrame::IObject::HateTable_GroupID, 1000);
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