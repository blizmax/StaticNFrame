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
#include <NFComm/NFPluginModule/NFCObject.h>
#include "NFComm/NFCore/NFMMOMD5.h"
#include "NFComm/NFCore/NFCRC32.h"
#include "NFComm/NFCore/NFCRC16.h"
#include "NFComm/NFCore/NFBase64.h"

#include "NFMessageDefine/NFNodeClassName.h"
#include "NFComm/NFPluginModule/NFIConfigModule.h"

#define NF_GUID_POWER 100000
#define NF_EPOCH 1288834974657

NFCKernelModule::NFCKernelModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
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
	DeleteNFObjectList();
	return true;
}

bool NFCKernelModule::BeforeShut()
{
	return true;
}

bool NFCKernelModule::Shut()
{
	DestroyAll();
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

uint64_t CreateUUID(int32_t nServerID)
{

	static uint32_t serialID = 0;
	++serialID;
	time_t now_time = time(NULL);
	uint64_t newid = (((uint64_t)nServerID << 40) | ((uint64_t)now_time << 16) | ((uint64_t)(serialID & 0x0FFFF)));
	if (serialID == 65534)
	{
		serialID = 0;
	}
	return newid;
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
	uint64_t appId = m_pPluginManager->GetAppID();
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

NFIObject* NFCKernelModule::CreateNFObject(uint64_t guid, const std::string& className)
{
	NFIObject* pObject = nullptr;
	if (ExistNFObject(guid, className))
	{
		NFLogWarning(NF_LOG_KERNEL_PLUGIN, guid, "the object:{} has Exist!", guid);
		return nullptr;
	}

	if (className.empty())
	{
		NFLogWarning(NF_LOG_KERNEL_PLUGIN, guid, "the className is empty", guid);
		return nullptr;
	}

	pObject = NF_NEW NFCObject(guid, m_pPluginManager);

	pObject->AddNode(NF_NODE_STRING_CLASS_NAME, NFCData(NF_DT_STRING, className), 0);

	NFClassObject* pClassObject = FindModule<NFIConfigModule>()->GetClassObject(className);
	if (pClassObject)
	{
		for (auto iter = pClassObject->mClassNodeArray.begin(); iter != pClassObject->mClassNodeArray.end(); iter++)
		{
			NFClassNode& classNode = *iter;
			pObject->AddNode(classNode.mNodeName, classNode.mNodeType, classNode.mFeature);
		}

		for (auto iterTable = pClassObject->mClassTableMap.begin(); iterTable != pClassObject->mClassTableMap.end(); iterTable++)
		{
			NFClassTable& tableNode = iterTable->second;

			std::vector<int> dataColType;

			for (auto iter = tableNode.mClassNodeArray.begin(); iter != tableNode.mClassNodeArray.end(); iter++)
			{
				NFClassNode& classNode = *iter;
				dataColType.push_back(classNode.mNodeType);
			}

			pObject->AddTable(guid, tableNode.mTableName, dataColType, 0);
		}
	}

	mObjectMap[className].emplace(guid, pObject);
	return pObject;
}

NFIObject* NFCKernelModule::GetNFObject(uint64_t guid, const std::string& className)
{
	auto iterClass = mObjectMap.find(className);
	if (iterClass == mObjectMap.end())
	{
		return nullptr;
	}

	auto iter = iterClass->second.find(guid);
	if (iter != iterClass->second.end())
	{
		return iter->second;
	}

	return nullptr;
}

bool NFCKernelModule::ExistNFObject(uint64_t guid, const std::string& className)
{
	auto iterClass = mObjectMap.find(className);
	if (iterClass == mObjectMap.end())
	{
		return false;
	}

	auto iter = iterClass->second.find(guid);
	if (iter != iterClass->second.end())
	{
		return true;
	}

	return false;
}

bool NFCKernelModule::DeleteNFObject(uint64_t guid, const std::string& className)
{
	NFIObject* pObject = GetNFObject(guid, className);
	if (pObject)
	{
		mDeleteList.push_back(std::pair<std::string, uint64_t>(className, guid));
		return true;
	}

	NFLogWarning(NF_LOG_KERNEL_PLUGIN, guid, "delete object:{} not exist!", guid);
	return false;
}

std::unordered_map<uint64_t, NFIObject*>* NFCKernelModule::GetAllObject(const std::string& className)
{
	auto iterClass = mObjectMap.find(className);
	if (iterClass != mObjectMap.end())
	{
		return &iterClass->second;
	}
	return nullptr;
}

bool NFCKernelModule::DestroyNFObject(uint64_t guid, const std::string& className)
{
	NFIObject* pObject = GetNFObject(guid, className);
	if (pObject)
	{
		NF_SAFE_DELETE(pObject);
		mObjectMap[className].erase(guid);
		return true;
	}

	NFLogWarning(NF_LOG_KERNEL_PLUGIN, guid, "destroy object:{} not exist!", guid);
	return false;
}

bool NFCKernelModule::DestroyAll()
{
	for (auto iterclass = mObjectMap.begin(); iterclass != mObjectMap.end(); iterclass++)
	{
		for (auto iter = iterclass->second.begin(); iter != iterclass->second.end(); iter++)
		{
			NF_SAFE_DELETE(iter->second);
		}
		iterclass->second.clear();
	}
	mObjectMap.clear();
	return true;
}

void NFCKernelModule::DeleteNFObjectList()
{
	for (auto iter = mDeleteList.begin(); iter != mDeleteList.end(); iter++)
	{
		DestroyNFObject(iter->second, iter->first);
	}
	mDeleteList.clear();
}

