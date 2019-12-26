// -------------------------------------------------------------------------
//    @FileName         :    NFICommonClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include <NFComm/NFPluginModule/NFServerDefine.h>

class NFICommonClient_MasterModule : public NFIModule
{
public:
	virtual ~NFICommonClient_MasterModule()
	{

	}

	virtual void SetServerType(NF_SERVER_TYPES serverType) = 0;
	virtual NF_SHARE_PTR<NFServerData> GetMasterServerData() = 0;
};

