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
	**  �첽�̴߳�������������һ���߳�������
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
	** ���̴߳������������̴߳�������ύ�����߳����������ݷ��غ����Ƿ��������
	����ֵ�� thread::TPTask::TPTaskState�� ��ο�TPTaskState
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
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceActor(uint64_t balanceId);

	/**
	* @brief ������һ��actor
	*
	* @return actor����
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
	* @brief actor��������
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	size_t mnSuitIndex = 0;
};

#endif