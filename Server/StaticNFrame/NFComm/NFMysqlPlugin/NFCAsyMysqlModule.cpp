#include "NFCAsyMysqlModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"

class NFMysqlTask : public NFTask
{
public:
	NFMysqlTask()
	{
		m_pMysqlDriver = nullptr;
		m_pAsyncMysqlModule = nullptr;
	}

	virtual ~NFMysqlTask()
	{

	}
public:
	NFIMysqlDriver* m_pMysqlDriver;
	NFCAsyMysqlModule* m_pAsyncMysqlModule;
};

class NFMysqlUpdateMessageTask : public NFMysqlTask
{
public:
	NFMysqlUpdateMessageTask(const google::protobuf::Message& message, uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_pMessage = message.New();
		m_pMessage->CopyFrom(message);
		m_result = false;
	}

	virtual ~NFMysqlUpdateMessageTask()
	{
		NF_SAFE_DELETE(m_pMessage);
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess()
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->Update(*m_pMessage);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->UpdateMessageCallBack(m_result);
		}
		return TPTASK_STATE_COMPLETED;
	}
private:
	google::protobuf::Message* m_pMessage;
	bool m_result;
};

class NFMysqlUpdateTask : public NFMysqlTask
{
public:
	NFMysqlUpdateTask(uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_result = false;
	}

	virtual ~NFMysqlUpdateTask()
	{

	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess()
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->Update(m_strTableName, m_strKeyColName, m_strKey, m_fieldValueMap);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->UpdateCallBack(m_result);
		}
		return TPTASK_STATE_COMPLETED;
	}
public:
	bool m_result;
	std::string m_strTableName;
	std::string m_strKeyColName;
	std::string m_strKey;
	std::map<std::string, std::string> m_fieldValueMap;
};

class NFMysqlTaskComponent : public NFITaskComponent
{
public:
	NFMysqlTaskComponent(NFCAsyMysqlModule* pAsyMysqlModule)
	{
		m_pAsyMysqlModule = pAsyMysqlModule;
		m_pMysqlDriverManager = NF_NEW NFCMysqlDriverManager();
	}

	virtual ~NFMysqlTaskComponent()
	{
		NF_SAFE_DELETE(m_pMysqlDriverManager);
	}


	virtual void ProcessTaskStart(NFTask* pTask)
	{
		NFMysqlTask* pMysqlTask = dynamic_cast<NFMysqlTask*>(pTask);
		if (pMysqlTask)
		{
			pMysqlTask->m_pMysqlDriver = m_pMysqlDriverManager->GetMysqlDriver();
		}
	}

	virtual void ProcessTask(NFTask* pTask)
	{
		if (pTask)
		{
			pTask->ThreadProcess();
		}
	}

	virtual void ProcessTaskEnd(NFTask* pTask)
	{
		NFMysqlTask* pMysqlTask = dynamic_cast<NFMysqlTask*>(pTask);
		if (pMysqlTask)
		{
			pMysqlTask->m_pMysqlDriver = nullptr;
			pMysqlTask->m_pAsyncMysqlModule = m_pAsyMysqlModule;
		}
	}
public:
	NFCMysqlDriverManager* m_pMysqlDriverManager;
	NFCAsyMysqlModule* m_pAsyMysqlModule;
};


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

bool NFCAsyMysqlModule::AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime /*= 10*/, const int nRconneCount /*= -1*/)
{
	for (int i = 0; i < 10; i++)
	{
		NFMysqlTaskComponent* pComonnet = NF_NEW NFMysqlTaskComponent(this);
		bool ret = pComonnet->m_pMysqlDriverManager->AddMysqlServer(nServerID, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
		if (ret == false)
		{
			return false;
		}

		AddActorComponent(pComonnet);
	}

    return true;
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
	NFMysqlUpdateMessageTask* pTask = NF_NEW NFMysqlUpdateMessageTask(message, balanceId);
	return AddTask(pTask);
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
	NFMysqlUpdateTask* pTask = NF_NEW NFMysqlUpdateTask(balanceId);
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
	
	return AddTask(pTask);
}

bool NFCAsyMysqlModule::Update(const std::string& strTableName, const std::string& strKeyColName,
	const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId)
{
	NFMysqlUpdateTask* pTask = NF_NEW NFMysqlUpdateTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strKeyColName = strKeyColName;
	pTask->m_strKey = strKey;
	pTask->m_fieldValueMap = fieldValueMap;

	return AddTask(pTask);
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
	//NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "UpdateCallBack");
}

void NFCAsyMysqlModule::UpdateMessageCallBack(bool result)
{
	//NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "UpdateMessageCallBack");
}
