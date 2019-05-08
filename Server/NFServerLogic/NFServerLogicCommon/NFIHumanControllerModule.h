// -------------------------------------------------------------------------
//    @FileName         :    NFIHumanControllerModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFIHumanControllerModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIGameDynamicModule.h"

class NFIHumanControllerModule : public NFIGameDynamicModule
{
public:
	NFIHumanControllerModule(NFIPluginManager* p) :NFIGameDynamicModule(p)
	{

	}

	virtual ~NFIHumanControllerModule()
	{

	}
};
