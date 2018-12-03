// -------------------------------------------------------------------------
//    @FileName         :    NFLogMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFCore/NFPlatform.h"
#include "spdlog/fmt/fmt.h"
#include "NFILogModule.h"

class NFLogMgr : public NFSingleton<NFLogMgr>
{
public:
	NFLogMgr();
	virtual ~NFLogMgr();
public:
	bool Init(NFILogModule* pSpdlogModule = nullptr);
	void UnInit();
public:
	template<typename... ARGS>
	void Log(uint32_t log_level, const char* function, int line, const char* my_fmt, const ARGS& ... args)
	{
		if (m_pLogModule)
		{
			std::string str = fmt::format(std::string("[{}:{}] | ") + my_fmt, function, line, args...);
			LogNormal(log_level, str);
		}
	}

	void LogNormal(uint32_t log_level, const std::string& log);
protected:
	NFILogModule* m_pLogModule;
};

#define NFLogDebug(format, ...) NFLogMgr::Instance()->Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, NF_FUNCTION_LINE, format, ##__VA_ARGS__);
#define NFLogInfo(format, ...) NFLogMgr::Instance()->Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, NF_FUNCTION_LINE, format, ##__VA_ARGS__);
#define NFLogWarning(format, ...) NFLogMgr::Instance()->Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, NF_FUNCTION_LINE, format, ##__VA_ARGS__);
#define NFLogError(format, ...) NFLogMgr::Instance()->Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, NF_FUNCTION_LINE, format, ##__VA_ARGS__);
#define NFLogFatal(format, ...) NFLogMgr::Instance()->Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, NF_FUNCTION_LINE, format, ##__VA_ARGS__);

