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
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"

#include <iostream>

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	//ֻҪ��Ϊ��Ч�ʣ��˷ѵ��ڴ�
	m_logInfoConfig.resize(NF_LOG_MAX_ID);
	//Create spdlog
	try
	{
		CreateDefaultLogger();
	}
	catch (std::system_error& error)
	{
		std::cout << "Create logger failed, error = " << error.what() << std::endl;
		assert(false);
	}

	NFLogMgr::Instance()->Init(this);
}

NFCLogModule::~NFCLogModule()
{
	spdlog::drop_all();
}

bool NFCLogModule::Awake()
{
	SetDefaultLevel((NF_LOG_LEVEL)NFConfigMgr::Instance()->GetLogLevel());
	SetDefaultFlush((NF_LOG_LEVEL)NFConfigMgr::Instance()->GetLogFlushLevel());
	SetDefaultLogConfig(NFConfigMgr::Instance()->GetLogInfoConfig());
	return true;
}

bool NFCLogModule::Shut()
{
	return true;
}

bool NFCLogModule::OnReloadPlugin()
{
	SetDefaultLevel((NF_LOG_LEVEL)NFConfigMgr::Instance()->GetLogLevel());
	SetDefaultFlush((NF_LOG_LEVEL)NFConfigMgr::Instance()->GetLogFlushLevel());
	SetDefaultLogConfig(NFConfigMgr::Instance()->GetLogInfoConfig());
	return true;
}

void NFCLogModule::LogDefault(NF_LOG_LEVEL log_level, const char* function, int line, uint32_t logId, uint64_t guid, const std::string& log)
{
	if (logId < m_logInfoConfig.size() && m_logInfoConfig[logId].mDisplay && m_logInfoConfig[logId].mLevel <= (uint32_t)log_level)
	{
		if (m_logInfoConfig[logId].mVecGuid.empty() || m_logInfoConfig[logId].Exist(guid))
		{
			std::string str = fmt::format("[{}:{}] |[{}:{}] | {}", function, line, m_logInfoConfig[logId].mLogName, guid, log);
			m_defaultLogger->log((spdlog::level::level_enum)log_level, str.c_str());
		}
	}
}

/**
* @brief ����ӿ����Ĭ�ϵ�LOG
*
* @param  log_level log�ȼ�
* @param  logId LOGѡ��ID�������������
* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
* @param  log
* @return bool
*/
void NFCLogModule::LogDefault(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log)
{
	if (logId < m_logInfoConfig.size() && m_logInfoConfig[logId].mDisplay && m_logInfoConfig[logId].mLevel <= (uint32_t)log_level)
	{
		if (m_logInfoConfig[logId].mVecGuid.empty() || m_logInfoConfig[logId].Exist(guid))
		{
			std::string str = fmt::format("|[{}:{}] | {}", m_logInfoConfig[logId].mLogName, guid, log);
			m_defaultLogger->log((spdlog::level::level_enum)log_level, str.c_str());
		}
	}
}

void NFCLogModule::LogOthers(uint32_t logNameId, NF_LOG_LEVEL log_level, const std::string& log)
{
	auto iter = m_loggerMap.find(logNameId);
	if (iter != m_loggerMap.end())
	{
		iter->second->log((spdlog::level::level_enum)log_level, log.c_str());
	}
}

void NFCLogModule::SetDefaultLevel(NF_LOG_LEVEL log_level)
{
	m_defaultLogger->set_level((spdlog::level::level_enum)(log_level));
}

void NFCLogModule::SetDefaultFlush(NF_LOG_LEVEL log_level)
{
	m_defaultLogger->flush_on((spdlog::level::level_enum)(log_level));
}

