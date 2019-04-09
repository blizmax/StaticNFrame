// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIDynamicModule.h"

class NFHumanServicesModule : public NFIDynamicModule
{
public:
	NFHumanServicesModule(NFIPluginManager* p);
	virtual ~NFHumanServicesModule();
};