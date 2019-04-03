// -------------------------------------------------------------------------
//    @FileName         :    NFINoSqlDriverManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFINoSqlDriverManager
//
// -------------------------------------------------------------------------

#ifndef NFI_NOSQL_DRIVER_MANAGER_H
#define NFI_NOSQL_DRIVER_MANAGER_H

#include "NFComm/NFPluginModule/NFINoSqlDriver.h"

class NFINoSqlDriverManager
{
public:
    virtual bool AddNoSqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort = 6379, const std::string& strAuthKey = "") = 0;
    virtual NFINoSqlDriver* GetNoSqlDriver() = 0;
    virtual void CheckNoSql() = 0;
};

#endif