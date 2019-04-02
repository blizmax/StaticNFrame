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
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlDriverManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

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
    NFINT64 mLastCheckTime = 0;

    NFMap<int, NFINoSqlDriver> mvNoSql;
    NFMap<int, NFINoSqlDriver> mvInvalidNoSql;
};

#endif