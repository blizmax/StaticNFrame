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

	virtual bool AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname);

	virtual bool IsExistCollection(const int nServerID, const std::string& collectionName);

	virtual bool CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key);

	virtual bool DropCollection(const int nServerID, const std::string& collectionName);

	virtual bool InsertOne(const int nServerID, const std::string& collectionName, const std::string& json_query);

	virtual bool InsertOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message);

	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key);

	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key);

	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key);

	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json);

	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message);

	virtual void CheckMongo();
protected:
	NFMap<int, NFCMongoDriver> mMongoDriver;
	NFMap<int, NFCMongoDriver> mInvalidMongoDriver;
	uint64_t mnLastCheckTime;
};

