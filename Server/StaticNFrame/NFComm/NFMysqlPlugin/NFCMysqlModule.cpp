// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/05
//    @Email			:    445267987@qq.com
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlModule.h"
#include "NFCMysqlDriverManager.h"

NFCMysqlModule::NFCMysqlModule(NFIPluginManager* p)
{
    pPluginManager = p;
    mnLastCheckTime = 0;
    m_pMysqlDriverManager = NF_NEW NFCMysqlDriverManager();
}

NFCMysqlModule::~NFCMysqlModule()
{
	NFSafeDelete(m_pMysqlDriverManager);
}

bool NFCMysqlModule::Init()
{
    return true;
}

bool NFCMysqlModule::Shut()
{
    return true;
}

bool NFCMysqlModule::AfterInit()
{
    return true;
}

bool NFCMysqlModule::Updata(const google::protobuf::Message& message)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Updata(message);
    }

    return false;
}

bool NFCMysqlModule::Execute()
{
    if (mnLastCheckTime + 10 > (uint64_t)NFGetSecondTime())
    {
        return true;
    }

    mnLastCheckTime = (uint64_t)NFGetSecondTime();

    if (m_pMysqlDriverManager)
    {
        m_pMysqlDriverManager->CheckMysql();
    }

    return true;
}

bool NFCMysqlModule::AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
    if (!m_pMysqlDriverManager)
    {
        return false;
    }

    return m_pMysqlDriverManager->AddMysqlServer(nServerID, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
}

bool NFCMysqlModule::Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Updata(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool NFCMysqlModule::Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const std::string& strKeyColName)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Query(strRecordName, strKey, fieldVec, valueVec, strKeyColName);
    }

    return false;
}

bool NFCMysqlModule::Delete(const std::string& strRecordName, const std::string& strKey, const std::string& strKeyColName)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Delete(strRecordName, strKey, strKeyColName);
    }

    return false;
}

bool NFCMysqlModule::Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit, const std::string& strKeyColName)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Exists(strRecordName, strKey, bExit, strKeyColName);
    }

    return false;
}

bool NFCMysqlModule::Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const std::string& strKeyColName)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Select(strRecordName, strKey, fieldVec, valueVec, strKeyColName);
    }

    return false;
}

bool NFCMysqlModule::Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Keys(strRecordName, strKeyName, valueVec);
    }

    return false;
}
