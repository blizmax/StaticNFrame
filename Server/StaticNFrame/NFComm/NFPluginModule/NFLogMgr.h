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

/**
* @brief 服务器分层架构，这里只能填写系统引擎的LOG
*
*/
enum NF_LOG_ID
{
	NF_LOG_DEFAULT = 0,						//默认LOG
	NF_LOG_SYSTEMLOG = 1,					//系统LOG
	NF_LOG_ACTOR_PLUGIN = 2,				//Actor 引擎 
	NF_LOG_KERNEL_PLUGIN = 3,				//kernel 引擎
	NF_LOG_LUA_PLUGIN = 4,					//lua 引擎
	NF_LOG_MONGO_PLUGIN = 5,				//mongo 引擎
	NF_LOG_MONITOR_PLUGIN = 5,				//monitor 引擎
	NF_LOG_MYSQL_PLUGIN = 7,				//mysql 引擎
	NF_LOG_NET_PLUGIN = 8,					//net 引擎
	NF_LOG_SQLITE_PLUGIN = 9,				//sqlite 引擎
	NF_LOG_TEST_PLUGIN = 10,				//test 引擎
	NF_LOG_PLUGIN_MANAGER = 100,				//引擎加载器
	NF_LOG_MAX_SYSTEM_PLUGIN = 100,
	NF_LOG_MAX_ID = 10240, //最大LOGID
};

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
			std::string str = fmt::format(std::string("[{}:{}] | [{}:{}] |") + my_fmt, function, line, logId, guid, args...);
			LogDefault(log_level, logId, guid, str);
		}
	}

	/**
	* @brief 对外接口输出默认的LOG
	*
	* @param  log_level log等级
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	void LogDefault(uint32_t log_level, uint32_t logId, uint64_t guid, const std::string& log);

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

