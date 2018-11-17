// -------------------------------------------------------------------------
//    @FileName         :    NFIKernelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIKernelModule
//
// -------------------------------------------------------------------------

#ifndef NFI_KERNEL_MODULE_H
#define NFI_KERNEL_MODULE_H

#include <iostream>
#include <string>
#include <functional>
#include "NFIModule.h"

class NFIObject;

class NFIKernelModule
	: public NFIModule
{
public:
	virtual uint64_t GetUUID() = 0;
	virtual std::string GetMD5(const std::string& str) = 0;
	virtual uint32_t GetCRC32(const std::string& s) = 0;
	virtual uint16_t GetCRC16(const std::string& s) = 0;
	virtual std::string Base64Encode(const std::string& s) = 0;
	virtual std::string Base64Decode(const std::string& s) = 0;
};

#endif

