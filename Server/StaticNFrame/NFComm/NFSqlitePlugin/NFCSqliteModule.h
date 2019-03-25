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
	* @brief �����ݿ�
	*
	* @return bool
	*/
	virtual bool AddSqliteServer(uint32_t nServerID, const std::string& dbname);

	/**
	* @brief ͨ��protobuf���䣬 ��ʵ��sqlite�Ľ���
	*
	* @param  message
	* @param  firstKey �Ƿ�ѵ�һ����ΪKEY
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const google::protobuf::Message& message, bool firstKey = true);

	/**
	* @brief �������ݿ��
	*
	* @param  tableName ���ݿ�����
	* @param  columnSql ���ַ��� ����"ServerID int, UserDBID bigint"
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const std::string& tableName, const std::string& columnSql);

	/**
	* @brief ִ��һ�����
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::string& sql);

	/**
	* @brief ִ��N�����
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::vector<std::string>& sql);

	/**
	* @brief ִ��һ���������
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, const google::protobuf::Message& message, bool insertKey = false);

	/**
	* @brief ִ��N���������
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey = false);
private:
	NFCSqliteDriverManager* m_pSqliteDriverManager;
};