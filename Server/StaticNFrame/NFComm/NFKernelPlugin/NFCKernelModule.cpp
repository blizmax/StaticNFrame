// -------------------------------------------------------------------------
//    @FileName         :    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCKernelModule.h"
#include "NFComm/NFCore/NFMemManager.hpp"

#include "NFComm/NFCore/NFProfiler.h"
#include <NFComm/NFPluginModule/NFIPluginManager.h>
#include <NFComm/NFPluginModule/NFCObject.h>
#include <NFComm/NFPluginModule/NFDataNode.h>
#include <NFComm/NFPluginModule/NFDataTable.h>

#define NF_GUID_POWER 100000
#define NF_EPOCH 1288834974657

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
	ProcessMemFree();
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

bool NFCKernelModule::Finalize()
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

uint64_t NFCKernelModule::CreateObjectId()
{
    uint64_t time = NFGetTime();

	if (mLastGuidTimeStamp == time)
	{
		nGUIDIndex = (nGUIDIndex + 1) & 0x3FF;

		if (nGUIDIndex == 0)
		{
			while (time == mLastGuidTimeStamp)
			{
				time = NFGetTime();
			}
		}
	}
	else
	{
		nGUIDIndex = 0;
	}

	//高16位 appId
	uint64_t appId = pPluginManager->GetAppID(); 
	appId = appId << 48;
	//中间38 毫秒
	uint64_t dataId = (time - NF_EPOCH);
	dataId = dataId << 10;
	//底10位，每毫秒1024
	dataId = appId | dataId | nGUIDIndex;

	mLastGuidTimeStamp = time;
	return dataId;
}

