// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCMysqlModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCMysqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"

class NFCMysqlModule
	: public NFIMysqlModule, public NFTimerObj
{
public:
	/**
	 * @brief
	 *
	 * @param  p
	 * @return  
	 */
	NFCMysqlModule(NFIPluginManager* p);
	/**
	 * @brief
	 *
	 * @return  
	 */
	virtual ~NFCMysqlModule();

	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Init() override;
	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Shut() override;

	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Execute() override;

	/**
	 * @brief
	 *
	 * @return bool
	 */
	virtual void OnTimer(uint32_t nTimerID) override;

	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool AfterInit() override;

	//////////////////////////////////////////////////////////////////////////
	/**
	 * @brief
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool Update(const google::protobuf::Message& message) override;
	
	/**
	 * @brief
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool Query(google::protobuf::Message& message) override;
	
	/**
	 * @brief
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool QueryMore(google::protobuf::Message& message) override;

	/**
	 * @brief
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool Execute(const std::string& qstr) override;
	
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
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) override;

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
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap) override;
	
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
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;

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
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::map<std::string, ::string>& valueVec) override;

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
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec) override;

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
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec) override;

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
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec) override;

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
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec) override;
	
	/**
	 * @brief
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @return bool 
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) override;

	/**
	 * @brief
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @param  bExit
	 * @return bool 
	 */
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit) override;

	/**
	 * @brief
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKeyName
	 * @param  valueVec
	 * @return bool 
	 */
	virtual bool Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec) override;

	/**
	 * @brief
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKeyName
	 * @param  valueVec
	 * @return bool
	 */
	virtual bool Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec) override;

	/**
	 * @brief
	 *
	 * @param  nServerID
	 * @param  strIP
	 * @param  nPort
	 * @param  strDBName
	 * @param  strDBUser
	 * @param  strDBPwd
	 * @param  nRconnectTime
	 * @param  nRconneCount
	 * @return bool 
	 */
	virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) override;
private:
	NFIMysqlDriverManager* m_pMysqlDriverManager;

	uint64_t mnLastCheckTime;
};

