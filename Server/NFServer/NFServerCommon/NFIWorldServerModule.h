// -------------------------------------------------------------------------
//    @FileName         :    NFIGameServerModule.h
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

class NFIWorldServerModule : public NFIModule
{
public:
	virtual ~NFIWorldServerModule()
	{

	}

	virtual NF_SHARE_PTR<NFServerData> GetGameByLink(uint32_t unLinkId) = 0;
};

