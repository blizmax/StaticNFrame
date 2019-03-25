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
	* @brief ���һЩ����
	*
	* @return void
	*/
	virtual void SetPragmaInfo();

	/**
	* @brief �ر����ݿ�
	*
	* @return bool
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
	* @brief �ͷŴ��ڱ�
	*
	* @param  string
	* @return bool
	*/
	virtual bool IsExistTable(const std::string& tableName);

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

	/**
	* @brief ִ��N�����
	*
	* @return void
	*/
	virtual bool ExecSql(const std::vector<std::string>& sql);

	/**
	* @brief ִ��һ���������
	*
	* @return void
	*/
	virtual bool InsertMessage(const google::protobuf::Message& message, bool insertKey = false);

	/**
	* @brief ִ��N���������
	*
	* @return void
	*/
	virtual bool InsertMessage(std::vector<google::protobuf::Message*>& vecMessage, bool insertKey = false);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool BeginTransaction();

	/**
	* @brief �ύ����
	*
	* @return bool
	*/
	virtual bool CommitTransaction();

	/**
	* @brief ִ��һ����� û�п�������
	*
	* @return void
	*/
	virtual bool ExecSqlNoTransaction(const std::string& sql);
private:
	/**
	* @brief �������
	*
	* @return bool
	*/
	virtual bool SqlitePrepare(const std::string& sql, sqlite3_stmt** pStmt);

	/**
	* @brief �ͷű������
	*
	* @return bool
	*/
	virtual bool SqliteFinalize(sqlite3_stmt* pStmt);

	/**
	* @brief �ͷű������
	*
	* @return bool
	*/
	virtual bool SqliteReset(sqlite3_stmt* pStmt);

	/**
	* @brief �ͷű������
	*
	* @return bool
	*/
	virtual bool SqliteStep(sqlite3_stmt* pStmt);

	/**
	* @brief
	*
	* @return bool
	*/
	virtual bool SqliteBindStep(const google::protobuf::Message& message, sqlite3_stmt* pStmt, bool insertKey = false);

	/**
	* @brief ִ��һ���������,û������û��Ԥ������
	*
	* @return void
	*/
	virtual bool InsertOneMessageNoTransactionNoPrepare(const google::protobuf::Message& dbFieldsMessage, sqlite3_stmt* pStmt, bool insertKey = false);
private:
	sqlite3* m_pSqliteDB;
	std::string m_dbName;
	std::unordered_map<std::string, std::string> m_tableNameToPrepareInsertSql;
};