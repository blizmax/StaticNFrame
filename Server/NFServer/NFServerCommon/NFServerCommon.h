// -------------------------------------------------------------------------
//    @FileName         :    NFServerCommon.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFConfigMgr.h"

class NFServerCommon
{
public:
	static NFServerConfig* GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
};