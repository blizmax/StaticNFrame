// -------------------------------------------------------------------------
//    @FileName         :    NFCSqliteDriverManager.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-25
//    @Module           :    NFSqlitePlugin
//
// -------------------------------------------------------------------------


#include "NFCSqliteDriverManager.h"

NFCSqliteDriverManager::NFCSqliteDriverManager()
{

}

NFCSqliteDriverManager::~NFCSqliteDriverManager()
{
	NFCSqliteDriver* pDriver = m_sqliteDriverMap.First();
	while (pDriver)
	{
		NF_SAFE_DELETE(pDriver);
		pDriver = m_sqliteDriverMap.Next();
	}

	m_sqliteDriverMap.ClearAll();
}

/**
* @brief 打开数据库
*
* @return bool
*/
bool NFCSqliteDriverManager::AddSqliteServer(uint32_t nServerID, const std::string& dbname)
{
	NFCSqliteDriver* pDriver = NF_NEW NFCSqliteDriver();
	bool ret = pDriver->Connect(dbname);
	if (!ret)
	{
		return false;
	}

	m_sqliteDriverMap.AddElement(nServerID, pDriver);
	return true;
}

/**
* @brief 获得数据库
*
* @return NFCSqliteDriver
*/
NFCSqliteDriver* NFCSqliteDriverManager::GetSqliteDriver(uint32_t nServerID)
{
	return m_sqliteDriverMap.GetElement(nServerID);
}

/**
* @brief 获得数据库
*
* @return NFCSqliteDriver
*/
NFCSqliteDriver* NFCSqliteDriverManager::GetSqliteDriver()
{
	return m_sqliteDriverMap.First();
}