#include "NFCAsyMysqlModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"


NFCAsyMysqlModule::NFCAsyMysqlModule(NFIPluginManager* p)
{
    m_pPluginManager = p;
    mnLastCheckTime = 0;
}

NFCAsyMysqlModule::~NFCAsyMysqlModule()
{

}

bool NFCAsyMysqlModule::Awake()
{
	StartActorPool(10);
	return true;
}

bool NFCAsyMysqlModule::Init()
{
    return true;
}

bool NFCAsyMysqlModule::BeforeShut()
{
    return true;
}

bool NFCAsyMysqlModule::Shut()
{
	CloseActorPool();
    return true;
}

bool NFCAsyMysqlModule::Execute()
{
    return true;
}

bool NFCAsyMysqlModule::AfterInit()
{
    return true;
}

bool NFCAsyMysqlModule::StartActorPool(const int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		int actorId = FindModule<NFITaskModule>()->RequireActor();
		if (actorId <= 0)
		{
			return false;
		}

		NFCMysqlDriverManager *pMysqlDriverManager = NF_NEW NFCMysqlDriverManager();
		m_mysqlDriverMap.emplace(actorId, pMysqlDriverManager);
		m_vecActorPool.push_back(actorId);
	}

    return true;
}

bool NFCAsyMysqlModule::CloseActorPool()
{
	for (auto iter = m_mysqlDriverMap.begin(); iter != m_mysqlDriverMap.end(); iter++)
	{
		NF_SAFE_DELETE(iter->second);
	}

	m_mysqlDriverMap.clear();
	return true;
}

bool NFCAsyMysqlModule::AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime /*= 10*/, const int nRconneCount /*= -1*/)
{
	for (auto iter = m_mysqlDriverMap.begin(); iter != m_mysqlDriverMap.end(); iter++)
	{
		NFCMysqlDriverManager* pMysqlDriverManager = iter->second;
		bool ret = pMysqlDriverManager->AddMysqlServer(nServerID, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
		if (ret == false)
		{
			return false;
		}
	}

    return true;
}

int NFCAsyMysqlModule::GetBalanceActor(uint64_t balanceId)
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

int NFCAsyMysqlModule::GetRandActor()
{
	if (m_vecActorPool.size() <= 0)
	{
		return -1;
	}

	mnSuitIndex++;
	mnSuitIndex = mnSuitIndex % m_vecActorPool.size();

	return m_vecActorPool[mnSuitIndex];
}

NFCMysqlDriverManager* NFCAsyMysqlModule::GetDriver(int actorId)
{
	auto iter = m_mysqlDriverMap.find(actorId);
	if (iter != m_mysqlDriverMap.end())
	{
		return iter->second;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
/**
* @brief
*
* @param  message
* @return bool
*/
bool NFCAsyMysqlModule::Update(const google::protobuf::Message& message, uint64_t balanceId, uint8_t mysqlEventType)
{
	int actorId = GetBalanceActor(balanceId);
	NFCMysqlDriverManager* pMysqlDriverManager = GetDriver(actorId);
	if (pMysqlDriverManager)
	{
		NFMysqlUpdateMessageTask* pTask = NF_NEW NFMysqlUpdateMessageTask(m_pPluginManager, pMysqlDriverManager, message, balanceId, mysqlEventType);
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief
*
* @param  message
* @return bool
*/
bool NFCAsyMysqlModule::Query(google::protobuf::Message& message, uint64_t balanceId)
{
	return true;
}

/**
* @brief
*
* @param  message
* @return bool
*/
bool NFCAsyMysqlModule::QueryMore(google::protobuf::Message& message, uint64_t balanceId)
{
	return true;
}

/**
* @brief
*
* @param  strTableName
* @param  strKeyColName
* @param  strKey
* @param  fieldVec
* @param  valueVec
* @return bool
*/
bool NFCAsyMysqlModule::Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, uint64_t balanceId)
{
	int actorId = GetBalanceActor(balanceId);
	NFCMysqlDriverManager* pMysqlDriverManager = GetDriver(actorId);
	if (pMysqlDriverManager)
	{
		NFMysqlUpdateTask* pTask = NF_NEW NFMysqlUpdateTask(this, pMysqlDriverManager->GetMysqlDriver(), balanceId);
		pTask->m_strTableName = strTableName;
		pTask->m_strKeyColName = strKeyColName;
		pTask->m_strKey = strKey;
		if (fieldVec.size() == valueVec.size())
		{
			for(size_t i = 0; i < fieldVec.size(); ++i)
			{
				pTask->m_fieldValueMap.emplace(fieldVec[i], valueVec[i]);
			}
		}
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

bool NFCAsyMysqlModule::Update(const std::string& strTableName, const std::string& strKeyColName,
	const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId)
{
	int actorId = GetBalanceActor(balanceId);
	NFCMysqlDriverManager* pMysqlDriverManager = GetDriver(actorId);
	if (pMysqlDriverManager)
	{
		NFMysqlUpdateTask* pTask = NF_NEW NFMysqlUpdateTask(this, pMysqlDriverManager->GetMysqlDriver(), balanceId);
		pTask->m_strTableName = strTableName;
		pTask->m_strKeyColName = strKeyColName;
		pTask->m_strKey = strKey;
		pTask->m_fieldValueMap = fieldValueMap;
		FindModule<NFITaskModule>()->AddTask(actorId, pTask);
	}
	return true;
}

/**
* @brief
*
* @param  strTableName
* @param  strKeyColName
* @param  strKey
* @param  fieldVec
* @param  valueVec
* @return bool
*/
bool NFCAsyMysqlModule::Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, uint64_t balanceId)
{
	return true;
}

/**
* @brief
*
* @param  strTableName
* @param  strKeyColName
* @param  nOffset
* @param  nRows
* @param  fieldVec
* @param  valueVec
* @return bool
*/
bool NFCAsyMysqlModule::Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, uint64_t balanceId)
{
	return true;
}

/**
* @brief
*
* @param  strTableName
* @param  strKeyColName
* @param  strKey
* @return bool
*/
bool NFCAsyMysqlModule::Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, uint64_t balanceId)
{
	return true;
}

/**
* @brief
*
* @param  strTableName
* @param  strKeyColName
* @param  strKey
* @param  bExit
* @return bool
*/
bool NFCAsyMysqlModule::Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit, uint64_t balanceId)
{
	return true;
}

/**
* @brief
*
* @param  strTableName
* @param  strKeyColName
* @param  strKeyName
* @param  valueVec
* @return bool
*/
bool NFCAsyMysqlModule::Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& valueVec, uint64_t balanceId)
{
	return true;
}

void NFCAsyMysqlModule::UpdateCallBack(bool result)
{

}
