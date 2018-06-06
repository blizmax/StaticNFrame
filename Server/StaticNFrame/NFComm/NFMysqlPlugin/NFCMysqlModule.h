// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCMysqlModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCMysqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"

class NFCMysqlModule
    : public NFIMysqlModule
{
public:
    NFCMysqlModule(NFIPluginManager* p);
    virtual ~NFCMysqlModule();

    virtual bool Init() override;
    virtual bool Shut() override;
    virtual bool Execute() override;
    virtual bool AfterInit() override;

    //////////////////////////////////////////////////////////////////////////
    virtual bool Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) override;
    virtual bool Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const std::string& strKeyColName = "") override;
    virtual bool Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const std::string& strKeyColName = "") override;

    virtual bool Delete(const std::string& strRecordName, const std::string& strKey, const std::string& strKeyColName = "") override;
    virtual bool Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit, const std::string& strKeyColName = "") override;
    virtual bool Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec) override;
    virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) override;
private:
    NFIMysqlDriverManager* m_pMysqlDriverManager;

    uint64_t mnLastCheckTime;
};