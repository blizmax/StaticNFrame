// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIScriptModule.h"

#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFILuaModule.h"
#include "NFComm/NFPluginModule/NFINetServerModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"


#include "NFComm/NFCore/NFMapEx.hpp"

class NFCLuaScriptModule;

class NFLuaTimer : public NFTimerObj
{
public:
	NFLuaTimer(NFCLuaScriptModule* p)
	{
		Clear();
		m_pLuaScriptModule = p;
	}

	void Clear()
	{
		mTimerId = 0;
		mInterVal = 0;
		mCallCount = 0;
		mCurCallCount = 0;
	}

	uint32_t mTimerId;
	std::string mGlobalLuaFunc;
	std::string mLuaFunc;
	uint64_t mInterVal;
	uint32_t mCallCount;
	uint32_t mCurCallCount;
	NFLuaRef mDataStr;
	std::string mTmpStr;
	NFCLuaScriptModule* m_pLuaScriptModule;

	virtual void OnTimer(uint32_t nTimerID) override;
};

class NFCLuaScriptModule
	: public NFILuaScriptModule, public NFILuaModule
{
public:
    NFCLuaScriptModule(NFIPluginManager* p)
    {
        m_pPluginManager = p;
		m_luaTimerIndex = 10000;
		m_pNetServerModule = nullptr;
		m_pNetClientModule = nullptr;
    }
public:

    virtual bool Init();
    virtual bool AfterInit();
	virtual bool ReadyExecute();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Finalize();
public:
	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg) override;
	virtual void RunHttpRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg) override;
public:
	virtual uint32_t AddTimer(const std::string& luaFunc, uint64_t nInterVal, uint32_t nCallCount, const NFLuaRef& dataStr);
	virtual uint32_t AddClocker(const std::string& luaFunc, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount, const NFLuaRef& dataStr);
	virtual void StopTimer(uint32_t nTimerID);
	virtual void StopClocker(uint32_t nTimerID);
public:
	virtual const std::string& GetAppName() const;
	virtual int GetAppID() const;
	virtual uint64_t GetInitTime() const;
	virtual uint64_t GetNowTime() const;
	virtual std::string GetMD5(const std::string& str);
	virtual uint32_t GetCRC32(const std::string& s);
	virtual uint16_t GetCRC16(const std::string& s);
	virtual std::string Base64Encode(const std::string& s);
	virtual std::string Base64Decode(const std::string& s);
	virtual uint64_t GetMsecTime() const;
	virtual uint64_t GetSecTime() const;

	virtual void SendMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToManyPlayer(const std::vector<uint64_t>& nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);

	virtual void SetDefaultLevel(uint32_t log_level);

	virtual void SetDefaultFlush(uint32_t log_level);

	virtual void LuaDebug(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void LuaInfo(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void LuaWarn(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void LuaError(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void ProcessLoop(const std::string& dataStr);

	virtual void ProcessWork(const std::string& luaFunc, const NFLuaRef& dataStr);

	virtual void ProcessTimer(uint32_t timeSec, const std::string& luaFunc, const NFLuaRef& dataStr);

	virtual void ProcessLoopTimer(uint32_t timeSec, const std::string& luaFunc, const NFLuaRef& dataStr);

	virtual void BeginProfiler(const std::string& luaFunc);
	virtual uint64_t EndProfiler();//return this time cost time(us) ΢��

	virtual void ReloadAllLuaFiles() override;
	virtual void ReloadLuaFiles() override;
	virtual void ReloadLuaFiles(const std::vector<std::string>& vecStr) override;
	virtual void GcStep();

	virtual std::string Sha256(const std::string& s);
public:
	void OnAccountEventCallBack(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData);
	NF_SHARE_PTR<PlayerGameServerInfo> GetPlayerInfo(uint64_t playerId);
public:
    bool Register();
	void LoadScript();
protected:
	NFINetServerModule* m_pNetServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFILogModule* m_pLogModule;
protected:
    int64_t mnTime;
protected:
	std::map<uint64_t, NFLuaTimer*> m_luaTimerMap;
	std::list<NFLuaTimer*> m_luaTimerList;
	uint32_t m_luaTimerIndex;

	NFMapEx<uint64_t, PlayerGameServerInfo> mPlayerProxyInfoMap;
};

