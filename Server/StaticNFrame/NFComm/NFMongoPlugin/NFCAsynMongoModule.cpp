// -------------------------------------------------------------------------
//    @FileName         :    NFCAsynMongoModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-1-1
//    @Module           :    NFCAsynMongoModule
//
//
// -------------------------------------------------------------------------

#include "NFCAsynMongoModule.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

NFCAsynMongoModule::NFCAsynMongoModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCAsynMongoModule::~NFCAsynMongoModule()
{

}

bool NFCAsynMongoModule::Awake()
{
	StartActorPool(10);
	return true;
}

bool NFCAsynMongoModule::Init()
{
	return true;
}

bool NFCAsynMongoModule::BeforeShut()
{
	CloseActorPool();
	return true;
}

bool NFCAsynMongoModule::Shut()
{
	return true;
}

bool NFCAsynMongoModule::Execute()
{
	return true;
}

bool NFCAsynMongoModule::AfterInit()
{
	return true;
}

bool NFCAsynMongoModule::StartActorPool(const int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		int actorId = FindModule<NFITaskModule>()->RequireActor();
		if (actorId <= 0)
		{
			return false;
		}

		NFCMongoDriverManager *pMongoDriverManager = NF_NEW NFCMongoDriverManager();
		m_mongoDriverMap.emplace(actorId, pMongoDriverManager);
		m_vecActorPool.push_back(actorId);
	}

	return true;
}

bool NFCAsynMongoModule::CloseActorPool()
{
	for (auto iter = m_mongoDriverMap.begin(); iter != m_mongoDriverMap.end(); iter++)
	{
		NF_SAFE_DELETE(iter->second);
	}

	m_mongoDriverMap.clear();
	return true;
}

int NFCAsynMongoModule::GetBalanceActor(uint64_t balanceId)
{
	if (balanceId == 0)
	{
		return GetRandActor();
	}
	else
	{
		if (m_vecActorPool.size() <= 0)
		{
			return -1;
		}
		mnSuitIndex = balanceId % m_vecActorPool.size();
		return m_vecActorPool[mnSuitIndex];
	}
}

int NFCAsynMongoModule::GetRandActor()
{
	if (m_vecActorPool.size() <= 0)
	{
		return -1;
	}

	mnSuitIndex++;
	mnSuitIndex = mnSuitIndex % m_vecActorPool.size();

	return m_vecActorPool[mnSuitIndex];
}

NFCMongoDriverManager* NFCAsynMongoModule::GetDriver(int actorId)
{
	auto iter = m_mongoDriverMap.find(actorId);
	if (iter != m_mongoDriverMap.end())
	{
		return iter->second;
	}
	return nullptr;
}

/**
* @brief 添加serverid的链接, 一个serverid对应一个链接数据库的驱动
*
* @return bool
*/
bool NFCAsynMongoModule::AddMongoServer(const int nServerID, const std::string& uri, const std::string& dbname)
{
	for (auto iter = m_mongoDriverMap.begin(); iter != m_mongoDriverMap.end(); iter++)
	{
		NFCMongoDriverManager* pMongoDriverManager = iter->second;
		pMongoDriverManager->AddMongoServer(nServerID, uri, dbname);
	}

	return true;
}


bool NFCAsynMongoModule::AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname)
{
	for (auto iter = m_mongoDriverMap.begin(); iter != m_mongoDriverMap.end(); iter++)
	{
		NFCMongoDriverManager* pMongoDriverManager = iter->second;
		pMongoDriverManager->AddMongoServer(nServerID, ip, port, dbname);
	}

	return true;
}

bool NFCAsynMongoModule::CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key)
{
	for (auto iter = m_mongoDriverMap.begin(); iter != m_mongoDriverMap.end(); iter++)
	{
		NFCMongoDriverManager* pMongoDriverManager = iter->second;
		pMongoDriverManager->CreateCollection(nServerID, collectionName, key);
	}

	return true;
}

