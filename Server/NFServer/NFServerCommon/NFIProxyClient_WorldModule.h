// -------------------------------------------------------------------------
//    @FileName         :    NFIProxyClient_WorldModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/3/19
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFMessageDefine/NFMsgDefine.h"

class NFIProxyClient_WorldModule : public NFIModule
{
public:
	virtual ~NFIProxyClient_WorldModule()
	{

	}

	virtual void OnHandleServerReport(const NFMsg::ServerInfoReport& xData) = 0;
};



