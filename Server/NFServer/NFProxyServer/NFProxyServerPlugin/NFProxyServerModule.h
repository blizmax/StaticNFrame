// -------------------------------------------------------------------------
//    @FileName         :    NFProxyServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include <NFServer/NFServerCommon/NFIProxyServerModule.h>

class NFCProxyServerModule : public NFIProxyServerModule
{
public:
	explicit NFCProxyServerModule(NFIPluginManager* p);
	virtual ~NFCProxyServerModule();


};
