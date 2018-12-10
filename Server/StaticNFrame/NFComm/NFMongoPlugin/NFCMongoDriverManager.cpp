
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

bool NFCMongoDriverManager::AddMongoServer(const int nServerID, const std::string& uri, const std::string& dbname)
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
	if (pMongoDriver->Connect(uri, dbname))
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

bool NFCMongoDriverManager::AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname)
{
	std::string str = NF_FORMAT("mongodb://{}:{}", ip, port);
	return AddMongoServer(nServerID, str, dbname);
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

bool NFCMongoDriverManager::UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateOneByKey(collectionName, message, key);
}

bool NFCMongoDriverManager::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string& key)
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

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoDriverManager::FindOne(const int nServerID, const std::string& collectionName, const std::string& json_query)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return std::string();
	}

	return pMongoDriver->FindOne(collectionName, json_query);
}

/**
* @brief 查找数据
*
* @return bool
*/
std::vector<std::string> NFCMongoDriverManager::FindMany(const int nServerID, const std::string& collectionName, const std::string& json_query, const std::string& json_opts)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return std::vector<std::string>();
	}

	return pMongoDriver->FindMany(collectionName, json_query, json_opts);
}

/**
* @brief 查找数据
*
* @return bool
*/
std::vector<std::string> NFCMongoDriverManager::FindAll(const int nServerID, const std::string& collectionName)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return std::vector<std::string>();
	}

	return pMongoDriver->FindAll(collectionName);
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoDriverManager::FindOneByKey(const int nServerID, const std::string& collectionName, int64_t key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return std::string();
	}

	return pMongoDriver->FindOneByKey(collectionName, key);
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoDriverManager::FindOneByKey(const int nServerID, const std::string& collectionName, const std::string& key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return std::string();
	}

	return pMongoDriver->FindOneByKey(collectionName, key);
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoDriverManager::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateFieldByKey(collectionName, json, key);
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoDriverManager::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateFieldByKey(collectionName, json, key);
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoDriverManager::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateFieldByKey(collectionName, message, key);
}

bool NFCMongoDriverManager::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->UpdateFieldByKey(collectionName, message, key);
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoDriverManager::FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, int64_t key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->FindFieldByKey(collectionName, fieldPath, key);
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoDriverManager::FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, const std::string& key)
{
	NFCMongoDriver* pMongoDriver = mMongoDriver.GetElement(nServerID);
	if (!pMongoDriver)
	{
		return false;
	}

	return pMongoDriver->FindFieldByKey(collectionName, fieldPath, key);
}


