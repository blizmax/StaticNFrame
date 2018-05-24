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
	NLL_DEBUG_NORMAL,
	NLL_INFO_NORMAL,
	NLL_WARING_NORMAL,
	NLL_ERROR_NORMAL,
	NLL_FATAL_NORMAL,
};

class NFILogModule
	: public NFIModule
{
public:

	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const int64_t nDesc, const char* func = "", int line = 0) = 0;
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func = "", int line = 0) = 0;
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::ostringstream& stream, const char* func = "", int line = 0) = 0;
};

#endif

