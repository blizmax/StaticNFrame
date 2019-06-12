// -------------------------------------------------------------------------
//    @FileName         :    NFIAsyMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAsyMysqlModule
//
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIAsycModule.h"
#include "google/protobuf/message.h"
#include <vector>

class NFIAsyMysqlModule
	: public NFIAsycModule
{
public:
	virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool Update(const google::protobuf::Message& message, uint64_t balanceId = 0, uint8_t mysqlEventType = 0) = 0;

	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool Query(google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	* @brief
	*
	* @param  message
	* @return bool
	*/
	virtual bool QueryMore(google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

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
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, uint64_t balanceId = 0) = 0;

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
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) = 0;

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
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, uint64_t balanceId = 0) = 0;

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
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, uint64_t balanceId = 0) = 0;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKey
	* @return bool
	*/
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, uint64_t balanceId = 0) = 0;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKey
	* @param  bExit
	* @return bool
	*/
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit, uint64_t balanceId = 0) = 0;

	/**
	* @brief
	*
	* @param  strTableName
	* @param  strKeyColName
	* @param  strKeyName
	* @param  valueVec
	* @return bool
	*/
	virtual bool Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& valueVec, uint64_t balanceId = 0) = 0;
};

