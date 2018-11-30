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

class NFProxyData
{
public:
	NFProxyData()
	{
		mServerId = 0;
		mLinkId = 0;
	}

	uint32_t mServerId;
	uint32_t mLinkId;

	virtual void print() { }
};

enum NFMsgIdSection
{
	emsgid_net_begin = 1,
	emsgid_net_end = 100,

	emsgid_login_begin = 100,
	emsgid_login_end = 200,

	emsgid_center_begin = 200,
	emsgid_center_end = 300,

    emsg_id_Num,	
};