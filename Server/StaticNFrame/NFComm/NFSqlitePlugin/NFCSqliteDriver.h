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
	* @brief 打开数据库
	*
	* @return bool
	*/
	virtual bool Connect(const std::string& dbname);

	/**
	* @brief 设计一些属性
	*
	* @return void
	*/
	virtual void SetPragmaInfo();

	/**
	* @brief 关闭数据库
	*
	* @return bool
	*/
	virtual bool CloseConnection();

	/**
	* @brief 通过protobuf反射， 来实现sqlite的建表
	*
	* @param  message
	* @param  firstKey 是否把第一列作为KEY
	* @return bool
	*/
	virtual bool CreateTable(const google::protobuf::Message& message, bool firstKey = true);

	/**
	* @brief 释放存在表
	*
	* @param  string
	* @return bool
	*/
	virtual bool IsExistTable(const std::string& tableName);

	/**
	* @brief 创建数据库表
	*
	* @param  tableName 数据库名字
	* @param  columnSql 列字符串 类似"ServerID int, UserDBID bigint"
	* @return bool
	*/
	virtual bool CreateTable(const std::string& tableName, const std::string& columnSql);

	/**
	* @brief 执行一条语句
	*
	* @return void
	*/
	virtual bool ExecSql(const std::string& sql);

	/**
	* @brief 执行N条语句
	*
	* @return void
	*/
	virtual bool ExecSql(const std::vector<std::string>& sql);

	/**
	* @brief 执行一条插入语句
	*
	* @return void
	*/
	virtual bool InsertMessage(const google::protobuf::Message& message, bool insertKey = false);

	/**
	* @brief 执行N条插入语句
	*
	* @return void
	*/
	virtual bool InsertMessage(std::vector<google::protobuf::Message*>& vecMessage, bool insertKey = false);

	/**
	* @brief 开启事务
	*
	* @return bool
	*/
	virtual bool BeginTransaction();

	/**
	* @brief 提交事务
	*
	* @return bool
	*/
	virtual bool CommitTransaction();

	/**
	* @brief 执行一条语句 没有开启事务
	*
	* @return void
	*/
	virtual bool ExecSqlNoTransaction(const std::string& sql);
private:
	/**
	* @brief 编译语句
	*
	* @return bool
	*/
	virtual bool SqlitePrepare(const std::string& sql, sqlite3_stmt** pStmt);

	/**
	* @brief 释放编译语句
	*
	* @return bool
	*/
	virtual bool SqliteFinalize(sqlite3_stmt* pStmt);

	/**
	* @brief 释放编译语句
	*
	* @return bool
	*/
	virtual bool SqliteReset(sqlite3_stmt* pStmt);

	/**
	* @brief 释放编译语句
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
	* @brief 执行一条插入语句,没有事务，没有预备编译
	*
	* @return void
	*/
	virtual bool InsertOneMessageNoTransactionNoPrepare(const google::protobuf::Message& dbFieldsMessage, sqlite3_stmt* pStmt, bool insertKey = false);
private:
	sqlite3* m_pSqliteDB;
	std::string m_dbName;
	std::unordered_map<std::string, std::string> m_tableNameToPrepareInsertSql;
};