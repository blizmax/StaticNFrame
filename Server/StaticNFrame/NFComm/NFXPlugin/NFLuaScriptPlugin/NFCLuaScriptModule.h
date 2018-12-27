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
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFILuaModule.h"

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
		mLuaFunc = "";
		mUseData = "";
	}

	uint32_t mTimerId;
	std::string mLuaFunc;
	uint64_t mInterVal;
	uint32_t mCallCount;
	uint32_t mCurCallCount;
	std::string mUseData;
	NFCLuaScriptModule* m_pLuaScriptModule;

	virtual void OnTimer(uint32_t nTimerID) override;
};

#ifndef NF_DYNAMIC_PLUGIN
class NFCLuaScriptModule
    : public NFILuaScriptModule
#else
class NFCLuaScriptModule
	: public NFILuaScriptModule, public NFILuaModule
#endif
{
public:
    NFCLuaScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
		m_luaTimerIndex = 10000;
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
	virtual void RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId) override;
	virtual void RunHtttpClientLuaFunc(const std::string& luaFunc, const int state_code, const std::string& strRespData, const std::string& strUserData) override;
	virtual void RunHttpServerLuaFunc(const std::string& luaFunc, uint32_t serverType, const NFHttpRequest & req) override;
	virtual void RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData) override;
	virtual void RunAccountNetEventLuaFunc(const std::string& luaFunc, uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData) override;
public:
	virtual uint32_t AddTimer(const std::string& luaFunc, uint64_t nInterVal, const std::string& useData) override;
	virtual uint32_t AddClocker(const std::string& luaFunc, uint64_t nStartTime, uint32_t nInterDays, const std::string& useData) override;
	virtual void StopTimer(uint32_t nTimerID) override;
	virtual void StopClocker(uint32_t nTimerID) override;
public:
    bool Register();
	void LoadScript();
protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventModule* m_pEventModule;
protected:
    int64_t mnTime;
protected:
	std::map<uint64_t, NFLuaTimer*> m_luaTimerMap;
	std::list<NFLuaTimer*> m_luaTimerList;
	uint32_t m_luaTimerIndex;
};

