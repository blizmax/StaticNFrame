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

	virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/) override;
	virtual NFIMysqlDriver* GetMysqlDriver() override;
	virtual void CheckMysql() override;
protected:
	NFMap<int, NFIMysqlDriver> mvMysql;
	NFMap<int, NFIMysqlDriver> mvInvalidMsyql;
	uint64_t mnLastCheckTime;
};

