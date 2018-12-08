// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlDriverManager.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2014-11-10
//    @Module           :    NFCMysqlDriverManager
//
// -------------------------------------------------------------------------

#pragma once


#include "NFCMongoDriver.h"
#include <NFComm/NFCore/NFMap.hpp>

class NFCMongoDriverManager
{
public:
	NFCMongoDriverManager();

	virtual ~NFCMongoDriverManager();

	/**
	* @brief 添加serverid的链接, 一个serverid对应一个链接数据库的驱动
	*
	* @return bool
	*/
	virtual bool AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname);

	/**
	* @brief 是否存在表格
	*
	* @return bool
	*/
	virtual bool IsExistCollection(const int nServerID, const std::string& collectionName);

	/**
	* @brief 创建表格
	*
	* @return bool
	*/
	virtual bool CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key);

	/**
	* @brief 删除表格
	*
	* @return bool
	*/
	virtual bool DropCollection(const int nServerID, const std::string& collectionName);

	/**
	* @brief 插入一条数据
	*
	* @return bool
	*/
	virtual bool InsertOne(const int nServerID, const std::string& collectionName, const std::string& json_query);

	/**
	* @brief 查找数据
	*
	* @return bool
	*/
	virtual std::string FindOne(const int nServerID, const std::string& collectionName, const std::string& json_query);

	/**
	* @brief 查找数据
	*
	* @return bool
	*/
	virtual std::vector<std::string> FindMany(const int nServerID, const std::string& collectionName, const std::string& json_query, const std::string& json_opts = "");

	/**
	* @brief 查找数据
	*
	* @return bool
	*/
	virtual std::vector<std::string> FindAll(const int nServerID, const std::string& collectionName);

	/**
	* @brief 查找数据
	*
	* @return bool
	*/
	virtual std::string FindOneyByKey(const int nServerID, const std::string& collectionName, int64_t key);

	/**
	* @brief 查找数据
	*
	* @return bool
	*/
	virtual std::string FindOneyByKey(const int nServerID, const std::string& collectionName, const std::string& key);

	/**
	* @brief 插入一条数据
	*
	* @return bool
	*/
	virtual bool InsertOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message);

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key);

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key);

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key);

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json);

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message);

	/**
	* @brief 定时检查链接
	*
	* @return bool
	*/
	virtual void CheckMongo();
protected:
	NFMap<int, NFCMongoDriver> mMongoDriver;
	NFMap<int, NFCMongoDriver> mInvalidMongoDriver;
	uint64_t mnLastCheckTime;
};

