// -------------------------------------------------------------------------
//    @FileName			:    {Filename}
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Module           :   {Filename}
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"

// CRC-16/CCITT
class _NFExport NFCRC16 {
public:
	static uint16_t Sum(const void* data, size_t len);
	static uint16_t Sum(const string& s) {
		return Sum(s.data(), s.size());
	}
};
