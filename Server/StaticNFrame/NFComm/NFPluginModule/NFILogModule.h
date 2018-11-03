// -------------------------------------------------------------------------
//    @FileName         :    NFILogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOG_MODULE_H
#define NFI_LOG_MODULE_H

#include "NFIModule.h"

enum NF_LOG_LEVEL
{
	NLL_TRACE_NORMAL = 0,
	NLL_DEBUG_NORMAL = 1,
	NLL_INFO_NORMAL = 2,
	NLL_WARING_NORMAL = 3,
	NLL_ERROR_NORMAL = 4,
	NLL_FATAL_NORMAL,
};

class NFILogModule
	: public NFIModule
{
public:
	virtual bool Log(const NF_LOG_LEVEL nll, const char* format, ...) = 0;
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const int64_t nDesc, const char* func = "", int line = 0) = 0;
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func = "", int line = 0) = 0;
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
	virtual void LuaDebug(const std::string& strInfo) = 0;
	virtual void LuaInfo(const std::string& strInfo) = 0;
	virtual void LuaWarn(const std::string& strInfo) = 0;
	virtual void LuaError(const std::string& strInfo) = 0;
};

#endif

