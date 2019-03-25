// -------------------------------------------------------------------------
//    @FileName         :    NFCSqliteModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-25
//    @Module           :    NFSqlitePlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFISqliteModule.h"
#include "NFCSqliteDriverManager.h"

class NFCSqliteModule : public NFISqliteModule
{
public:
	NFCSqliteModule(NFIPluginManager* pPluginManager);
	virtual ~NFCSqliteModule();

	virtual bool Init();

	virtual bool AfterInit();

	virtual bool Execute();

	virtual bool Shut();

	virtual bool Finalize();
public:
	/**
	* @brief 打开数据库
	*
	* @return bool
	*/
	virtual bool AddSqliteServer(uint32_t nServerID, const std::string& dbname);

	/**
	* @brief 通过protobuf反射， 来实现sqlite的建表
	*
	* @param  message
	* @param  firstKey 是否把第一列作为KEY
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const google::protobuf::Message& message, bool firstKey = true);

	/**
	* @brief 创建数据库表
	*
	* @param  tableName 数据库名字
	* @param  columnSql 列字符串 类似"ServerID int, UserDBID bigint"
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const std::string& tableName, const std::string& columnSql);

	/**
	* @brief 执行一条语句
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::string& sql);

	/**
	* @brief 执行N条语句
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::vector<std::string>& sql);

	/**
	* @brief 执行一条插入语句
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, const google::protobuf::Message& message, bool insertKey = false);

	/**
	* @brief 执行N条插入语句
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey = false);
private:
	NFCSqliteDriverManager* m_pSqliteDriverManager;
};