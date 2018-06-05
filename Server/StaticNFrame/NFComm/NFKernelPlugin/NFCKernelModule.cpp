// -------------------------------------------------------------------------
//    @FileName         :    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCKernelModule.h"
#include "NFComm/NFCore/NFMemManager.hpp"

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFCore/NFProfiler.h"

NFCKernelModule::NFCKernelModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCKernelModule::~NFCKernelModule()
{
}

bool NFCKernelModule::Init()
{
	return true;
}

bool NFCKernelModule::Execute()
{
	BEGIN_PROFILE(__FUNCTION__);
	ProcessMemFree();
	END_PROFILE();
	return true;
}

bool NFCKernelModule::BeforeShut()
{
	return true;
}

bool NFCKernelModule::Shut()
{
	return true;
}

bool NFCKernelModule::AfterInit()
{
	return true;
}

void NFCKernelModule::ProcessMemFree()
{
	NFMemManager::GetSingletonPtr()->FreeMem();
}