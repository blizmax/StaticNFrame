// -------------------------------------------------------------------------
//    @FileName         :    NFIGameClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"

class NFIGameClient_MasterModule : public NFIModule
{
public:
	virtual ~NFIGameClient_MasterModule()
	{

	}

	virtual void SetOnlineNum(uint32_t num) = 0;
};
