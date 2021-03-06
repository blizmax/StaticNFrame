// -------------------------------------------------------------------------
//    @FileName         :    NFIGameClient_WorldModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFMessageDefine/NFMsgDefine.h"

class NFIGameClient_WorldModule : public NFIModule
{
public:
	virtual ~NFIGameClient_WorldModule()
	{

	}

	virtual void OnHandleWorldReport(const NFMsg::ServerInfoReport& xData) = 0;
};

