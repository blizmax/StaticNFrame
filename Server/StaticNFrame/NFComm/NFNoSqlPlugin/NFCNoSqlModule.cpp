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
        bool ret = pNoSqlDriver->Connect(strIP, 6379, "");
        mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
		return ret;
    }

    return false;
}

bool NFCNoSqlModule::AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort)
{
    if (!mxNoSqlDriver.ExistElement(strID))
    {
        NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver(new NFCNoSqlDriver());
        bool ret = pNoSqlDriver->Connect(strIP, nPort, "");
        mxNoSqlDriver.AddElement(strID, pNoSqlDriver);
		return ret;
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

const bool NFCNoSqlModule::Del(const std::string& strKey)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Del(strKey);
	}
	return false;
}

const bool NFCNoSqlModule::Exists(const std::string& strKey)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Exists(strKey);
	}
	return false;
}

const bool NFCNoSqlModule::Expire(const std::string& strKey, unsigned int nSecs)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Expire(strKey, nSecs);
	}
	return false;
}

const bool NFCNoSqlModule::Expireat(const std::string& strKey, unsigned int nUnixTime)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Expireat(strKey, nUnixTime);
	}
	return false;
}

///////////////////////////////////////////////////////////

const bool NFCNoSqlModule::Set(const std::string& strKey, const std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Set(strKey, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::Get(const std::string& strKey, std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Get(strKey, strValue);
	}
	return false;
}

///////////////////////////////////////////////////////////

//SET if Not eXists
const bool NFCNoSqlModule::SetNX(const std::string& strKey, const std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->SetNX(strKey, strValue);
	}
	return false;
}

//set key->value and set Expire time
const bool NFCNoSqlModule::SetEX(const std::string& strKey, const std::string& strValue, const unsigned int nSeconds)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->SetEX(strKey, strValue, nSeconds);
	}
	return false;
}

const bool NFCNoSqlModule::HSet(const std::string& strKey, const std::string& strField, const std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HSet(strKey, strField, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::HGet(const std::string& strKey, const std::string& strField, std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HGet(strKey, strField, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::HMSet(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HMSet(strKey, fieldVec, valueVec);
	}
	return false;
}

const bool NFCNoSqlModule::HMGet(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HMGet(strKey, fieldVec, valueVec);
	}
	return false;
}


const bool NFCNoSqlModule::HExists(const std::string& strKey, const std::string& strField)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HExists(strKey, strField);
	}
	return false;
}

const bool NFCNoSqlModule::HDel(const std::string& strKey, const std::string& strField)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HDel(strKey, strField);
	}
	return false;
}

const bool NFCNoSqlModule::HLength(const std::string& strKey, int& nLen)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HLength(strKey, nLen);
	}
	return false;
}


const bool NFCNoSqlModule::HKeys(const std::string& strKey, std::vector<std::string>& fieldVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HKeys(strKey, fieldVec);
	}
	return false;
}

const bool NFCNoSqlModule::HValues(const std::string& strKey, std::vector<std::string>& valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HValues(strKey, valueVec);
	}
	return false;
}

const bool NFCNoSqlModule::HGetAll(const std::string& strKey, std::vector<std::pair<std::string, std::string> >& valueVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->HGetAll(strKey, valueVec);
	}
	return false;
}


/////////////
const bool NFCNoSqlModule::Incr(const std::string& strKey, int64_t& value)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->Incr(strKey, value);
	}
	return false;
}

const bool NFCNoSqlModule::ZAdd(const std::string& strKey, const double nScore, const std::string& strMember)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZAdd(strKey, nScore, strMember);
	}
	return false;
}

const bool NFCNoSqlModule::ZIncrBy(const std::string& strKey, const std::string& strMember, const double nIncrement)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZIncrBy(strKey, strMember, nIncrement);
	}
	return false;
}

const bool NFCNoSqlModule::ZRem(const std::string& strKey, const std::string& strMember)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRem(strKey, strMember);
	}
	return false;
}

const bool NFCNoSqlModule::ZRemRangeByRank(const std::string& strKey, const int nStart, const int nStop)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRemRangeByRank(strKey, nStart, nStop);
	}
	return false;
}

const bool NFCNoSqlModule::ZRemRangeByScore(const std::string& strKey, const int nMin, const int nMax)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRemRangeByScore(strKey, nMin, nMax);
	}
	return false;
}

const bool NFCNoSqlModule::ZScore(const std::string& strKey, const std::string& strMember, double& nScore)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZScore(strKey, strMember, nScore);
	}
	return false;
}

const bool NFCNoSqlModule::ZCard(const std::string& strKey, int& nCount)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZCard(strKey, nCount);
	}
	return false;
}

const bool NFCNoSqlModule::ZRank(const std::string& strKey, const std::string& strMember, int& nRank)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRank(strKey, strMember, nRank);
	}
	return false;
}

const bool NFCNoSqlModule::ZCount(const std::string& strKey, const int nMin, const int nMax, int& nCount)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZCount(strKey, nMin, nMax, nCount);
	}
	return false;
}


const bool NFCNoSqlModule::ZRevRange(const std::string& strKey, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRevRange(strKey, nStart, nStop, memberScoreVec);
	}
	return false;
}

const bool NFCNoSqlModule::ZRevRank(const std::string& strKey, const std::string& strMember, int& nRank)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRevRank(strKey, strMember, nRank);
	}
	return false;
}

const bool NFCNoSqlModule::ZRange(const std::string& strKey, const int nStartIndex, const int nEndIndex, std::vector<std::pair<std::string, double> >& memberScoreVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRange(strKey, nStartIndex, nEndIndex, memberScoreVec);
	}
	return false;
}

const bool NFCNoSqlModule::ZRangeByScore(const std::string& strKey, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ZRangeByScore(strKey, nMin, nMax, memberScoreVec);
	}
	return false;
}

///////////////////////////////////////////////////////////
//push form back of the list
//pop form head of the list
const bool NFCNoSqlModule::ListPush(const std::string& strKey, const std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListPush(strKey, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::ListPop(const std::string& strKey, std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListPop(strKey, strValue);
	}
	return false;
}

//>= star, < end
const bool NFCNoSqlModule::ListRange(const std::string& strKey, const int nStar, const int nEnd, std::vector<std::string>& xList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListRange(strKey, nStar, nEnd, xList);
	}
	return false;
}

const bool NFCNoSqlModule::ListLen(const std::string& strKey, int& nLength)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListLen(strKey, nLength);
	}
	return false;
}

const bool NFCNoSqlModule::ListIndex(const std::string& strKey, const int nIndex, std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListIndex(strKey, nIndex, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::ListRem(const std::string& strKey, const int nCount, const std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListRem(strKey, nCount, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::ListSet(const std::string& strKey, const int nCount, const std::string& strValue)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListSet(strKey, nCount, strValue);
	}
	return false;
}

const bool NFCNoSqlModule::ListTrim(const std::string& strKey, const int nStar, const int nEnd)
{
	NF_SHARE_PTR<NFINoSqlDriver> pDriver = GetDriverBySuitConsistent();
	if (pDriver)
	{
		return pDriver->ListTrim(strKey, nStar, nEnd);
	}
	return false;
}
