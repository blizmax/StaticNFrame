
#include "NFCMongoDriverManager.h"

NFCMongoDriverManager::NFCMongoDriverManager(): mnLastCheckTime(0)
{
}

NFCMongoDriverManager::~NFCMongoDriverManager()
{
	for (NFCMongoDriver* pMongoDriver = mMongoDriver.First(); nullptr != pMongoDriver; pMongoDriver = mMongoDriver.Next())
	{
		NF_SAFE_DELETE(pMongoDriver);
		pMongoDriver = nullptr;
	}
	mMongoDriver.ClearAll();
	for (NFCMongoDriver* pMongoDriver = mInvalidMongoDriver.First(); nullptr != pMongoDriver; pMongoDriver = mInvalidMongoDriver.Next())
	{
		NF_SAFE_DELETE(pMongoDriver);
		pMongoDriver = nullptr;
	}
	mInvalidMongoDriver.ClearAll();
}

void NFCMongoDriverManager::CheckMongo()
{
	int nServerID = 0;
	std::vector<int> xIntVec;
	for (NFCMongoDriver* pMongolDriver = mMongoDriver.First(nServerID); pMongolDriver != NULL; pMongolDriver = mMongoDriver.Next(nServerID))
	{
		if (!pMongolDriver->Enable())
		{
			xIntVec.push_back(nServerID);
			mInvalidMongoDriver.AddElement(nServerID, pMongolDriver);
		}
	}

	for (int i = 0; i < (int)xIntVec.size(); ++i)
	{
		mMongoDriver.RemoveElement(xIntVec[i]);
	}
	//////////////////////////////////////////////////////////////////////////
	xIntVec.clear();
	nServerID = 0;

	for (NFCMongoDriver* pMongolDriver = mInvalidMongoDriver.First(nServerID); pMongolDriver != NULL; pMongolDriver = mInvalidMongoDriver.Next(nServerID))
	{
		if (!pMongolDriver->Enable() && pMongolDriver->CanReconnect())
		{
			pMongolDriver->Reconnect();
			if (pMongolDriver->Enable())
			{
				xIntVec.push_back(nServerID);
				mMongoDriver.AddElement(nServerID, pMongolDriver);
			}
		}
	}

	for (int i = 0; i < (int)xIntVec.size(); ++i)
	{
		mInvalidMongoDriver.RemoveElement(xIntVec[i]);
	}
}

bool NFCMongoDriverManager::AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (pMongoDriver)
	{
		return false;
	}

	NFCMongoDriver* pInvalidMongoDriver = mInvalidMongoDriver.GetElement(nServerID);
	if (pInvalidMongoDriver)
	{
		return false;
	}


	pMongoDriver = NF_NEW NFCMongoDriver();
	if (pMongoDriver->Connect(ip, port, dbname))
	{
		pMongoDriver->CreateCollection(PRIMARY_TABLE);
		pMongoDriver->FindAllPrimaryKey();
		mMongoDriver.AddElement(nServerID, pMongoDriver);
	}
	else
	{
		mInvalidMongoDriver.AddElement(nServerID, pMongoDriver);
	}

	return true;
}

bool NFCMongoDriverManager::IsExistCollection(const int nServerID, const std::string& collectionName)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->IsExistCollection(collectionName);
}

bool NFCMongoDriverManager::CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->CreateCollection(collectionName, key);
}

bool NFCMongoDriverManager::DropCollection(const int nServerID, const std::string& collectionName)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->DropCollection(collectionName);
}

bool NFCMongoDriverManager::InsertOne(const int nServerID, const std::string& collectionName, const std::string& json_query)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->InsertOne(collectionName, json_query);
}

bool NFCMongoDriverManager::InsertOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->InsertOne(collectionName, message);
}

bool NFCMongoDriverManager::UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateOneByKey(collectionName, message, key);
}

bool NFCMongoDriverManager::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateOneByKey(collectionName, json, key);
}

bool NFCMongoDriverManager::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateOneByKey(collectionName, json, key);
}

bool NFCMongoDriverManager::UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateOne(collectionName, json);
}

bool NFCMongoDriverManager::UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateOne(collectionName, message);
}


