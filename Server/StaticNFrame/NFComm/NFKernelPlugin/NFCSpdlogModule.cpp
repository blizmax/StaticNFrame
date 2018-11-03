// -------------------------------------------------------------------------
//    @FileName         :    NFCSpdlogModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018-11-3
//    @Module           :    NFCSpdlogModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "spdlog/contrib/sinks/date_and_hour_file_sink.h"
#include "NFCSpdlogModule.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"

#include <iostream>


bool NFCSpdlogModule::Init()
{
	//Create spdlog
	try
	{
		CreateLogger();
	}
	catch (std::system_error& error)
	{
		std::cout << "Create logger failed, error = " << error.what() << std::endl;
		NF_ASSERT_RET_VAL(0, false);
	}

	return true;
}

bool NFCSpdlogModule::Shut()
{
	spdlog::drop_all();
	return true;
}

void NFCSpdlogModule::LogNormal(NF_LOG_LEVEL log_level, const std::string& log)
{
	mxLogger->log((spdlog::level::level_enum)log_level, log.c_str());
}

void NFCSpdlogModule::CreateLogger()
{
	std::vector<spdlog::sink_ptr> sinks_vec;
	std::string log_name = NF_FORMAT("{}{}{}.log", "spdlog", NF_FOLDER_SEP, pPluginManager->GetAppName());
	auto date_and_hour_sink = std::make_shared<spdlog::sinks::date_and_hour_file_sink_mt>(log_name);
#if NF_DEBUG_MODE
#if NF_PLATFORM == NF_PLATFORM_WIN
	auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
	auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

	sinks_vec.push_back(color_sink);
#endif
	sinks_vec.push_back(date_and_hour_sink);

	mxLogger = std::make_shared<spdlog::async_logger>(pPluginManager->GetAppName(), std::begin(sinks_vec), std::end(sinks_vec), 1024);

#if NF_DEBUG_MODE
	mxLogger->set_level((spdlog::level::level_enum)(NFConfigMgr::Instance()->GetLogLevel()));
	mxLogger->set_pattern("%^[%l | %Y-%m-%d %H:%M:%S.%e] | %v%$");
#else
	mxLogger->set_pattern("[%l | %Y-%m-%d %H:%M:%S.%e] | %v");
#endif

	mxLogger->flush_on(spdlog::level::err);

	spdlog::register_logger(mxLogger);
}

