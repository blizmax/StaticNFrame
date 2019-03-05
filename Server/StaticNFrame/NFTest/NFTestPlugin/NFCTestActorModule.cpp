// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestActorModule.h"


NFCTestActorModule::NFCTestActorModule(NFIPluginManager* p)
{
	pPluginManager = p;
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