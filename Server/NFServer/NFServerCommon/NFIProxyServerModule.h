// -------------------------------------------------------------------------
//    @FileName         :    NFIProxyServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFIProxyServerModule : public NFIModule
{
public:	
	virtual ~NFIProxyServerModule()
	{
		
	}
	virtual NFServerData* GetGameServerData(uint32_t serverId) = 0;
	virtual NFServerData* GetWorldServerData(uint32_t serverId) = 0;
};
