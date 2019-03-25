// -------------------------------------------------------------------------
//    @FileName         :    NFCSqliteDriverManager.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-25
//    @Module           :    NFSqlitePlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCSqliteDriver.h"
#include <NFComm/NFCore/NFMap.hpp>

class NFCSqliteDriverManager
{
public:
	NFCSqliteDriverManager();

	virtual ~NFCSqliteDriverManager();

	/**
	* @brief �����ݿ�
	*
	* @return bool
	*/
	virtual bool AddSqliteServer(uint32_t nServerID, const std::string& dbname);

	/**
	* @brief ������ݿ�
	*
	* @return NFCSqliteDriver
	*/
	virtual NFCSqliteDriver* GetSqliteDriver(uint32_t nServerID);

	/**
	* @brief ������ݿ�
	*
	* @return NFCSqliteDriver
	*/
	virtual NFCSqliteDriver* GetSqliteDriver();
protected:
	NFMap<int, NFCSqliteDriver> m_sqliteDriverMap;
};
