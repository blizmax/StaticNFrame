// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCMysqlModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCMongoDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIMongoModule.h"
#include "NFCMongoDriverManager.h"

class NFCMongoModule : public NFIMongoModule
{
public:
	/**
	 * @brief
	 *
	 * @param  p
	 * @return  
	 */
	NFCMongoModule(NFIPluginManager* p);
	/**
	 * @brief
	 *
	 * @return  
	 */
	virtual ~NFCMongoModule();

	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Init() override;
	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Shut() override;
	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool Execute() override;
	/**
	 * @brief
	 *
	 * @return bool 
	 */
	virtual bool AfterInit() override;

	virtual bool AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname) override;

	virtual bool IsExistCollection(const int nServerID, const std::string& collectionName) override;

	virtual bool CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key) override;

	virtual bool DropCollection(const int nServerID, const std::string& collectionName) override;

	virtual bool InsertJson(const int nServerID, const std::string& collectionName, const std::string& json_query) override;

	virtual bool InsertMessage(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message) override;
private:
	NFCMongoDriverManager* m_pMongoDriverManager;

	uint64_t mnLastCheckTime;
};

