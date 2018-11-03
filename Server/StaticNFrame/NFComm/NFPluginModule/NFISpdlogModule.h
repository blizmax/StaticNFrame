// -------------------------------------------------------------------------
//    @FileName         :    NFCSpdlogModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFCSpdlogModule
//    @Desc             :
// -------------------------------------------------------------------------
#pragma once

#include "spdlog/fmt/fmt.h"

#include "NFIModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFISpdlogModule : public NFIModule
{
public:
	template<typename... ARGS>
	void Log(NF_LOG_LEVEL log_level, const char* function, int line, const char* my_fmt, const ARGS& ... args)
	{
		std::string new_fmt = std::string("[{}:{}] | ") + my_fmt;
		std::string str = fmt::format(new_fmt, function, line, args...);
		LogNormal(log_level, str);
	}

	virtual void LogNormal(NF_LOG_LEVEL log_level, const std::string& log) = 0;

	virtual void LuaDebug(const std::string& strInfo)
	{
		LogNormal(NLL_DEBUG_NORMAL, strInfo);
	}

	virtual void LuaInfo(const std::string& strInfo)
	{
		LogNormal(NLL_INFO_NORMAL, strInfo);
	}

	virtual void LuaWarn(const std::string& strInfo)
	{
		LogNormal(NLL_WARING_NORMAL, strInfo);
	}

	virtual void LuaError(const std::string& strInfo)
	{
		LogNormal(NLL_ERROR_NORMAL, strInfo);
	}
};
