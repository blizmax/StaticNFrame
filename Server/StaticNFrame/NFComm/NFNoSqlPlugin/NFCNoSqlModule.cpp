// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataNoSqlModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCNoSqlModule.h"
#include "NFMessageDefine/NFMsgDefine.h"

NFCNoSqlModule::NFCNoSqlModule(NFIPluginManager* p)
{
    m_pPluginManager = p;
}

NFCNoSqlModule::~NFCNoSqlModule()
{

}

bool NFCNoSqlModule::Init()
{
    mLastCheckTime = 0;

    return true;
}

bool NFCNoSqlModule::Shut()
{

    return true;
}

bool NFCNoSqlModule::AfterInit()
{
    return true;
}

bool NFCNoSqlModule::Execute()
{
    NF_SHARE_PTR<NFINoSqlDriver> xNosqlDriver = this->mxNoSqlDriver.First();
    while (xNosqlDriver)
    {
        if (!xNosqlDriver->Enable())
        {
            xNosqlDriver->ReConnect();
        }

        xNosqlDriver = this->mxNoSqlDriver.Next();
    }

    return true;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuitRandom()
{
    NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuitRandom();
    if (xDriver && xDriver->Enable())
    {
        return xDriver;
    }

    return nullptr;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuitConsistent()
{
    NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuitConsistent();
    if (xDriver && xDriver->Enable())
    {
        return xDriver;
    }

    return nullptr;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriverBySuit(const std::string& strHash)
{
    NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElementBySuit(strHash);
    if (xDriver && xDriver->Enable())
    {
        return xDriver;
    }

    return nullptr;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP)
{
    if (!mxNoSqlDriver.ExistElement(strID))
    {
        NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
        pNoSqlDriver->Connect(strIP, 6379, "");
        return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
    }

    return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort)
{
    if (!mxNoSqlDriver.ExistElement(strID))
    {
        NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
        pNoSqlDriver->Connect(strIP, nPort, "");
        return mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
    }

    return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass)
{
    if (!mxNoSqlDriver.ExistElement(strID))
    {
        NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
        if (pNoSqlDriver->Connect(strIP, nPort, strPass))
        {
            mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

NF_SHARE_PTR<NFINoSqlDriver> NFCNoSqlModule::GetDriver(const std::string& strID)
{
    NF_SHARE_PTR<NFINoSqlDriver> xDriver = mxNoSqlDriver.GetElement(strID);
    if (xDriver && xDriver->Enable())
    {
        return xDriver;
    }

    return nullptr;
}

bool NFCNoSqlModule::RemoveConnectSql(const std::string& strID)
{
    return mxNoSqlDriver.RemoveElement(strID);
}
