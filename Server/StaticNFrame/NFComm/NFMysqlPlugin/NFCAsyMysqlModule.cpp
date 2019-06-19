// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlModule.h
//    @Author           :    Gao.Yi
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlModule
//
// -------------------------------------------------------------------------

#include "NFCAsyMysqlModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFITaskComponent.h"

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
	bool ThreadProcess() override
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
	TPTaskState MainThreadProcess() override
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

class NFMysqlQueryOneMessageTask : public NFMysqlTask
{
public:
	NFMysqlQueryOneMessageTask(const google::protobuf::Message& message, uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_pMessage = message.New();
		m_pMessage->CopyFrom(message);
		m_result = false;
	}

	virtual ~NFMysqlQueryOneMessageTask()
	{
		NF_SAFE_DELETE(m_pMessage);
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->Query(*m_pMessage);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryOneMessageCallBack(m_result, *m_pMessage);
		}
		return TPTASK_STATE_COMPLETED;
	}

private:
	google::protobuf::Message* m_pMessage;
	bool m_result;
};

class NFMysqlQueryMoreMessageTask : public NFMysqlTask
{
public:
	/**
	 * \brief 
	 * \param message 
	 * \param balanceId 
	 */
	NFMysqlQueryMoreMessageTask(const google::protobuf::Message& message, uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_pMessage = message.New();
		m_pMessage->CopyFrom(message);
		m_result = false;
	}

	virtual ~NFMysqlQueryMoreMessageTask()
	{
		NF_SAFE_DELETE(m_pMessage);
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->QueryMore(*m_pMessage);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryMoreMessageCallBack(m_result, *m_pMessage);
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
	bool ThreadProcess() override
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
	TPTaskState MainThreadProcess() override
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

class NFMysqlQueryOneTask : public NFMysqlTask
{
public:
	NFMysqlQueryOneTask(uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_result = false;
	}

	virtual ~NFMysqlQueryOneTask()
	{
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->QueryOne(m_strTableName, m_strKeyColName, m_strKey, m_fieldVec, m_fieldValueMap);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryOneCallBack(m_result, m_fieldValueMap);
		}
		return TPTASK_STATE_COMPLETED;
	}

public:
	bool m_result;
	std::string m_strTableName;
	std::string m_strKeyColName;
	std::string m_strKey;
	std::vector<std::string> m_fieldVec;
	std::map<std::string, std::string> m_fieldValueMap;
};

class NFMysqlQueryMoreTask : public NFMysqlTask
{
public:
	NFMysqlQueryMoreTask(uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_result = false;
	}

	virtual ~NFMysqlQueryMoreTask()
	{
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->QueryMore(m_strTableName, m_strKeyColName, m_strKey, m_fieldVec,
			                                     m_fieldValueMap);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryMoreCallBack(m_result, m_fieldValueMap);
		}
		return TPTASK_STATE_COMPLETED;
	}

public:
	bool m_result;
	std::string m_strTableName;
	std::string m_strKeyColName;
	std::string m_strKey;
	std::vector<std::string> m_fieldVec;
	std::vector<std::map<std::string, std::string>> m_fieldValueMap;
};

class NFMysqlQueryMoreWithCondTask : public NFMysqlTask
{
public:
	NFMysqlQueryMoreWithCondTask(uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_result = false;
	}

	virtual ~NFMysqlQueryMoreWithCondTask()
	{
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->QueryMoreWithCond(m_strTableName, m_strWhereSql, m_fieldVec, m_fieldValueMap);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryMoreWithCondCallBack(m_result, m_fieldValueMap);
		}
		return TPTASK_STATE_COMPLETED;
	}

public:
	bool m_result;
	std::string m_strTableName;
	std::string m_strWhereSql;
	std::vector<std::string> m_fieldVec;
	std::vector<std::map<std::string, std::string>> m_fieldValueMap;
};

class NFMysqlQueryMoreWithLimitTask : public NFMysqlTask
{
public:
	explicit NFMysqlQueryMoreWithLimitTask(uint64_t balanceId): m_nOffset(0), m_nRows(0)
	{
		m_balanceId = balanceId;
		m_result = false;
	}

