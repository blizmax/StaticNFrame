// -------------------------------------------------------------------------
//    @FileName         :    NFLogMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFILogModule.h"
#include "NFComm/NFCore/NFSingleton.hpp"

class NFLogMgr : public NFSingleton<NFLogMgr>
{
public:
	NFLogMgr();
	virtual ~NFLogMgr();
public:
	bool Init(NFILogModule* p);
	void UnInit();
public:
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const int64_t nDesc, const char* func = "", int line = 0);
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func = "", int line = 0);
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::ostringstream& stream, const char* func = "", int line = 0);

	virtual NFILogModule* GetLogModule();
protected:
	NFILogModule* m_pLogModule;
};

#define NFLogNormalDebug(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define NFLogNormalInfo(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_INFO_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define NFLogNormalWarning(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_WARING_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define NFLogNormalError(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_ERROR_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define NFLogNormalFatal(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_FATAL_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);

#define NFLogDebug(format, ...) NFLogMgr::Instance()->GetLogModule()->Log(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, format, ##__VA_ARGS__);
#define NFLogInfo(format, ...) NFLogMgr::Instance()->GetLogModule()->Log(NF_LOG_LEVEL::NLL_INFO_NORMAL, format, ##__VA_ARGS__);
#define NFLogWarning(format, ...) NFLogMgr::Instance()->GetLogModule()->Log(NF_LOG_LEVEL::NLL_WARING_NORMAL, format, ##__VA_ARGS__);
#define NFLogError(format, ...) NFLogMgr::Instance()->GetLogModule()->Log(NF_LOG_LEVEL::NLL_ERROR_NORMAL, format, ##__VA_ARGS__);
#define NFLogFatal(format, ...) NFLogMgr::Instance()->GetLogModule()->Log(NF_LOG_LEVEL::NLL_FATAL_NORMAL, format, ##__VA_ARGS__);

