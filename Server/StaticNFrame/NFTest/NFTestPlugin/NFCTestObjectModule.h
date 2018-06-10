// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFDBActorMgr.h"

class NFCTestObjectModule : public NFIModule
{
public:
	explicit NFCTestObjectModule(NFIPluginManager* p);
	virtual ~NFCTestObjectModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
};
