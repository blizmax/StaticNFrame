// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlDriverManager.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2014-11-10
//    @Module           :    NFCMysqlDriverManager
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIMysqlDriver.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"
#include <NFComm/NFCore/NFMap.hpp>

class NFCMysqlDriverManager : public NFIMysqlDriverManager
{
public:
	NFCMysqlDriverManager();

	virtual ~NFCMysqlDriverManager();

	/**
	 * @brief
	 *
	 * @param  nServerID
	 * @param  strIP
	 * @param  nPort
	 * @param  strDBName
	 * @param  strDBUser
	 * @param  strDBPwd
	 * @param  nRconnectTime
	 * @param  nRconneCount
	 * @return bool 
	 */
	bool AddMysqlServer(int nServerID, const std::string& strIP, int nPort, std::string strDBName,
	                    std::string strDBUser, std::string strDBPwd, int nRconnectTime/* = 10*/,
	                    int nRconneCount/* = -1*/) override;

	/**
	 * @brief
	 *
	 * @return NFIMysqlDriver* 
	 */
	NFIMysqlDriver* GetMysqlDriver() override;

	/**
	 * @brief
	 *
	 * @return void 
	 */
	void CheckMysql() override;
protected:
	NFMap<int, NFIMysqlDriver> mvMysql;
	NFMap<int, NFIMysqlDriver> mvInvalidMsyql;
	uint64_t mnLastCheckTime;
};
