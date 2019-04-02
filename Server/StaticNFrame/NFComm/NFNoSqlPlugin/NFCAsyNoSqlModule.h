// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCAsyNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_NOSQL_MODULE_H
#define NFC_ASY_NOSQL_MODULE_H

#include "NFComm/NFPluginModule/NFIAsyNoSqlModule.h"

class NFINoSqlDriverManager;
class NFIClassModule;
class NFIElementModule;
class NFILogModule;
class NFIActorModule;

struct SNosqlBaseParam
{
public:
    SNosqlBaseParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ADDSERVER);
    }
public:
    bool PackParam(std::string& strData);
    bool UnPackParam(const std::string& strData);
public:
    NFGUID                  self;
    NFMsg::PackNoSqlParam   xMsg;
    std::string             mStrUseData;
public:
    NOSQL_RETURN_FUNCTOR    mFunReturnRsp;
};

struct SNosqlSaveObjectParam : public SNosqlBaseParam
{
    SNosqlSaveObjectParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_SAVE_OBJECT);
    }
};

struct SNosqlLoadObjectParam : public SNosqlBaseParam
{
    SNosqlLoadObjectParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_LOAD_OBJECT);
    }
};

struct SNosqlDelParam : public SNosqlBaseParam
{
    SNosqlDelParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_DEL);
    }
};

struct SNosqlExistsParam : public SNosqlBaseParam
{
    SNosqlExistsParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXISTS);
    }
};

struct SNosqlExpireParam : public SNosqlBaseParam
{
    SNosqlExpireParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXPIRE);
    }
};

struct SNosqlExpireatParam : public SNosqlBaseParam
{
    SNosqlExpireatParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXPIREAT);
    }
};

struct SNosqlSetParam : public SNosqlBaseParam
{
    SNosqlSetParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_SET);
    }
};

struct SNosqlGetParam : public SNosqlBaseParam
{
    SNosqlGetParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_GET);
    }
};

struct SNosqlSetNXParam : public SNosqlBaseParam
{
    SNosqlSetNXParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_SETNX);
    }
};

struct SNosqlSetEXParam : public SNosqlBaseParam
{
    SNosqlSetEXParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_SETEX);
    }
};

struct SNosqlHSetParam : public SNosqlBaseParam
{
    SNosqlHSetParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_HSET);
    }
};

struct SNosqlHGetParam : public SNosqlBaseParam
{
    SNosqlHGetParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_HGET);
    }
};

struct SNosqlHDelParam : public SNosqlBaseParam
{
    SNosqlHDelParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_HDEL);
    }
};

struct SNosqlHExistsParam : public SNosqlBaseParam
{
    SNosqlHExistsParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_HEXISTS);
    }
};

struct SNosqlZAddParam : public SNosqlBaseParam
{
    SNosqlZAddParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZADD);
    }
};

struct SNosqlZIncrbyParam : public SNosqlBaseParam
{
    SNosqlZIncrbyParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZINCRBY);
    }
};


struct SNosqlZScoreParam : public SNosqlBaseParam
{
    SNosqlZScoreParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZSCORE);
    }
};

struct SNosqlZCardParam : public SNosqlBaseParam
{
    SNosqlZCardParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZCARD);
    }
};

struct SNosqlZRankParam : public SNosqlBaseParam
{
    SNosqlZRankParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANK);
    }
};

struct SNosqlZCountParam : public SNosqlBaseParam
{
    SNosqlZCountParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZCOUNT);
    }
};


struct SNosqlZRevRangeParam : public SNosqlBaseParam
{
    SNosqlZRevRangeParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZREVRANGE);
    }
};

struct SNosqlZRevRankParam : public SNosqlBaseParam
{
    SNosqlZRevRankParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZREVRANK);
    }
};

struct SNosqlZRangeParam : public SNosqlBaseParam
{
    SNosqlZRangeParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANGE);
    }
};

struct SNosqlZRangeByScoreParam : public SNosqlBaseParam
{
    SNosqlZRangeByScoreParam()
    {
        xMsg.set_type(NFIAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANGEBYSCORE);
    }
};

class NFCNoSqlComponent : public NFIComponent
{
public:
    NFCNoSqlComponent() : NFIComponent(GET_CLASS_NAME(NFCNoSqlComponent))
    {
    }

    NFCNoSqlComponent(NFIPluginManager* pPluginManager) : NFIComponent(GET_CLASS_NAME(NFCMysqlComponent))
    {
    }

    virtual ~NFCNoSqlComponent()
    {

    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual int OnASyncEvent(const NFGUID& self, const int from, const int event, std::string& arg);
public:
    int OnASyAddNoSqlServerEvent(const NFGUID& self, const int event, std::string& arg);
    int OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg);
public:
    int OnAsyLoadObjectEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsySaveObjectEvent(const NFGUID& self, const int event, std::string& arg);
public:
    int OnAsyDelEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyExistsEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyExpireEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyExpireatEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsySetEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyGetEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsySetNXEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsySetEXEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyHSetEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyHGetEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyHDelEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyHExistsEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZAddEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZIncrbyEvent(const NFGUID& self, const int event, std::string& arg);

