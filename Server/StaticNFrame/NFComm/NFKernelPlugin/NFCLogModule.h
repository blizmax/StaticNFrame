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

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

#include "spdlog/sinks/base_sink.h"

#include "termcolor.hpp"

#include <mutex>
#include <string>
#include <unordered_map>
#include <wincon.h>

namespace spdlog {
	namespace sinks {
		/*
		* Windows color console sink. Uses WriteConsoleA to write to the console with colors
		*/
		template<class Mutex>
		class stdcolor_sink : public base_sink<Mutex>
		{
		public:
			stdcolor_sink()
			{
				colors_[level::trace] = termcolor::white;
				colors_[level::debug] = termcolor::cyan;
				colors_[level::info] = termcolor::green;
				colors_[level::warn] = termcolor::yellow;// | BOLD;
				colors_[level::err] = termcolor::red;                         // red bold
				colors_[level::critical] = termcolor::red;//BACKGROUND_RED | WHITE | BOLD; // white bold on red background
				colors_[level::off] = nullptr;
			}

			~stdcolor_sink() override
			{
				this->flush();
			}

			stdcolor_sink(const stdcolor_sink &other) = delete;
			stdcolor_sink &operator=(const stdcolor_sink &other) = delete;

		protected:
			void _sink_it(const details::log_msg &msg) override
			{
				std::cout << colors_[msg.level];
				std::cout << msg.formatted.c_str();
			}

			void _flush() override
			{
				// windows console always flushed?
			}

		private:
			std::unordered_map<level::level_enum, std::ostream& (*)(std::ostream& stream), level::level_hasher> colors_;
		};

		//
		// windows color console to stdout
		//
		template<class Mutex>
		class stdcolor_stdout_sink : public stdcolor_sink<Mutex>
		{
		public:
			stdcolor_stdout_sink()
				: stdcolor_sink<Mutex>()
			{
			}
		};

		using stdcolor_stdout_sink_mt = stdcolor_stdout_sink<std::mutex>;
		using stdcolor_stdout_sink_st = stdcolor_stdout_sink<details::null_mutex>;

		//
		// windows color console to stderr
		//
		template<class Mutex>
		class stdcolor_stderr_sink : public stdcolor_sink<Mutex>
		{
		public:
			stdcolor_stderr_sink()
				: stdcolor_sink<Mutex>()
			{
			}
		};

		using stdcolor_stderr_sink_mt = stdcolor_stderr_sink<std::mutex>;
		using stdcolor_stderr_sink_st = stdcolor_stderr_sink<details::null_mutex>;
	} // namespace sinks
} // namespace spdlog

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
protected:
	void CreateLogger();
private:
	std::shared_ptr<spdlog::async_logger> mxLogger;
};


