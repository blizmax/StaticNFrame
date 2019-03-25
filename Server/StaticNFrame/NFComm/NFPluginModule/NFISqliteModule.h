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
	* @brief �����ݿ�
	*
	* @return bool
	*/
	virtual bool AddSqliteServer(uint32_t nServerID, const std::string& dbname) = 0;

	/**
	* @brief ͨ��protobuf���䣬 ��ʵ��sqlite�Ľ���
	*
	* @param  message
	* @param  firstKey �Ƿ�ѵ�һ����ΪKEY
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const google::protobuf::Message& message, bool firstKey = true) = 0;

	/**
	* @brief �������ݿ��
	*
	* @param  tableName ���ݿ�����
	* @param  columnSql ���ַ��� ����"ServerID int, UserDBID bigint"
	* @return bool
	*/
	virtual bool CreateTable(uint32_t nServerID, const std::string& tableName, const std::string& columnSql) = 0;

	/**
	* @brief ִ��һ�����
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::string& sql) = 0;

	/**
	* @brief ִ��N�����
	*
	* @return void
	*/
	virtual bool ExecSql(uint32_t nServerID, const std::vector<std::string>& sql) = 0;

	/**
	* @brief ִ��һ���������
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, const google::protobuf::Message& message, bool insertKey = false) = 0;

	/**
	* @brief ִ��N���������
	*
	* @return void
	*/
	virtual bool InsertMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey = false) = 0;
};
