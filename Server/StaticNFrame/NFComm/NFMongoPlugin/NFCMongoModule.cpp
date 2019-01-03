// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/05
//    @Email			:    445267987@qq.com
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMongoModule.h"
#include "NFCMongoDriverManager.h"

NFCMongoModule::NFCMongoModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mnLastCheckTime = 0;
	m_pMongoDriverManager = NF_NEW NFCMongoDriverManager();
}

NFCMongoModule::~NFCMongoModule()
{
	NF_SAFE_DELETE(m_pMongoDriverManager);
}

bool NFCMongoModule::Init()
{
	return true;
}

bool NFCMongoModule::Shut()
{
	return true;
}

bool NFCMongoModule::AfterInit()
{
	return true;
}

bool NFCMongoModule::Execute()
{
	return true;
}

/**
* @brief 添加serverid的链接, 一个serverid对应一个链接数据库的驱动
*
* @return bool
*/
bool NFCMongoModule::AddMongoServer(const int nServerID, const std::string& uri, const std::string& dbname)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->AddMongoServer(nServerID, uri, dbname);
	}
	return false;
}


bool NFCMongoModule::AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->AddMongoServer(nServerID, ip, port, dbname);
	}
	return false;
}

bool NFCMongoModule::IsExistCollection(const int nServerID, const std::string& collectionName)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->IsExistCollection(nServerID, collectionName);
	}
	return false;
}

bool NFCMongoModule::CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->CreateCollection(nServerID, collectionName, key);
	}
	return false;
}

bool NFCMongoModule::DropCollection(const int nServerID, const std::string& collectionName)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->DropCollection(nServerID, collectionName);
	}
	return false;
}

bool NFCMongoModule::InsertJson(const int nServerID, const std::string& collectionName, const std::string& json_query)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->InsertOne(nServerID, collectionName, json_query);
	}
	return false;
}

bool NFCMongoModule::InsertMessage(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->InsertOne(nServerID, collectionName, message);
	}
	return false;
}

bool NFCMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, json, key);
	}
	return false;
}

bool NFCMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, json, key);
	}
	return false;
}

bool NFCMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, message, key);
	}
	return false;
}

bool NFCMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, message, key);
	}
	return false;
}

bool NFCMongoModule::UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateOne(nServerID, collectionName, json);
	}
	return false;
}

bool NFCMongoModule::UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateOne(nServerID, collectionName, message);
	}
	return false;
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoModule::FindOne(const int nServerID, const std::string& collectionName, const std::string& json_query)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindOne(nServerID, collectionName, json_query);
	}
	return std::string();
}

/**
* @brief 查找数据
*
* @return bool
*/
std::vector<std::string> NFCMongoModule::FindMany(const int nServerID, const std::string& collectionName, const std::string& json_query, const std::string& json_opts)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindMany(nServerID, collectionName, json_query, json_opts);
	}
	return std::vector<std::string>();
}

/**
* @brief 查找数据
*
* @return bool
*/
std::vector<std::string> NFCMongoModule::FindAll(const int nServerID, const std::string& collectionName)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindAll(nServerID, collectionName);
	}
	return std::vector<std::string>();
}

std::string NFCMongoModule::FindAllToJson(const int nServerID, const std::string& collectionName)
{
	std::string result;
	std::vector<std::string> vec = FindAll(nServerID, collectionName);
	result += "[";
	for (size_t i = 0; i < vec.size(); i++)
	{
		result += vec[i];
		if (i != vec.size()-1)
			result += ",";
	}
	result += "]";
	return result;
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoModule::FindOneByKey(const int nServerID, const std::string& collectionName, int64_t key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindOneByKey(nServerID, collectionName, key);
	}
	return std::string();
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoModule::FindOneByKey(const int nServerID, const std::string& collectionName, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindOneByKey(nServerID, collectionName, key);
	}
	return std::string();
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, json, key);
	}
	return false;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, json, key);
	}
	return false;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, message, key);
	}
	return false;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, message, key);
	}
	return false;
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoModule::FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, int64_t key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindFieldByKey(nServerID, collectionName, fieldPath, key);
	}
	return false;
}

/**
* @brief 查找数据
*
* @return bool
*/
std::string NFCMongoModule::FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->FindFieldByKey(nServerID, collectionName, fieldPath, key);
	}
	return false;
}