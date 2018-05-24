// -------------------------------------------------------------------------
//    @FileName         :    NFException.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-22
//    @Module           :    NFException
//
// -------------------------------------------------------------------------

#ifndef NF_EXCEPTION_HPP
#define NF_EXCEPTION_HPP

#include <string>
#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include "NFStringUtility.h"

class NFException
{
public:
	NFException(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		NFStringUtility::StringAppendV(&message, format, ap);
		va_end(ap);
	}

	std::string GetMessage() const
	{
		return message;
	}
protected:
	std::string message;
};

#endif