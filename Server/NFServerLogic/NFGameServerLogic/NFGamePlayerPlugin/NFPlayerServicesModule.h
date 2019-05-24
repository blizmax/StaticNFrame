// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIPlayerServicesModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"

class NFCPlayerServicesModule : public NFIPlayerServicesModule
{
public:
	NFCPlayerServicesModule(NFIPluginManager* p);
	virtual ~NFCPlayerServicesModule();

	virtual bool Init() override;

	virtual void OnTimer(uint32_t nTimerID) override;

	virtual void SavePlayerDataToDB();
};