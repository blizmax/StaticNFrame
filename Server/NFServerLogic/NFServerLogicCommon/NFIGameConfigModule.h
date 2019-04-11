// -------------------------------------------------------------------------
//    @FileName         :    NFIGameConfigModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFIGameConfigModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIObject.h"

#pragma once

class NFIGameConfigModule : public NFIModule
{
public:
	virtual NFIObject* GetGlobalConfig() const = 0;
};
