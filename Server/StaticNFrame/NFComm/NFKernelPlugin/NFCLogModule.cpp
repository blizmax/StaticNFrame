// -------------------------------------------------------------------------
//    @FileName         :    NFCLogModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-11-3
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFComm/NFCore/NFPlatform.h"
#include "common/spdlog/contrib/sinks/date_and_hour_file_sink.h"
#include "common/spdlog/sinks/ansicolor_sink.h"
#include "NFCLogModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include "my_date_and_hour_file_sink.h"
#include "NFComm/NFCore/NFCommon.h"

#include <iostream>

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
	pPluginManager = p;
	//Create spdlog
	try
	{
		CreateLogger();
	}
	catch (std::system_error& error)
	{
		std::cout << "Create logger failed, error = " << error.what() << std::endl;
	}
}

NFCLogModule::~NFCLogModule()
{
	spdlog::drop_all();
}

bool NFCLogModule::Init()
{
	return true;
}

bool NFCLogModule::Shut()
{
	return true;
}

void NFCLogModule::LogNormal(NF_LOG_LEVEL log_level, const std::string& log)
{
	mdefaultLogger->log((spdlog::level::level_enum)log_level, log.c_str());
}

void NFCLogModule::LogNormal(uint32_t logId, NF_LOG_LEVEL log_level, const std::string& log)
{
	auto iter = m_loggerMap.find(logId);
	if (iter != m_loggerMap.end())
	{
		iter->second->log((spdlog::level::level_enum)log_level, log.c_str());
	}
}

void NFCLogModule::SetLogLevel(NF_LOG_LEVEL log_level)
{
	mdefaultLogger->set_level((spdlog::level::level_enum)(log_level));
}

void NFCLogModule::SetFlushOn(NF_LOG_LEVEL log_level)
{
	mdefaultLogger->flush_on((spdlog::level::level_enum)(log_level));
}

void NFCLogModule::CreateLogger()
{
	std::vector<spdlog::sink_ptr> sinks_vec;
	std::string log_name = NF_FORMAT("{}{}{}{}{}.log", pPluginManager->GetLogPath(), spdlog::details::os::folder_sep, pPluginManager->GetAppName()+lexical_cast<std::string>(pPluginManager->GetAppID()), spdlog::details::os::folder_sep, pPluginManager->GetAppName());
	auto date_and_hour_sink = std::make_shared<spdlog::sinks::my_date_and_hour_file_sink_mt>(log_name);

#if NF_PLATFORM == NF_PLATFORM_WIN
	auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
	sinks_vec.push_back(color_sink);
#else
	if (pPluginManager->IsDaemon() == false)
	{
		auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
		sinks_vec.push_back(color_sink);
	}
#endif

	sinks_vec.push_back(date_and_hour_sink);
#ifdef NF_DEBUG_MODE
	mdefaultLogger = std::make_shared<spdlog::logger>(pPluginManager->GetAppName(), std::begin(sinks_vec), std::end(sinks_vec));
#else
	mdefaultLogger = std::make_shared<spdlog::async_logger>(pPluginManager->GetAppName(), std::begin(sinks_vec), std::end(sinks_vec), 1024,
		spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::milliseconds(1000), nullptr);
#endif

	mdefaultLogger->set_level(spdlog::level::level_enum::trace);

	mdefaultLogger->set_pattern("%^[%l | %Y-%m-%d %H:%M:%S.%e] | %v%$");

	mdefaultLogger->flush_on(spdlog::level::debug);

	spdlog::register_logger(mdefaultLogger);
}

/*创建同步LOG系统*/
void NFCLogModule::CreateLogger(uint32_t logId, const std::string& logName, bool async)
{
	std::vector<spdlog::sink_ptr> sinks_vec;
	std::string log_name = NF_FORMAT("{}{}{}{}{}.log", pPluginManager->GetLogPath(), spdlog::details::os::folder_sep, pPluginManager->GetAppName() + lexical_cast<std::string>(pPluginManager->GetAppID()) + "_" + logName, spdlog::details::os::folder_sep, logName);
	auto date_and_hour_sink = std::make_shared<spdlog::sinks::my_date_and_hour_file_sink_mt>(log_name);

#if NF_PLATFORM == NF_PLATFORM_WIN
	auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
	sinks_vec.push_back(color_sink);
#else
	if (pPluginManager->IsDaemon() == false)
	{
		auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
		sinks_vec.push_back(color_sink);
	}
#endif

	sinks_vec.push_back(date_and_hour_sink);

	std::shared_ptr<spdlog::logger> pLogger;
	if (async)
	{
		pLogger = std::make_shared<spdlog::async_logger>(pPluginManager->GetAppName() + "_" + logName, std::begin(sinks_vec), std::end(sinks_vec), 1024,
			spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::milliseconds(1000), nullptr);
	}
	else
	{
		pLogger = std::make_shared<spdlog::logger>(pPluginManager->GetAppName() + "_" + logName, std::begin(sinks_vec), std::end(sinks_vec));
	}

	pLogger->set_level(spdlog::level::level_enum::trace);

	pLogger->set_pattern("%^[%l | %Y-%m-%d %H:%M:%S.%e] | %v%$");

	pLogger->flush_on(spdlog::level::debug);

	spdlog::register_logger(pLogger);
	m_loggerMap.emplace(logId, pLogger);
}

