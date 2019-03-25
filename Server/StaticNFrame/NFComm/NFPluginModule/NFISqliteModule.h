// -------------------------------------------------------------------------
//    @FileName         :    NFISqliteModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-25
//    @Module           :    NFSqlitePlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIModule.h"
#include "protobuf/src/google/protobuf/message.h"
#include <vector>
#include <string>

class NFISqliteModule : public NFIModule
{
public:
	virtual ~NFISqliteModule()
	{

	}

	/**
	* @brief 打开数据库
	*
	* @return bool
	*/
	virtual bool AddSqliteServer(uint32_t nServerID, const std::string& dbname) = 0;

	/**
	* @brief 通过protobuf反射， 来实现sqlite的建表
	*
	* @param  message
	* @param  firstKey 是否把第一列作为KEY
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const google::protobuf::Message& message, bool firstKey = true) = 0;

	/**
	* @brief 创建数据库表
	*
	* @param  tableName 数据库名字
	* @param  columnSql 列字符串 类似"ServerID int, UserDBID bigint"
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const std::string& tableName, const std::string& columnSql) = 0;

	/**
	* @brief 执行一条语句
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::string& sql) = 0;

	/**
	* @brief 执行N条语句
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::vector<std::string>& sql) = 0;

	/**
	* @brief 执行一条插入语句
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, const google::protobuf::Message& message, bool insertKey = false) = 0;

	/**
	* @brief 执行N条插入语句
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey = false) = 0;
};