    int OnAsyZScoreEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZCardEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZRankEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZCountEvent(const NFGUID& self, const int event, std::string& arg);

    int OnAsyZRevRangeEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZRevRankEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZRangeEvent(const NFGUID& self, const int event, std::string& arg);
    int OnAsyZRangeByScoreEvent(const NFGUID& self, const int event, std::string& arg);
protected:
    NF_SHARE_PTR<NFINoSqlDriverManager> m_pNoSqlDriverManager;
};

class NFCAsyNoSqlModule
    : public NFIAsyNoSqlModule
{
public:
    NFCAsyNoSqlModule(NFIPluginManager* p);
    virtual ~NFCAsyNoSqlModule();

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool Execute();
    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    virtual bool StartActorPool(const int nCount);
    virtual bool CloseActorPool();

    template<typename ClassParam>
    ClassParam* UnpackResult(const std::string& strMsgData);
public:
    int RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
public:
    int OnBaseNosqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
public:
    virtual int  GetActor();
    virtual bool AddNoSqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort = 6379, const std::string& strAuthKey = "");
    virtual bool KeepAliveMysqlServer();
    virtual bool AddCallBack(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr);
public:
    virtual bool LoadNFObject(const NFGUID& self, const NFMsg::PackNoSqlParam xMsg, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool SaveNFObject(const NFGUID& self, const NFMsg::PackNoSqlParam xMsg, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
public:
    virtual bool Del(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");                                     //Nosql删除一个key
    virtual bool Exists(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");                                  //Nosql查询一个key是否存在
    virtual bool Expire(const NFGUID& self, const std::string& strKey, unsigned int nSecs, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");              //Nosql使key存在nSecs时间
    virtual bool Expireat(const NFGUID& self, const std::string& strKey, unsigned int nUnixTime, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");        ////Nosql使key存在到nUnixTime时间

    ///////////////////////////////////////////////////////////

    virtual bool Set(const NFGUID& self, const std::string& strKey, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");        //设置strKey值为strValue
    virtual bool Get(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");                                     //获得strKey的值

    //SET if Not eXists
    virtual bool SetNX(const NFGUID& self, const std::string& strKey, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    //set key->value and set Expire time
    virtual bool SetEX(const NFGUID& self, const std::string& strKey, const std::string& strValue, const unsigned int nSeconds, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");

    virtual bool HSet(const NFGUID& self, const std::string& strKey, const std::string& strField, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool HGet(const NFGUID& self, const std::string& strKey, const std::string& strField, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");

    virtual bool HExists(const NFGUID& self, const std::string& strKey, const std::string& strField, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool HDel(const NFGUID& self, const std::string& strKey, const std::string& strField, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");

    virtual bool ZAdd(const NFGUID& self, const std::string& strKey, const double nScore, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool ZIncrBy(const NFGUID& self, const std::string& strKey, const std::string& strMember, const double nIncrement, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    //
    //  virtual bool ZRem(const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ZRemRangeByRank(const std::string& strKey, const int nStart, const int nStop, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ZRemRangeByScore(const std::string& strKey, const int nMin, const int nMax, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //
    //
    virtual bool ZScore(const NFGUID& self, const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");

    virtual bool ZCard(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool ZRank(const NFGUID& self, const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool ZCount(const NFGUID& self, const std::string& strKey, const int nMin, const int nMax, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");


    virtual bool ZRevRange(const NFGUID& self, const std::string& strKey, const int nStart, const int nStop, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool ZRevRank(const NFGUID& self, const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool ZRange(const NFGUID& self, const std::string& strKey, const int nStartIndex, const int nEndIndex, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    virtual bool ZRangeByScore(const NFGUID& self, const std::string& strKey, const int nMin, const int nMax, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData = "");
    //
    //  ///////////////////////////////////////////////////////////
    //  //push form back of the list
    //  //pop form head of the list
    //  virtual bool ListPush(const std::string& strKey, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ListPop(const std::string& strKey, std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //
    //  //>= star, < end
    //  virtual bool ListRange(const std::string& strKey, const int nStar, const int nEnd, std::vector<std::string>& xList, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ListLen(const std::string& strKey, int& nLength, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //
    //  virtual bool ListIndex(const std::string& strKey, const int nIndex, std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ListRem(const std::string& strKey, const int nCount, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ListSet(const std::string& strKey, const int nCount, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
    //  virtual bool ListTrim(const std::string& strKey, const int nStar, const int nEnd, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp);
public:
    virtual int ApplyRequest(NF_SHARE_PTR<SNosqlBaseParam> pParam, const int nEvetID);
protected:
    NFIClassModule* m_pClassModule = nullptr;
    NFIElementModule* m_pElementModule = nullptr;
    NFILogModule* m_pLogModule = nullptr;
    NFIActorModule* m_pActorModule = nullptr;
protected:
    int64_t  mLastKeepAliveTime = 0;
    int mnSuitIndex = 0;
    NFMapEx<int, int> mActorList; //actorid <-->Used
protected:
    NFMapEx<int, SNosqlBaseParam> mReqList;// reqID <-->Param
    int nCurReqID = 0;
};

#endif