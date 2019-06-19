// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFCMysqlDriverManager.h"
#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFITaskComponent.h"

class NFCAsyMysqlModule;

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

    virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);
public:
	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool UpdateOne(const google::protobuf::Message& message, uint64_t balanceId = 0, uint8_t mysqlEventType = 0) override;

	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool QueryOne(google::protobuf::Message& message, uint64_t balanceId = 0) override;

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
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) override;

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
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

    /**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		查询条件
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKeyName
	* @param  valueVec
	* @return bool
	*/
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

public:
	void UpdateCallBack(bool result);

	void UpdateMessageCallBack(bool result);

	void QueryOneCallBack(bool result, const std::map<std::string, std::string>& fieldValueMap);

	void QueryMoreCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	void QueryMoreWithCondCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	void QueryMoreWithLimitCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	void QueryMoreByLikeCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	void QueryOneMessageCallBack(bool result, const google::protobuf::Message& message);

	void QueryMoreMessageCallBack(bool result, const google::protobuf::Message& message);
private:
	uint64_t mnLastCheckTime;
};