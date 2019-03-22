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
	* @brief 关闭数据库
	*
	* @return void
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
private:
	sqlite3* m_pSqliteDB;
	std::string m_dbName;
};