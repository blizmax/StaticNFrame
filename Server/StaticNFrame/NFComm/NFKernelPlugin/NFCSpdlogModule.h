// -------------------------------------------------------------------------
//    @FileName         :    NFCSpdlogModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFCSpdlogModule
//    @Desc             :
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFISpdlogModule.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

class NFCSpdlogModule : public NFISpdlogModule
{
public:
	NFCSpdlogModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init() override;
	virtual bool Shut() override;

	virtual void LogNormal(NF_LOG_LEVEL log_level, const std::string& log);
protected:
	void CreateLogger();
private:
	std::shared_ptr<spdlog::async_logger> mxLogger;
};


