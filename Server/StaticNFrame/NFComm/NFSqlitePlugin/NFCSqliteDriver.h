// -------------------------------------------------------------------------
//    @FileName         :    NFCSqliteDriver.h
//    @Author           :    GaoYi
//    @Date             :    2019-3-21
//    @Module           :    NFCSqliteDriver
//    @Desc             :
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include <string>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFMessageDefine/NFMsgDefine.h"

#include "sqlite3/sqlite3.h"

class NFCSqliteDriver : public NFIModule
{
public:
	NFCSqliteDriver();

	virtual ~NFCSqliteDriver();

	/**
	* @brief �����ݿ�
	*
	* @return bool
	*/
	virtual bool Connect(const std::string& dbname);

	/**
	* @brief �ر����ݿ�
	*
	* @return void
	*/
	virtual bool CloseConnection();

	/**
	* @brief ͨ��protobuf���䣬 ��ʵ��sqlite�Ľ���
	*
	* @param  message
	* @param  firstKey �Ƿ�ѵ�һ����ΪKEY
	* @return bool
	*/
	virtual bool CreateTable(const google::protobuf::Message& message, bool firstKey = true);

	/**
	* @brief �������ݿ��
	*
	* @param  tableName ���ݿ�����
	* @param  columnSql ���ַ��� ����"ServerID int, UserDBID bigint"
	* @return bool
	*/
	virtual bool CreateTable(const std::string& tableName, const std::string& columnSql);

	/**
	* @brief ִ��һ�����
	*
	* @return void
	*/
	virtual bool ExecSql(const std::string& sql);
private:
	sqlite3* m_pSqliteDB;
	std::string m_dbName;
};