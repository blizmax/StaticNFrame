// -------------------------------------------------------------------------
//    @FileName         :    NFCKernelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_KERNEL_MODULE_H
#define NFC_KERNEL_MODULE_H

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include <unordered_map>
#include <NFComm/NFPluginModule/NFIClassModule.h>

class NFIObject;
class NFITimerModule;

class NFCKernelModule : public NFIKernelModule
{
public:
	explicit NFCKernelModule(NFIPluginManager* p);
	virtual ~NFCKernelModule();

	virtual bool Init() override;
	virtual bool Shut() override;
	virtual bool Finalize() override;

	virtual bool BeforeShut() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual uint64_t GetUUID() override;
	virtual std::string GetMD5(const std::string& str) override;
	virtual uint32_t GetCRC32(const std::string& s) override;
	virtual uint16_t GetCRC16(const std::string& s) override;
	virtual std::string Base64Encode(const std::string& s) override;
	virtual std::string Base64Decode(const std::string& s) override;

	virtual uint64_t Get64UUID() override;
	virtual uint64_t Get32UUID() override;

	virtual NFIObject* CreateNFObject(uint64_t guid, const std::string& className) override;
	virtual NFIObject* GetNFObject(uint64_t guid, const std::string& className = "") override;
	virtual bool ExistNFObject(uint64_t guid) override;
	virtual bool DeleteNFObject(uint64_t guid) override;
protected:
	static void ProcessMemFree();
	virtual bool DestroyNFObject(uint64_t guid);
	virtual bool DestroyAll();
	virtual void DeleteNFObjectList();
private:
	uint64_t nGUIDIndex = 0;
	uint64_t mLastGuidTimeStamp = 0;
private:
	std::unordered_map<uint64_t, NFIObject*> mObjectMap;
	std::list<uint64_t> mDeleteList;
};

#endif

