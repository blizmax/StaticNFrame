// -------------------------------------------------------------------------
//    @FileName         :    NFILuaScriptModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include <vector>
#include <map>
#include <memory>

#include "NFITimerModule.h"
#include "NFServerDefine.h"

class NFHttpRequest;

class NFILuaScriptModule : public NFIModule
{
public:
	NFILuaScriptModule()
	{
	}
public:
	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg) = 0;


	virtual void RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId) = 0;

	virtual void RunHtttpClientLuaFunc(const std::string& luaFunc, const int state_code, const std::string& strRespData, const std::string& strUserData) = 0;

	virtual void RunHttpServerLuaFunc(const std::string& luaFunc, uint32_t serverType, const NFHttpRequest & req) = 0;

	virtual void RunAccountConnectFunc(PlayerAccountInfo* laccount) = 0;
	virtual void RunAccountDisConnectFunc(PlayerAccountInfo* laccount) = 0;

	virtual void RunAccountReConnectFunc(PlayerAccountInfo* laccount) = 0;
public:

	virtual uint32_t AddTimer(const std::string& luaFunc, uint64_t nInterVal, const std::string& useData) = 0;
	virtual uint32_t AddClocker(const std::string& luaFunc, uint64_t nStartTime, uint32_t nInterDays, const std::string& useData) = 0;
	virtual void StopTimer(uint32_t nTimerID) = 0;
	virtual void StopClocker(uint32_t nTimerID) = 0;
};

