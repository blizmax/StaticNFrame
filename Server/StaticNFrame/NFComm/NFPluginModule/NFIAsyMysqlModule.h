// -------------------------------------------------------------------------
//    @FileName         :    NFIAsyMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAsyMysqlModule
//
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#include <vector>

typedef std::function<void(const uint64_t self, const int nRet, const std::string& strUseData)> MYSQL_RETURN_FUNCTOR;
typedef std::function<void(const uint64_t self, const int nRet, const int nValue, const std::string& strUseData)> MYSQL_RETURN_INT_FUNCTOR;
typedef std::function<void(const uint64_t self, const int nRet, const std::vector<std::string>& valueVec, const std::string& strUseData)> MYSQL_RETURN_VECVALUE_FUNCTOR;
typedef std::function<void(const uint64_t self, const int nRet, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const std::string& strUseData)> MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR;

struct SMysqlParam;

class NFIAsyMysqlModule
    : public NFIModule
{
public:
    enum ACOTERMYSQLEVENT
    {
        ACOTERMYSQLEVENT_INISERVER = 2,
        ACOTERMYSQLEVENT_KEEPALIVESERVER = 3,
        ACOTERMYSQLEVENT_UPDATA = 4,
        ACOTERMYSQLEVENT_QUERY = 5,
        ACOTERMYSQLEVENT_SELECT = 6,
        ACOTERMYSQLEVENT_DELETE = 7,
        ACOTERMYSQLEVENT_EXISTS = 8,
        ACOTERMYSQLEVENT_KEYS = 9,
    };

public:
    template<typename BaseType>
    int Updata(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const uint64_t, const int, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        return Updata(self, strRecordName, strKey, fieldVec, valueVec, FunReturnRsp, strUseData);
    }

    template<typename BaseType>
    int Query(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec,
              BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const uint64_t, const int, const std::vector<std::string>&, const std::vector<std::string>&, const std::string&),  const std::string& strUseData, const std::string& strKeyColName = "")
    {
        const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

        return Query(self, strRecordName, strKey, fieldVec, mFunReturnVeckKeyValueRsp, strUseData, strKeyColName);
    }

    template<typename BaseType>
    bool Query(const std::string& strRecordName, int offline, int rows, const std::vector<std::string>& fieldVec,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const uint64_t, const int, const std::vector<std::string>&, const std::vector<std::string>&, const std::string&), const std::string& strUseData, const std::string& strKeyColName = "")
    {
        const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

        return Query(strRecordName, offline, rows, fieldVec, mFunReturnVeckKeyValueRsp, strUseData, strKeyColName);
    }

    template<typename BaseType>
    int Delete(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const uint64_t, const int, const std::string&),  const std::string& strUseData, const std::string& strKeyColName = "")
    {
        const MYSQL_RETURN_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        return Delete(self, strRecordName, strKey, FunReturnRsp, strUseData, strKeyColName);
    }

    template<typename BaseType>
    int Exists(const uint64_t self, const std::string& strRecordName, const std::string& strKey,
               BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const uint64_t, const int, const int, const std::string&),  const std::string& strUseData, const std::string& strKeyColName = "")
    {
        const MYSQL_RETURN_INT_FUNCTOR mFunReturnIntRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        return Exists(self, strRecordName, strKey, mFunReturnIntRsp, strUseData, strKeyColName);
    }

    template<typename BaseType>
    int Keys(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec,
             BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const uint64_t, const int, const std::vector<std::string>&, const std::string&),  const std::string& strUseData)
    {
        const MYSQL_RETURN_VECVALUE_FUNCTOR mFunReturnVecValueRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

        return Keys(self, strRecordName, strKey, mFunReturnVecValueRsp, strUseData);
    }

    virtual bool CloseActorPool() = 0;
    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

public:
    virtual bool StartActorPool(const int nCount) = 0;
    virtual bool Updata(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData) = 0;
    virtual bool Query(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData, const std::string& strKeyColName = "") = 0;
    virtual bool Query(const std::string& strRecordName, int offline, int rows, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData, const std::string& strKeyColName = "") = 0;

    virtual bool Delete(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData, const std::string& strKeyColName = "") = 0;
    virtual bool Exists(const uint64_t self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_INT_FUNCTOR& mFunReturnIntRsp, const std::string& strUseData, const std::string& strKeyColName = "") = 0;
    virtual bool Keys(const uint64_t self, const std::string& strRecordName, const std::string& strKeyName, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnVecValueRsp, const std::string& strUseData) = 0;

};