	virtual ~NFMysqlQueryMoreWithLimitTask()
	{
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->QueryMoreWithLimit(m_strTableName, m_strKeyColName, m_nOffset, m_nRows,
			                                              m_fieldVec, m_fieldValueMap);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryMoreWithLimitCallBack(m_result, m_fieldValueMap);
		}
		return TPTASK_STATE_COMPLETED;
	}

public:
	bool m_result;
	std::string m_strTableName;
	std::string m_strKeyColName;
	int m_nOffset;
	int m_nRows;
	std::vector<std::string> m_fieldVec;
	std::vector<std::map<std::string, std::string>> m_fieldValueMap;
};

class NFMysqlQueryMoreByLikeTask : public NFMysqlTask
{
public:
	NFMysqlQueryMoreByLikeTask(uint64_t balanceId)
	{
		m_balanceId = balanceId;
		m_result = false;
	}

	virtual ~NFMysqlQueryMoreByLikeTask()
	{
	}

	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	bool ThreadProcess() override
	{
		if (m_pMysqlDriver)
		{
			m_result = m_pMysqlDriver->QueryMoreByLike(m_strTableName, m_strKeyColName, m_strKey, m_fieldVec,
			                                           m_fieldValueMap);
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	TPTaskState MainThreadProcess() override
	{
		if (m_pAsyncMysqlModule)
		{
			m_pAsyncMysqlModule->QueryMoreByLikeCallBack(m_result, m_fieldValueMap);
		}
		return TPTASK_STATE_COMPLETED;
	}

public:
	bool m_result;
	std::string m_strTableName;
	std::string m_strKeyColName;
	std::string m_strKey;
	std::vector<std::string> m_fieldVec;
	std::vector<std::map<std::string, std::string>> m_fieldValueMap;
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


	void ProcessTaskStart(NFTask* pTask) override
	{
		NFMysqlTask* pMysqlTask = dynamic_cast<NFMysqlTask*>(pTask);
		if (pMysqlTask)
		{
			pMysqlTask->m_pMysqlDriver = m_pMysqlDriverManager->GetMysqlDriver();
		}
	}

	void ProcessTask(NFTask* pTask) override
	{
		if (pTask)
		{
			pTask->ThreadProcess();
		}
	}

	void ProcessTaskEnd(NFTask* pTask) override
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

bool NFCAsyMysqlModule::AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort,
                                       const std::string strDBName, const std::string strDBUser,
                                       const std::string strDBPwd, const int nRconnectTime /*= 10*/,
                                       const int nRconneCount /*= -1*/)
{
	for (int i = 0; i < 10; i++)
	{
		NFMysqlTaskComponent* pComonnet = NF_NEW NFMysqlTaskComponent(this);
		bool ret = pComonnet->m_pMysqlDriverManager->AddMysqlServer(nServerID, strIP, nPort, strDBName, strDBUser,
		                                                            strDBPwd, nRconnectTime, nRconneCount);
		if (ret == false)
		{
			return false;
		}

		AddActorComponent(pComonnet);
	}

	return true;
}

bool NFCAsyMysqlModule::UpdateOne(const google::protobuf::Message& message, uint64_t balanceId)
{
	NFMysqlUpdateMessageTask* pTask = NF_NEW NFMysqlUpdateMessageTask(message, balanceId);
	return AddTask(pTask);
}


bool NFCAsyMysqlModule::QueryOne(google::protobuf::Message& message, uint64_t balanceId)
{
	NFMysqlQueryOneMessageTask* pTask = NF_NEW NFMysqlQueryOneMessageTask(message, balanceId);
	return AddTask(pTask);
}

bool NFCAsyMysqlModule::QueryMore(google::protobuf::Message& message, uint64_t balanceId)
{
	NFMysqlQueryMoreMessageTask* pTask = NF_NEW NFMysqlQueryMoreMessageTask(message, balanceId);
	return AddTask(pTask);
}

bool NFCAsyMysqlModule::UpdateOne(const std::string& strTableName, const std::string& strKeyColName,
                                  const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap,
                                  uint64_t balanceId)
{
	NFMysqlUpdateTask* pTask = NF_NEW NFMysqlUpdateTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strKeyColName = strKeyColName;
	pTask->m_strKey = strKey;
	pTask->m_fieldValueMap = fieldValueMap;

	return AddTask(pTask);
}

bool NFCAsyMysqlModule::QueryOne(const std::string& strTableName, const std::string& strKeyColName,
                                 const std::string& strKey, const std::vector<std::string>& fieldVec,
                                 uint64_t balanceId)
{
	NFMysqlQueryOneTask* pTask = NF_NEW NFMysqlQueryOneTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strKeyColName = strKeyColName;
	pTask->m_strKey = strKey;
	pTask->m_fieldVec = fieldVec;

	return AddTask(pTask);
}

bool NFCAsyMysqlModule::QueryMore(const std::string& strTableName, const std::string& strKeyColName,
                                  const std::string& strKey, const std::vector<std::string>& fieldVec,
                                  uint64_t balanceId)
{
	NFMysqlQueryMoreTask* pTask = NF_NEW NFMysqlQueryMoreTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strKeyColName = strKeyColName;
	pTask->m_strKey = strKey;
	pTask->m_fieldVec = fieldVec;

	return AddTask(pTask);
}

bool NFCAsyMysqlModule::QueryMoreWithCond(const std::string& strTableName, const std::string& whereSql,
                                          const std::vector<std::string>& fieldVec, uint64_t balanceId)
{
	NFMysqlQueryMoreWithCondTask* pTask = NF_NEW NFMysqlQueryMoreWithCondTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strWhereSql = whereSql;
	pTask->m_fieldVec = fieldVec;

	return AddTask(pTask);
}

bool NFCAsyMysqlModule::QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName,
                                           int nOffset, int nRows, const std::vector<std::string>& fieldVec,
                                           uint64_t balanceId)
{
	NFMysqlQueryMoreWithLimitTask* pTask = NF_NEW NFMysqlQueryMoreWithLimitTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strKeyColName = strKeyColName;
	pTask->m_nOffset = nOffset;
	pTask->m_nRows = nRows;
	pTask->m_fieldVec = fieldVec;

	return AddTask(pTask);
}

bool NFCAsyMysqlModule::QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
                                        const std::string& strKeyName, std::vector<std::string>& fieldVec,
                                        uint64_t balanceId)
{
	NFMysqlQueryMoreByLikeTask* pTask = NF_NEW NFMysqlQueryMoreByLikeTask(balanceId);
	pTask->m_strTableName = strTableName;
	pTask->m_strKeyColName = strKeyColName;
	pTask->m_strKey = strKeyName;
	pTask->m_fieldVec = fieldVec;

	return AddTask(pTask);
}

