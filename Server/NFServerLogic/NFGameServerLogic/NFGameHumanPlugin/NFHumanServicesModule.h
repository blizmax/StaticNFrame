// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIHumanServicesModule.h"

class NFCHumanServicesModule : public NFIHumanServicesModule
{
public:
	NFCHumanServicesModule(NFIPluginManager* p);
	virtual ~NFCHumanServicesModule();

	/*
	** 这个函数主要用来保存引擎指针, 动态加载的时候引擎指针可能会失效
	*/
	virtual bool DynamicLoadPlugin() override;
};