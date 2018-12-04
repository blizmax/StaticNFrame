// -------------------------------------------------------------------------
//    @FileName         :    NFIProxyClient_GameModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFIProxyClient_GameModule : public NFIModule
{
public:	
	virtual ~NFIProxyClient_GameModule()
	{
		
	}

	virtual NF_SHARE_PTR<NFServerData> GetServerData(uint32_t serverId) = 0;
};