// -------------------------------------------------------------------------
//    @FileName         :    NFILoginClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFILoginClient_MasterModule : public NFIModule
{
public:
	virtual ~NFILoginClient_MasterModule()
	{

	}

	virtual uint32_t GetMasterLinkId() const = 0;

};
