// -------------------------------------------------------------------------
//    @FileName         :    NFCLogModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFCore/NFPlatform.h"

#include "common/spdlog/spdlog.h"
#include "common/spdlog/fmt/fmt.h"

#include <unordered_map>
#include <unordered_set>

class NFCLogModule : public NFILogModule
{
public:
	NFCLogModule(NFIPluginManager* p);

	virtual ~NFCLogModule();

	virtual bool Awake() override;
	virtual bool Shut() override;

	/**
	* @brief 对外接口输出默认的LOG
	*
	* @param  log_level log等级
	* @param  logId LOG选项ID，可以配置输出
	* @param  guid 一般是玩家ID，某些情况下，只想输出一个玩家的LOG
	* @param  log
	* @return bool
	*/
	virtual void LogDefault(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log);

	/**
	* @brief 设置默认的LOG的输出等级
	*
	* @param  log_level log等级
	* @return bool
	*/
	virtual void SetDefaultLevel(NF_LOG_LEVEL log_level);

	/**
	* @brief 设置默认的LOG的刷新等级
	*
	* @param  log_level log等级
	* @return bool
	*/
	virtual void SetDefaultFlush(NF_LOG_LEVEL log_level);

	/**
	* @brief 创建别的LOG系统
	*
	* @param  logNameId log文件唯一的ID
	* @param  logName log文件名
	* @param  async 是否异步，多线程
	* @return bool
	*/
	virtual void CreateOthersLogger(uint32_t logNameId, const std::string& logName, bool async);

	/**
	* @brief 通过logNameId，输出LOG
	*
	* @param  logNameId log文件唯一的ID
	* @param  log_level log等级
	* @param  log
	* @return bool
	*/
	virtual void LogOthers(uint32_t logNameId, NF_LOG_LEVEL log_level, const std::string& log);
protected:
	/*创建默认系统LOG系统*/
	void CreateDefaultLogger();
private:
	std::shared_ptr<spdlog::logger> m_defaultLogger;
	std::unordered_map<uint32_t, std::shared_ptr<spdlog::logger>> m_loggerMap;
	std::unordered_set<std::string> m_loggerName;
};


