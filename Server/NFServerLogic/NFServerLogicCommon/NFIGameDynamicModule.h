// -------------------------------------------------------------------------
//    @FileName         :    NFIGameDynamicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFIGameDynamicModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIDynamicModule.h"
#include "NFIGameLogicModule.h"

class NFIGameDynamicModule : public NFIDynamicModule
{
public:
	NFIGameDynamicModule(NFIPluginManager* p) :NFIDynamicModule(p)
	{

	}

	virtual ~NFIGameDynamicModule()
	{

	}

	virtual bool Awake() final
	{
		return true;
	}
};