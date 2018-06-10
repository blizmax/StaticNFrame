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