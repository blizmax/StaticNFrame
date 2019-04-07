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
#include "common/spdlog/fmt/fmt.h"
#include "NFILogModule.h"

class _NFExport NFLogMgr : public NFSingleton<NFLogMgr>
{
public:
	NFLogMgr();
	virtual ~NFLogMgr();
public:
	bool Init(NFILogModule* pSpdlogModule = nullptr);
	void UnInit();
public:
	/**
	* @brief 对外接口输出默认的LOG
	*
	* @param  log_level log等级
	* @param  function log所在函数
	* @param  line log所在文件行
	* @return bool
	*/
	template<typename... ARGS>
	void Log(uint32_t log_level, const char* function, int line, uint32_t logId, uint64_t guid, const char* my_fmt, const ARGS& ... args)
	{
		if (m_pLogModule)
		{
			std::string str = fmt::format(my_fmt, args...);
			m_pLogModule->LogDefault((NF_LOG_LEVEL)log_level, function, line, logId, guid, str);
		}
	}

	virtual bool IsLogIdEnable(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid)
	{
		if (m_pLogModule)
		{
			return m_pLogModule->IsLogIdEnable(log_level, logId, guid);
		}
		return false;
	}

	/**
	* @brief 设置默认的LOG的输出等级
	*
	* @param  log_level log等级
	* @return bool
	*/
	void SetDefaultLevel(NF_LOG_LEVEL log_level);

	/**
	* @brief 设置默认的LOG的刷新等级
	*
	* @param  log_level log等级
	* @return bool
	*/
	void SetDefaultFlush(NF_LOG_LEVEL log_level);
protected:
	NFILogModule* m_pLogModule;
};

#define NFLogDebug(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_DEBUG_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogInfo(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_INFO_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogWarning(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_WARING_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogError(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_ERROR_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogDebugEnable(logID, guid) NFLogMgr::Instance()->IsLogIdEnable(NLL_DEBUG_NORMAL, logID, guid)
#define NFLogInfoEnable(logID, guid) NFLogMgr::Instance()->IsLogIdEnable(NLL_INFO_NORMAL, logID, guid)
#define NFLogWarningEnable(logID, guid) NFLogMgr::Instance()->IsLogIdEnable(NLL_WARING_NORMAL, logID, guid)
#define NFLogErrorEnable(logID, guid) NFLogMgr::Instance()->IsLogIdEnable(NLL_ERROR_NORMAL, logID, guid)