void NFCAsyMysqlModule::UpdateCallBack(bool result)
{
	//NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "UpdateCallBack");
}

void NFCAsyMysqlModule::UpdateMessageCallBack(bool result)
{
	//NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "UpdateMessageCallBack");
}

void NFCAsyMysqlModule::QueryOneCallBack(bool result, const std::map<std::string, std::string>& fieldValueMap)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, NFCommon::tostr(fieldValueMap));
}

void NFCAsyMysqlModule::QueryMoreCallBack(bool result,
                                          const std::vector<std::map<std::string, std::string>>& fieldValueMap)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, NFCommon::tostr(fieldValueMap));
}

void NFCAsyMysqlModule::QueryMoreWithCondCallBack(bool result,
                                                  const std::vector<std::map<std::string, std::string>>& fieldValueMap)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, NFCommon::tostr(fieldValueMap));
}

void NFCAsyMysqlModule::QueryMoreByLikeCallBack(bool result,
                                                const std::vector<std::map<std::string, std::string>>& fieldValueMap)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, NFCommon::tostr(fieldValueMap));
}

void NFCAsyMysqlModule::QueryOneMessageCallBack(bool result, const google::protobuf::Message& message)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, message.DebugString());
}

void NFCAsyMysqlModule::QueryMoreMessageCallBack(bool result, const google::protobuf::Message& message)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, message.DebugString());
}

void NFCAsyMysqlModule::QueryMoreWithLimitCallBack(bool result,
                                                   const std::vector<std::map<std::string, std::string>>& fieldValueMap)
{
	NFLogInfo(NF_LOG_MYSQL_PLUGIN, 0, "result:{}, data:{}", result, NFCommon::tostr(fieldValueMap));
}
