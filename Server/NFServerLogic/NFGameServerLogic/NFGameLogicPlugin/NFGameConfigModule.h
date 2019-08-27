// -------------------------------------------------------------------------
//    @FileName         :    NFGameConfigModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameConfigModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIGameConfigModule.h"


class NFCGameConfigModule : public NFIGameConfigModule
{
public:
	NFCGameConfigModule(NFIPluginManager* p);
	virtual ~NFCGameConfigModule();

	virtual bool Awake();

	virtual bool Finalize();

	virtual bool OnReloadPlugin();
};
