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

class NFILogModule;
class NFITimerModule;

class NFCKernelModule : public NFIKernelModule
{
public:
	explicit NFCKernelModule(NFIPluginManager* p);
	virtual ~NFCKernelModule();

	virtual bool Init() override;
	virtual bool Shut() override;

	virtual bool BeforeShut() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;
protected:
	static void ProcessMemFree();
};

#endif

