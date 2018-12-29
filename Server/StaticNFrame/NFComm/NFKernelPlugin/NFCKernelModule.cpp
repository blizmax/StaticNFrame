// -------------------------------------------------------------------------
//    @FileName         :    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCKernelModule.h"

#include <NFComm/NFPluginModule/NFIPluginManager.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include <NFComm/NFPluginModule/NFDataNode.h>
#include <NFComm/NFPluginModule/NFDataTable.h>
#include "NFComm/NFCore/NFMMOMD5.h"
#include "NFComm/NFCore/NFCRC32.h"
#include "NFComm/NFCore/NFCRC16.h"
#include "NFComm/NFCore/NFBase64.h"

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

}

uint64_t NFCKernelModule::Get64UUID()
{
	uint64_t time = NFGetTime();

	if (mLastGuidTimeStamp == time)
	{
		nGUIDIndex = (nGUIDIndex + 1) & 0xFFF;

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
	//保留后41位时间
	uint64_t dataId = (time - NF_EPOCH);
	dataId = dataId << 22;

	//中间十位是机器代码 

	dataId |= (appId & 0x3FF) << 12;

	//最后12位是squenceID
	dataId |= nGUIDIndex & 0xFFF;

	mLastGuidTimeStamp = time;
	return dataId;
}

uint64_t NFCKernelModule::Get32UUID()
{
	uint64_t time = NFGetSecondTime();
	if (mLastGuidTimeStamp == 0)
	{
		mLastGuidTimeStamp = time;
	}

	mLastGuidTimeStamp = mLastGuidTimeStamp + 1;
	return mLastGuidTimeStamp;
}

uint64_t NFCKernelModule::GetUUID()
{
	return Get64UUID();
}

std::string NFCKernelModule::GetMD5(const std::string& str)
{
	return NFMMOMD5(str).toStr();
}

uint32_t NFCKernelModule::GetCRC32(const std::string& s)
{
	return NFCRC32::Sum(s);
}

uint16_t NFCKernelModule::GetCRC16(const std::string& s)
{
	return NFCRC16::Sum(s);
}

std::string NFCKernelModule::Base64Encode(const std::string& s)
{
	return NFBase64::Encode(s);
}

std::string NFCKernelModule::Base64Decode(const std::string& s)
{
	return NFBase64::Decode(s);
}

