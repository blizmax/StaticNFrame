// -------------------------------------------------------------------------
//    @FileName         :    NFCRC16.h
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"

// CRC-16/CCITT
class NFCRC16
{
public:
	static uint16_t Sum(const void* data, size_t len);

	static uint16_t Sum(const std::string& s)
	{
		return Sum(s.data(), s.size());
	}
};

