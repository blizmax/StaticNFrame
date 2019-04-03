// -------------------------------------------------------------------------
//    @FileName         :    NFCNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFCNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_DATANOSQL_MODULE_H
#define NFC_DATANOSQL_MODULE_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFCore/NFMapEx.hpp"

class NFCNoSqlModule
    : public NFINoSqlModule
{
public:

    NFCNoSqlModule(NFIPluginManager* p);
    virtual ~NFCNoSqlModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool AddConnectSql(const std::string& strID, const std::string& strIP);
    virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort);
    virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass);

    virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriver(const std::string& strID);
    virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuitRandom();
    virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuitConsistent();
    virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuit(const std::string& strHash);
    virtual bool RemoveConnectSql(const std::string& strID);

protected:
    uint64_t mLastCheckTime = 0;

    NFConsistentHashMapEx<std::string, NFINoSqlDriver> mxNoSqlDriver;

};

#endif