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
#include "NFMessageDefine/NFMsgDefine.h"

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

//服务器之间的内部协议号1-1000, 不涉及业务
enum NF_EGMI_MsgId
{
	EGMI_NET_MSG_BEGIN = 0,

	//0号协议是特殊的JSON协议,数据是JSON字符串
	EGMI_NET_MSG_JSON_MSG = 0,
	//1号协议是服务器内存心跳协议
	EGMI_NET_MSG_HEART_KEEP_ALIVE = 1,

	////////////////////////////////////////////
	
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 11,	//网关服务器注册世界服务器协议
	EGMI_NET_GAME_TO_WORLD_REGISTER = 12,	//游戏服务器注册世界服务器协议
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//登录服务器注册MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//世界服务器注册MASTER服务器协议

	EGMI_NET_PROXY_TO_GAME_REGISTER = 14,	//网关服务器注册游戏服务器协议

	EGMI_NET_MSG_END = 1000,
};

//////////////////////////////////////////////////////////////////////////
class NFServerData
{
	NFServerData()
	{
		mUnlinkId = 0;
	}

	~NFServerData()
	{
		mUnlinkId = 0;
	}

	uint32_t mUnlinkId;
	NFMsg::ServerInfoReport pData;
};