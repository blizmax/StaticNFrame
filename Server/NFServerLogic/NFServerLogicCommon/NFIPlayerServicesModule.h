// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFIHumanServicesModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIGameDynamicModule.h"

class NFIPlayerServicesModule : public NFIGameDynamicModule
{
public:
	NFIPlayerServicesModule(NFIPluginManager* p) :NFIGameDynamicModule(p)
	{

	}

	virtual ~NFIPlayerServicesModule()
	{

	}
};