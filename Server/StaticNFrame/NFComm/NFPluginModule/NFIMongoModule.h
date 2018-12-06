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
};


