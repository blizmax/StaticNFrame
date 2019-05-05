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
	virtual uint64_t Get64UUID() = 0;
	virtual uint64_t Get32UUID() = 0;
	virtual std::string GetMD5(const std::string& str) = 0;
	virtual uint32_t GetCRC32(const std::string& s) = 0;
	virtual uint16_t GetCRC16(const std::string& s) = 0;
	virtual std::string Base64Encode(const std::string& s) = 0;
	virtual std::string Base64Decode(const std::string& s) = 0;

	virtual NFIObject* CreateNFObject(uint64_t guid, const std::string& className) = 0;
	virtual NFIObject* GetNFObject(uint64_t guid, const std::string& className = "") = 0;
	virtual bool ExistNFObject(uint64_t guid) = 0;
	virtual bool DeleteNFObject(uint64_t guid) = 0;
};

#endif

