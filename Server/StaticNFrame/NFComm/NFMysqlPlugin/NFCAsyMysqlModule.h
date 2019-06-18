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
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) override;

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
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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

public:
	void UpdateCallBack(bool result);

	void UpdateMessageCallBack(bool result);
private:
	uint64_t mnLastCheckTime;
};