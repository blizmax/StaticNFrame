// -------------------------------------------------------------------------
//    @FileName         :    NFCNoSqlDriverManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCNoSqlDriverManager
//
// -------------------------------------------------------------------------

#ifndef NFC_NOSQL_DRIVER_MANAGER_H
#define NFC_NOSQL_DRIVER_MANAGER_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlDriverManager.h"
#include "NFComm/NFCore/NFMap.hpp"

class NFCNoSqlDriverManager
    : public NFINoSqlDriverManager
{
public:
    NFCNoSqlDriverManager();
    virtual ~NFCNoSqlDriverManager();
public:
    virtual bool AddNoSqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort = 6379, const std::string& strAuthKey = "");
    virtual NFINoSqlDriver* GetNoSqlDriver();
    virtual void CheckNoSql();

protected:
    std::string GetIPByHostName(const std::string& strHostName);

protected:
    uint64_t mLastCheckTime = 0;

    NFMap<int, NFINoSqlDriver> mvNoSql;
    NFMap<int, NFINoSqlDriver> mvInvalidNoSql;
};

#endif