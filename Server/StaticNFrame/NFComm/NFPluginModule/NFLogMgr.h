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
protected:
	NFILogModule* m_pLogModule;
};

#define LogDebug(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_DEBUG_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define LogInfo(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_INFO_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define LogWarning(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_WARING_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define LogError(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_ERROR_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);
#define LogFatal(ident, strInfo, nDesc) NFLogMgr::Instance()->LogNormal(NF_LOG_LEVEL::NLL_FATAL_NORMAL, ident, strInfo, nDesc, __FUNCTION__, __LINE__);

