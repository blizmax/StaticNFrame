// -------------------------------------------------------------------------
//    @FileName         :    NFCLogModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFILogModule.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

class NFCLogModule : public NFILogModule
{
public:
	NFCLogModule(NFIPluginManager* p);

	virtual ~NFCLogModule();

	virtual bool Init() override;
	virtual bool Shut() override;

	virtual void LogNormal(NF_LOG_LEVEL log_level, const std::string& log);
protected:
	void CreateLogger();
private:
	std::shared_ptr<spdlog::async_logger> mxLogger;
};


