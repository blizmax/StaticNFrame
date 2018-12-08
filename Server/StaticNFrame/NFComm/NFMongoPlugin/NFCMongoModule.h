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

	/**
	* @brief 添加serverid的链接, 一个serverid对应一个链接数据库的驱动
	*
	* @return bool
	*/
	virtual bool AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname) override;

	/**
	* @brief 是否存在表格
	*
	* @return bool
	*/
	virtual bool IsExistCollection(const int nServerID, const std::string& collectionName) override;

	/**
	* @brief 创建表格
	*
	* @return bool
	*/
	virtual bool CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key) override;

	/**
	* @brief 删除表格
	*
	* @return bool
	*/
	virtual bool DropCollection(const int nServerID, const std::string& collectionName) override;

	/**
	* @brief 插入一条数据
	*
	* @return bool
	*/
	virtual bool InsertJson(const int nServerID, const std::string& collectionName, const std::string& json_query) override;

	/**
	* @brief 插入一条数据
	*
	* @return bool
	*/
	virtual bool InsertMessage(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message) override;

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key) override;

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key) override;

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key) override;

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json) override;;

	/**
	* @brief 更新一个数据
	*
	* @return bool
	*/
	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message) override;
private:
	NFCMongoDriverManager* m_pMongoDriverManager;

	uint64_t mnLastCheckTime;
};

