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

class NFHttpHandle;

#ifndef NF_DYNAMIC_PLUGIN
#include "NFComm/NFPluginModule/NFILuaModule.h"
class NFILuaScriptModule : public NFIModule, public NFILuaModule
#else
class NFILuaScriptModule : public NFIModule
#endif
{
public:
	NFILuaScriptModule()
	{
	}
public:
	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg) = 0;

	virtual void RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId) = 0;

	virtual void RunHtttpClientLuaFunc(const std::string& luaFunc, const int state_code, const std::string& strRespData, const std::string& strUserData) = 0;

	virtual void RunHttpServerLuaFunc(const std::string& luaFunc, uint32_t serverType, const NFHttpHandle & req) = 0;

	virtual void RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData) = 0;

	virtual void RunAccountNetEventLuaFunc(const std::string& luaFunc, uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData) = 0;
};

