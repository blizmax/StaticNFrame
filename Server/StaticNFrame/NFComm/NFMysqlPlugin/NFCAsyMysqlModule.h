// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_MYSQL_MODULE_H
#define NFC_ASY_MYSQL_MODULE_H

#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFCMysqlDriverManager.h"
#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

class NFMysqlUpdateMessageTask : public NFTask
{
public:
	NFMysqlUpdateMessageTask(NFIPluginManager* pPluginManager, NFCMysqlDriverManager* p, const google::protobuf::Message& message, uint64_t balanceId, uint8_t mysqlEventType)
	{
		m_pPluginManager = pPluginManager;
		m_pMysqlDriverManager = p;
		m_balanceId = balanceId;
		mMysqlEventType = mysqlEventType;
		m_pMessage = message.New();
		m_pMessage->CopyFrom(message);
		ret = false;
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
		if (m_pMysqlDriverManager)
		{
			NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
			if (pDriver)
			{
				ret = pDriver->Update(*m_pMessage);
			}
		}
		return true;
	}

	/**
	** 主线程处理函数，将在线程处理完后，提交给主线程来处理，根据返回函数是否继续处理
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		if (mMysqlEventType != 0)
		{
			m_pPluginManager->FindModule<NFIEventModule>()->FireExecute(NFEVENT_MYSQL_UPDATE_MESSAGE, m_balanceId, mMysqlEventType, *m_pMessage);
		}

		return TPTASK_STATE_COMPLETED;
	}
private:
	NFCMysqlDriverManager* m_pMysqlDriverManager;
	google::protobuf::Message* m_pMessage;
	NFIPluginManager* m_pPluginManager;
	bool ret;
	uint8_t mMysqlEventType;
};

class NFCAsyMysqlModule
    : public NFIAsyMysqlModule
{
public:

    NFCAsyMysqlModule(NFIPluginManager* p);
    virtual ~NFCAsyMysqlModule();

	virtual bool Awake();
    virtual bool Init();
    virtual bool AfterInit();
    virtual bool Execute();
    virtual bool BeforeShut();
    virtual bool Shut();

    virtual bool StartActorPool(const int nCount);
    virtual bool CloseActorPool();

	/**
	* @brief 通过任务的动态均衡id，获得actor
	*		 为了防止数据库错乱，防止同时对数据库表中的一条数据，读取写入，
	*		 使用动态均衡id, 使得在某个时候只有一条线程对表中的一条数据，读取或写入
	* @param balanceId 动态均衡id
	* @return	一个actor索引
	*/
	int GetBalanceActor(uint64_t balanceId);

	/**
	* @brief 随机获得一个actor
	*
	* @return actor索引
	*/
	int GetRandActor();

	NFCMysqlDriverManager* GetDriver(int actorId);

    virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);
public:
	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool Update(const google::protobuf::Message& message, uint64_t balanceId = 0, uint8_t mysqlEventType = 0) override;

	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool Query(google::protobuf::Message& message, uint64_t balanceId = 0) override;

	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool QueryMore(google::protobuf::Message& message, uint64_t balanceId = 0) override;

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
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, uint64_t balanceId = 0) override;

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
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, uint64_t balanceId = 0) override;

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
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, uint64_t balanceId = 0) override;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKey
	* @return bool
	*/
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, uint64_t balanceId = 0) override;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKey
	* @param  bExit
	* @return bool
	*/
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit, uint64_t balanceId = 0) override;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKeyName
	* @param  valueVec
	* @return bool
	*/
	virtual bool Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& valueVec, uint64_t balanceId = 0) override;

private:
	std::unordered_map<int, NFCMysqlDriverManager*> m_mysqlDriverMap;
	uint64_t mnLastCheckTime;

	/**
	* @brief actor索引数组
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief 用来平衡随机获得actor
	*/
	size_t mnSuitIndex = 0;
};

#endif