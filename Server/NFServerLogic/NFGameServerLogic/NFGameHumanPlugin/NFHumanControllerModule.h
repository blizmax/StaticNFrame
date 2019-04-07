// -------------------------------------------------------------------------
//    @FileName         :    NFHumanControllerModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"

class NFHumanControllerModule : public NFIModule
{
public:
	NFHumanControllerModule(NFIPluginManager* p);

	virtual ~NFHumanControllerModule();

	virtual bool Init();
public:

};
