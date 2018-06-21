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
class NFILogModule;
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

	virtual NFIObject* CreateObject(uint64_t objectId, const std::string& strClassName) override;
	virtual uint64_t CreateObjectId() override;
protected:
	static void ProcessMemFree();
private:
	uint64_t nGUIDIndex = 0;
	uint64_t mLastGuidTimeStamp = 0;
	std::unordered_map<uint64_t, NFIObject*> mObjectMap;
	NFIClassModule* m_pClassModule;
};

#endif

