// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIMysqlModule
//
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIMongoModule
	: public NFIModule
{
public:
	virtual ~NFIMongoModule() {}

	virtual bool AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname) = 0;

	virtual bool IsExistCollection(const int nServerID, const std::string& collectionName) = 0;

	virtual bool CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key) = 0;

	virtual bool DropCollection(const int nServerID, const std::string& collectionName) = 0;

	virtual bool InsertJson(const int nServerID, const std::string& collectionName, const std::string& json_query) = 0;

	virtual bool InsertMessage(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message) = 0;

	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key) = 0;

	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key) = 0;

	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key) = 0;

	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json) = 0;

	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message) = 0;
};


