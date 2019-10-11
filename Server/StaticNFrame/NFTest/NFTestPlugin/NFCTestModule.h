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
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"

#include "NFEventLoopThread.h"

class NFCTestModule : public NFIModule, NFTimerObj
{
public:
	explicit NFCTestModule(NFIPluginManager* p);
	virtual ~NFCTestModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnTimer(uint32_t nTimerID) override;

	NFEventLoopThread loopThread;
};
