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
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	return true;
}

bool NFCKernelModule::Execute()
{
	ProcessMemFree();
	return true;
}

NFIObject* NFCKernelModule::CreateObject(uint64_t objectId, const std::string& strClassName)
{
	if (objectId == 0)
	{
		objectId = CreateObjectId();
	}

	NFIObject* pObject = nullptr;

	NFIDataNodeManager* pClassNodeManager = m_pClassModule->GetNodeManager(strClassName);
	NFIDataTableManager* pClassTableManager = m_pClassModule->GetTableManager(strClassName);

	if (pClassNodeManager != nullptr && pClassTableManager != nullptr)
	{
		pObject = NF_NEW NFCObject(objectId, pPluginManager);
		mObjectMap.emplace(objectId, pObject);

		NFIDataNodeManager* pNodeManager = pObject->GetNodeManager();
		NFIDataTableManager* pTableManager = pObject->GetTableManager();

		for (size_t index = 0; index < pClassNodeManager->GetNodeCount(); ++index)
		{
			NFDataNode* pClassConfigNode = pClassNodeManager->GetNodeByIndex(index);
			NF_ASSERT(pClassConfigNode != nullptr);
			bool bRet = pNodeManager->AddNode(pClassConfigNode->GetName(), pClassConfigNode->GetValue(), pClassConfigNode->GetFeature());
			if (!bRet)
			{
				NF_ASSERT_MSG(0, "AddNode failed, please check it");
				continue;
			}
		}

		for (size_t index = 0; index < pClassTableManager->GetCount(); ++index)
		{
			NFDataTable* pClassConfigTable = pClassTableManager->GetTableByIndex(index);
			NF_ASSERT(pClassConfigTable != nullptr);
			NFCData colTypeList;
			pClassConfigTable->GetColTypeList(colTypeList);
			pTableManager->AddTable(objectId, pClassConfigTable->GetName(), colTypeList, pClassConfigTable->GetFeature());
		}
	}
	return pObject;
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
	for (auto it = mObjectMap.begin(); it != mObjectMap.end(); ++it)
	{
		NF_SAFE_DELETE(it->second);
	}
	mObjectMap.clear();
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

