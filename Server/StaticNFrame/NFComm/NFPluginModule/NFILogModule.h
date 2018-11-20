// -------------------------------------------------------------------------
//    @FileName         :    NFILogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOG_MODULE_H
#define NFI_LOG_MODULE_H

#include "NFIModule.h"
#include "spdlog/fmt/fmt.h"

enum NF_LOG_LEVEL
{
	NLL_TRACE_NORMAL = 0,
	NLL_DEBUG_NORMAL = 1,
	NLL_INFO_NORMAL = 2,
	NLL_WARING_NORMAL = 3,
	NLL_ERROR_NORMAL = 4,
	NLL_FATAL_NORMAL,
};

class NFILogModule
	: public NFIModule
{
public:
public:
	template<typename... ARGS>
	void Log(NF_LOG_LEVEL log_level, const char* function, int line, const char* my_fmt, const ARGS& ... args)
	{
		std::string str = fmt::format(std::string("[{}:{}] | ") + my_fmt, function, line, args...);
		LogNormal(log_level, str);
	}

	virtual void LogNormal(NF_LOG_LEVEL log_level, const std::string& log) = 0;

	virtual void SetLogLevel(NF_LOG_LEVEL log_level) = 0;

	virtual void SetFlushOn(NF_LOG_LEVEL log_level) = 0;

	virtual void LuaDebug(const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogNormal(NLL_DEBUG_NORMAL, strInfo);
	}

	virtual void LuaInfo(const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogNormal(NLL_INFO_NORMAL, strInfo);
	}

	virtual void LuaWarn(const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogNormal(NLL_WARING_NORMAL, strInfo);
	}

	virtual void LuaError(const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogNormal(NLL_ERROR_NORMAL, strInfo);
	}
};

#endif

