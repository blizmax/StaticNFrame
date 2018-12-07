// -------------------------------------------------------------------------
//    @FileName         :    NFCMongoDriver.h
//    @Author           :    GaoYi
//    @Date             :    2018-07-28
//    @Module           :    NFCMongoDriver
//    @Desc             :
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include <string>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFMessageDefine/NFMsgDefine.h"

#include "mongoc.h"
#include "bson.h"
#include "bcon.h"

//特殊表，用来记录当前数据库，主键， _id将于主键数据一致
#define PRIMARY_TABLE "max_id_index"
#define PRIMARY_TABLE_COL "primary"
#define PRIMARY_TABLE_KEY "_id"

class NFCMongoDriver : public NFIModule
{
public:
	NFCMongoDriver(const int nReconnectTime = 60, const int nReconnectCount = -1);

	virtual ~NFCMongoDriver();

	virtual bool Connect(const std::string& ip, uint32_t port, const std::string& dbname);

	virtual bool Ping();

	/**
	* @brief 关闭连接
	*
	* @return void
	*/
	virtual void CloseConnection();

	/**
	* @brief 是否连接有效
	*
	* @return bool
	*/
	virtual bool Enable();

	/**
	* @brief 是否可以重连
	*
	* @return bool
	*/
	virtual bool CanReconnect();

	/**
	* @brief 重连
	*
	* @return bool
	*/
	virtual bool Reconnect();

	/**
	* @brief 是否需要重连
	*
	* @return bool
	*/
	bool IsNeedReconnect();

	/**
	* @brief 连接
	*
	* @return bool
	*/
	bool Connect();

	/**
	* @brief 循环执行
	*
	* @return bool
	*/
	virtual bool Execute();

	virtual bool IsExistCollection(const std::string& collectionName);

	virtual bool CreateCollection(const std::string& collectionName, const std::string& primay_key = "");

	virtual bool DropCollection(const std::string& collectionName);

	virtual std::string FindOne(const std::string& collectionName, const std::string& json_query);
	virtual std::vector<std::string> FindMany(const std::string& collectionName, const std::string& json_query);

	virtual std::vector<std::string> FindAll(const std::string& collectionName);

	virtual std::string FindKey(const std::string& collectionName, int64_t key);

	virtual bool InsertOne(const std::string& collectionName, bson_t *doc);
	virtual bool InsertOne(const std::string& collectionName, const std::string& json_query);
	virtual bool InsertOne(const std::string& collectionName, const google::protobuf::Message& message);

	virtual bool Update(const std::string& json);

	virtual bool InsertPrimaryKey(const std::string& collectionName, const std::string& primaryKey);
	virtual void FindAllPrimaryKey();

	virtual mongoc_collection_t* GetCollection(const std::string& collectionName);

	static bool HandleMongocError(const bson_error_t& error);
private:
	mongoc_client_t		*m_pClient;
	mongoc_database_t    *m_pDatabase;
	mongoc_uri_t* m_uri;
	std::string m_ip;
	uint32_t m_port;
	std::string m_dbname;

	NFMap<std::string, mongoc_collection_t> m_collectionMap;
	std::map<std::string, std::string> m_collectionPrimaryKeyMap;

	float mfCheckReconnect; //检查重连的时间

	int mnReconnectTime;
	int mnReconnectCount;
};
