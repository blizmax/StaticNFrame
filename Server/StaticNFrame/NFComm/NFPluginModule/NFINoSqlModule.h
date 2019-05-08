// -------------------------------------------------------------------------
//    @FileName         :    NFINoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFINoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFI_NOSQL_MODULE_H
#define NFI_NOSQL_MODULE_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFINoSqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"

class NFINoSqlModule
    : public NFIModule
{
public:

    virtual bool AddConnectSql(const std::string& strID, const std::string& strIP) = 0;
    virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort) = 0;
    virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass) = 0;

    virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriver(const std::string& strID) = 0;
    virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuitRandom() = 0;
    virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuitConsistent() = 0;
    virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuit(const std::string& strHash) = 0;
    //virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuit(const int nHash) = 0;
    virtual bool RemoveConnectSql(const std::string& strID) = 0;

	virtual const bool Del(const std::string& strKey) = 0;
	virtual const bool Exists(const std::string& strKey) = 0;
	virtual const bool Expire(const std::string& strKey, unsigned int nSecs) = 0;
	virtual const bool Expireat(const std::string& strKey, unsigned int nUnixTime) = 0;

	///////////////////////////////////////////////////////////

	virtual const bool Set(const std::string& strKey, const std::string& strValue) = 0;
	virtual const bool Get(const std::string& strKey, std::string& strValue) = 0;

	///////////////////////////////////////////////////////////

	//SET if Not eXists
	virtual const bool SetNX(const std::string& strKey, const std::string& strValue) = 0;
	//set key->value and set Expire time
	virtual const bool SetEX(const std::string& strKey, const std::string& strValue, const unsigned int nSeconds) = 0;

	virtual const bool HSet(const std::string& strKey, const std::string& strField, const std::string& strValue) = 0;
	virtual const bool HGet(const std::string& strKey, const std::string& strField, std::string& strValue) = 0;
	virtual const bool HMSet(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) = 0;
	virtual const bool HMGet(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;

	virtual const bool HExists(const std::string& strKey, const std::string& strField) = 0;
	virtual const bool HDel(const std::string& strKey, const std::string& strField) = 0;
	virtual const bool HLength(const std::string& strKey, int& nLen) = 0;

	virtual const bool HKeys(const std::string& strKey, std::vector<std::string>& fieldVec) = 0;
	virtual const bool HValues(const std::string& strKey, std::vector<std::string>& valueVec) = 0;
	virtual const bool HGetAll(const std::string& strKey, std::vector<std::pair<std::string, std::string> >& valueVec) = 0;

	/////////////
	virtual const bool Incr(const std::string& strKey, int64_t& value) = 0;

	virtual const bool ZAdd(const std::string& strKey, const double nScore, const std::string& strMember) = 0;
	virtual const bool ZIncrBy(const std::string& strKey, const std::string& strMember, const double nIncrement) = 0;

	virtual const bool ZRem(const std::string& strKey, const std::string& strMember) = 0;
	virtual const bool ZRemRangeByRank(const std::string& strKey, const int nStart, const int nStop) = 0;
	virtual const bool ZRemRangeByScore(const std::string& strKey, const int nMin, const int nMax) = 0;


	virtual const bool ZScore(const std::string& strKey, const std::string& strMember, double& nScore) = 0;


	virtual const bool ZCard(const std::string& strKey, int& nCount) = 0;
	virtual const bool ZRank(const std::string& strKey, const std::string& strMember, int& nRank) = 0;
	virtual const bool ZCount(const std::string& strKey, const int nMin, const int nMax, int& nCount) = 0;


	virtual const bool ZRevRange(const std::string& strKey, const int nStart, const int nStop, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;
	virtual const bool ZRevRank(const std::string& strKey, const std::string& strMember, int& nRank) = 0;
	virtual const bool ZRange(const std::string& strKey, const int nStartIndex, const int nEndIndex, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;
	virtual const bool ZRangeByScore(const std::string& strKey, const int nMin, const int nMax, std::vector<std::pair<std::string, double> >& memberScoreVec) = 0;

	///////////////////////////////////////////////////////////
	//push form back of the list
	//pop form head of the list
	virtual const bool ListPush(const std::string& strKey, const std::string& strValue) = 0;
	virtual const bool ListPop(const std::string& strKey, std::string& strValue) = 0;

	//>= star, < end
	virtual const bool ListRange(const std::string& strKey, const int nStar, const int nEnd, std::vector<std::string>& xList) = 0;
	virtual const bool ListLen(const std::string& strKey, int& nLength) = 0;

	virtual const bool ListIndex(const std::string& strKey, const int nIndex, std::string& strValue) = 0;
	virtual const bool ListRem(const std::string& strKey, const int nCount, const std::string& strValue) = 0;
	virtual const bool ListSet(const std::string& strKey, const int nCount, const std::string& strValue) = 0;
	virtual const bool ListTrim(const std::string& strKey, const int nStar, const int nEnd) = 0;
};

#endif