/**
* @brief ����log������
*
* @param  vecLogConfig
* @return
*/
void NFCLogModule::SetDefaultLogConfig(const std::vector<LogInfoConfig>& vecLogConfig)
{
	m_logInfoConfig.clear();
	//��Ҫ��Ϊ��Ч�ʣ��˷ѵ��ڴ�
	m_logInfoConfig.resize(NF_LOG_MAX_ID);
	for (size_t i = 0; i < vecLogConfig.size(); i++)
	{
		const LogInfoConfig& config = vecLogConfig[i];
		if (config.mLogId >= 0 && config.mLogId < m_logInfoConfig.size())
		{
			m_logInfoConfig[config.mLogId] = config;
		}
	}
}

void NFCLogModule::CreateDefaultLogger()
{
	std::vector<spdlog::sink_ptr> sinks_vec;
	std::string log_name = NF_FORMAT("{}{}{}{}{}.log", m_pPluginManager->GetLogPath(), spdlog::details::os::folder_sep, m_pPluginManager->GetAppName()+lexical_cast<std::string>(m_pPluginManager->GetAppID()), spdlog::details::os::folder_sep, m_pPluginManager->GetAppName());
	auto date_and_hour_sink = std::make_shared<spdlog::sinks::my_date_and_hour_file_sink_mt>(log_name);

#if NF_PLATFORM == NF_PLATFORM_WIN
	auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
	sinks_vec.push_back(color_sink);
#else
	if (m_pPluginManager->IsDaemon() == false)
	{
		auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
		sinks_vec.push_back(color_sink);
	}
#endif

	sinks_vec.push_back(date_and_hour_sink);
#ifdef NF_DEBUG_MODE
	m_defaultLogger = std::make_shared<spdlog::logger>(m_pPluginManager->GetAppName(), std::begin(sinks_vec), std::end(sinks_vec));
#else
	m_defaultLogger = std::make_shared<spdlog::async_logger>(m_pPluginManager->GetAppName(), std::begin(sinks_vec), std::end(sinks_vec), 1024,
		spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::milliseconds(1000), nullptr);
#endif

	m_defaultLogger->set_level(spdlog::level::level_enum::trace);

	m_defaultLogger->set_pattern("%^[%l | %Y-%m-%d %H:%M:%S.%e] | %v%$");

	m_defaultLogger->flush_on(spdlog::level::debug);

	spdlog::register_logger(m_defaultLogger);
}

/*����ͬ��LOGϵͳ*/
void NFCLogModule::CreateOthersLogger(uint32_t logNameId, const std::string& logName, bool async)
{
	std::vector<spdlog::sink_ptr> sinks_vec;
	std::string log_name = NF_FORMAT("{}{}{}{}{}.log", m_pPluginManager->GetLogPath(), spdlog::details::os::folder_sep, m_pPluginManager->GetAppName() + lexical_cast<std::string>(m_pPluginManager->GetAppID()) + "_" + logName, spdlog::details::os::folder_sep, logName);
	auto date_and_hour_sink = std::make_shared<spdlog::sinks::my_date_and_hour_file_sink_mt>(log_name);

#if NF_PLATFORM == NF_PLATFORM_WIN
	auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
	sinks_vec.push_back(color_sink);
#else
	if (m_pPluginManager->IsDaemon() == false)
	{
		auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
		sinks_vec.push_back(color_sink);
	}
#endif

	sinks_vec.push_back(date_and_hour_sink);

	std::shared_ptr<spdlog::logger> pLogger;
	if (async)
	{
		pLogger = std::make_shared<spdlog::async_logger>(m_pPluginManager->GetAppName() + "_" + logName, std::begin(sinks_vec), std::end(sinks_vec), 1024,
			spdlog::async_overflow_policy::block_retry, nullptr, std::chrono::milliseconds(1000), nullptr);
	}
	else
	{
		pLogger = std::make_shared<spdlog::logger>(m_pPluginManager->GetAppName() + "_" + logName, std::begin(sinks_vec), std::end(sinks_vec));
	}

	pLogger->set_level(spdlog::level::level_enum::trace);

	pLogger->set_pattern("%^[%l | %Y-%m-%d %H:%M:%S.%e] | %v%$");

	pLogger->flush_on(spdlog::level::debug);

	spdlog::register_logger(pLogger);
	m_loggerMap.emplace(logNameId, pLogger);
}

