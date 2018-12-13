// -------------------------------------------------------------------------
//    @FileName         :    NFIGameClient_ProxyModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFMessageDefine/NFMsgDefine.h"

class NFIGameClient_ProxyModule : public NFIModule
{
public:
	virtual ~NFIGameClient_ProxyModule()
	{

	}

	virtual void OnHandleProxyReport(const NFMsg::ServerInfoReport& xData) = 0;
};


