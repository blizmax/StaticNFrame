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

	virtual bool Init() override;
	virtual bool Shut() override;

	virtual void LogNormal(NF_LOG_LEVEL log_level, const std::string& log);
	virtual void SetLogLevel(NF_LOG_LEVEL log_level);
	virtual void SetFlushOn(NF_LOG_LEVEL log_level);

	/*创建LOG系统*/
	virtual void CreateLogger(uint32_t logId, const std::string& logName, bool async);

	virtual void LogNormal(uint32_t logId, NF_LOG_LEVEL log_level, const std::string& log);
protected:
	/*创建默认系统LOG系统*/
	void CreateLogger();
private:
	std::shared_ptr<spdlog::logger> mdefaultLogger;
	std::unordered_map<uint32_t, std::shared_ptr<spdlog::logger>> m_loggerMap;
	std::unordered_set<std::string> m_loggerName;
};


