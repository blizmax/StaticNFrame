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
#include "common/spdlog/fmt/fmt.h"

class LogInfoConfig;

enum NF_LOG_LEVEL
{
	NLL_TRACE_NORMAL = 0,
	NLL_DEBUG_NORMAL = 1,
	NLL_INFO_NORMAL = 2,
	NLL_WARING_NORMAL = 3,
	NLL_ERROR_NORMAL = 4,
	NLL_CRITICAL_NORMAL = 5,
	NLL_OFF_NORMAL = 6,
};

/**
* @brief 服务器分层架构，这里只能填写系统引擎的LOG
*
*/
enum NF_LOG_ID
{
	//0-100是基础框架层LOG
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

	//-----------------------------------------------------------
	NF_LOG_LOAD_CONFIG = 21,				//加载配置
	NF_LOG_PROTOBUF_PARSE = 22,				//protobuf解析出错
	NF_LOG_RECV_MSG = 23,					//接受消息
	NF_LOG_RECV_MSG_JSON_PRINTF = 24,		//接受消息Json输出
	NF_LOG_PLUGIN_MANAGER = 100,			//引擎加载器
	NF_LOG_MAX_SYSTEM_PLUGIN = 100,
	//-----------------------------------------------------------
	NF_LOG_BEGIN_SERVER_LOG = 101,			//101-1000是系统框架层LOG
	NF_LOG_END_SERVER_LOG = 1000,			//
	//-----------------------------------------------------------
	NF_LOG_BEGIN_LOGIC_LOG = 1001,			//1001以后是逻辑框架层

	NF_LOG_BEHAVIOR_LOGIC_LOG = 1002,		//行为LOG日志

	NF_LOG_MAX_ID = 10240, //最大LOGID
};

class NFILogModule
	: public NFIModule
{
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
	void Log(NF_LOG_LEVEL log_level, const char* function, int line, uint32_t logId, uint64_t guid, const char* my_fmt, const ARGS& ... args)
	{
		std::string str = fmt::format(my_fmt, args...);
		LogDefault(log_level, function, line, logId, guid, str);
	}

	/**
	* @brief 对外接口输出默认的LOG
	*
	* @param  log_level log等级
	* @param  function
	* @param  line
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LogDefault(NF_LOG_LEVEL log_level, const char* function, int line, uint32_t logId, uint64_t guid, const std::string& log) = 0;

	/**
	* @brief 对外接口输出默认的LOG
	*
	* @param  log_level log等级
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LogDefault(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log) = 0;

	/**
	* @brief 是否输出对应等级的logId的log
	*
	* @param  log_level log等级
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @return bool
	*/
	virtual bool IsLogIdEnable(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid) = 0;

	/**
	* @brief 设置默认的LOG的输出等级
	*
	* @param  log_level log等级
	* @return bool
	*/
	virtual void SetDefaultLevel(NF_LOG_LEVEL log_level) = 0;

	/**
	* @brief 设置默认的LOG的刷新等级
	*
	* @param  log_level log等级
	* @return bool
	*/
	virtual void SetDefaultFlush(NF_LOG_LEVEL log_level) = 0;

	/**
	* @brief 设置log的配置
	*
	* @param  vecLogConfig
	* @return
	*/
	virtual void SetDefaultLogConfig(const std::vector<LogInfoConfig>& vecLogConfig) = 0;

	/**
	* @brief 创建别的LOG系统
	*
	* @param  logNameId log文件唯一的ID
	* @param  logName log文件名
	* @param  async 是否异步，多线程
	* @return bool
	*/
	virtual void CreateOthersLogger(uint32_t logNameId, const std::string& logName, bool async) = 0;
	
	/**
	* @brief 通过logNameId，输出LOG
	*
	* @param  logNameId log文件唯一的ID
	* @param  log_level log等级
	* @param  log
	* @return bool
	*/
	virtual void LogOthers(uint32_t logNameId, NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log) = 0;

	/**
	* @brief 用于LUA系统的LOG
	*
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaDebug(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_DEBUG_NORMAL, logId, guid, strInfo);
	}

	/**
	* @brief 用于LUA系统的LOG
	*
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaInfo(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_INFO_NORMAL, logId, guid, strInfo);
	}

	/**
	* @brief 用于LUA系统的LOG
	*
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaWarn(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_WARING_NORMAL, logId, guid, strInfo);
	}

	/**
	* @brief 用于LUA系统的LOG
	*
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaError(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_ERROR_NORMAL, logId, guid, strInfo);
	}
};

#endif