bool NFCAsynMongoModule::InsertJson(const int nServerID, const std::string& collectionName, const std::string& json_query)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoInsertJsonTask* pTask = NF_NEW NFMongoInsertJsonTask(pMongoDriverManager, nServerID, collectionName, json_query);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::InsertMessage(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoInsertMessageTask* pTask = NF_NEW NFMongoInsertMessageTask(pMongoDriverManager, nServerID, collectionName, message);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateOneByIntKeyTask* pTask = NF_NEW NFMongoUpdateOneByIntKeyTask(pMongoDriverManager, nServerID, collectionName, json, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string& key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateOneByStringKeyTask* pTask = NF_NEW NFMongoUpdateOneByStringKeyTask(pMongoDriverManager, nServerID, collectionName, json, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateOneMessageByIntKeyTask* pTask = NF_NEW NFMongoUpdateOneMessageByIntKeyTask(pMongoDriverManager, nServerID, collectionName, message, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateOneMessageByStringKeyTask* pTask = NF_NEW NFMongoUpdateOneMessageByStringKeyTask(pMongoDriverManager, nServerID, collectionName, message, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateOneJsonTask* pTask = NF_NEW NFMongoUpdateOneJsonTask(pMongoDriverManager, nServerID, collectionName, json);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateOneMessageTask* pTask = NF_NEW NFMongoUpdateOneMessageTask(pMongoDriverManager, nServerID, collectionName, message);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return false;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindOne(const int nServerID, const std::string& collectionName, const std::string& json_query)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoFindOneJsonTask* pTask = NF_NEW NFMongoFindOneJsonTask(pMongoDriverManager, nServerID, collectionName, json_query);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindMany(const int nServerID, const std::string& collectionName, const std::string& json_query, const std::string& json_opts)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoFindManyTask* pTask = NF_NEW NFMongoFindManyTask(pMongoDriverManager, nServerID, collectionName, json_query, json_opts);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindAll(const int nServerID, const std::string& collectionName)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoFindAllTask* pTask = NF_NEW NFMongoFindAllTask(pMongoDriverManager, nServerID, collectionName);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsynMongoModule::FindAllToJson(const int nServerID, const std::string& collectionName)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoFindAllToJsonTask* pTask = NF_NEW NFMongoFindAllToJsonTask(pMongoDriverManager, nServerID, collectionName);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindOneByKey(const int nServerID, const std::string& collectionName, int64_t key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		FindOneByIntKey* pTask = NF_NEW FindOneByIntKey(pMongoDriverManager, nServerID, collectionName, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindOneByKey(const int nServerID, const std::string& collectionName, const std::string& key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		FindOneByStringKey* pTask = NF_NEW FindOneByStringKey(pMongoDriverManager, nServerID, collectionName, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCAsynMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateFieldByStringKeyTask* pTask = NF_NEW NFMongoUpdateFieldByStringKeyTask(pMongoDriverManager, nServerID, collectionName, json, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCAsynMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateFieldByIntKeyTask* pTask = NF_NEW NFMongoUpdateFieldByIntKeyTask(pMongoDriverManager, nServerID, collectionName, json, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}

	return true;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCAsynMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateFieldMessageByIntKeyTask* pTask = NF_NEW NFMongoUpdateFieldMessageByIntKeyTask(pMongoDriverManager, nServerID, collectionName, message, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 更新数据
*
* @return bool
*/
bool NFCAsynMongoModule::UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoUpdateFieldMessageByStringKeyTask* pTask = NF_NEW NFMongoUpdateFieldMessageByStringKeyTask(pMongoDriverManager, nServerID, collectionName, message, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, int64_t key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		FindFieldByIntKey* pTask = NF_NEW FindFieldByIntKey(pMongoDriverManager, nServerID, collectionName, fieldPath, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief 查找数据
*
* @return bool
*/
bool NFCAsynMongoModule::FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, const std::string& key)
{
	int actorId = GetRandActor();
	NFCMongoDriverManager* pMongoDriverManager = GetDriver(actorId);
	if (pMongoDriverManager)
	{
		NFMongoFindFieldByStringKeyTask* pTask = NF_NEW NFMongoFindFieldByStringKeyTask(pMongoDriverManager, nServerID, collectionName, fieldPath, key);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}

	return true;
}


