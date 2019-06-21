// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <string>

#include "NFComm/NFPluginModule/NFIModule.h"
#include "libgo/libgo/coroutine.h"
#include "libgo/libgo/libgo.h"

class NFCTestModule : public NFIModule
{
public:
	explicit NFCTestModule(NFIPluginManager* p);
	virtual ~NFCTestModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void test(bool result);
};
