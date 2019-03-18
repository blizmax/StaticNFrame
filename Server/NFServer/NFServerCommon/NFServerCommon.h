// -------------------------------------------------------------------------
//    @FileName         :    NFServerCommon.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include "NFMessageDefine/NFMsgDefine.h"

class _NFExport NFServerCommon
{
public:
	static NFServerConfig* GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
	static NFServerConfig* GetAppConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
};

class NFProxyData
{
public:
	NFProxyData()
	{
		mUnlinkId = 0;
	}

	uint32_t mUnlinkId;
	NFMsg::ServerInfoReport mServerInfo;
};

//服务器之间的内部协议号1-1000, 不涉及业务
enum NF_EGMI_MsgId
{
	EGMI_NET_MSG_BEGIN = 0,

	//0号协议是特殊的JSON协议,数据是JSON字符串
	EGMI_NET_MSG_JSON_MSG = 0,
	//1号协议是服务器内存心跳协议
	EGMI_STS_HEART_BEAT = 1,

	EGMI_STS_SERVER_REPORT = 2,

	//3 Master收到GM协议数据，转发给网关
	EGMI_STS_GM_MSG = 3,
	////////////////////////////////////////////
	
	EGMI_NET_PROXY_TO_MASTER_REGISTER = 11,	//网关服务器	注册	MASTER服务器协议
	EGMI_NET_GAME_TO_MASTER_REGISTER = 12,	//网关服务器	注册	MASTER服务器协议
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//登录服务器	注册	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//世界服务器	注册	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_REGISTER = 15,	//网关服务器	注册	游戏服务器协议
	EGMI_NET_GAME_TO_WORLD_REGISTER = 16,	//游戏服务器	注册	世界服务器协议
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 17,	//网关服务器	注册	WORLD服务器协议

	EGMI_NET_PROXY_TO_MASTER_UNREGISTER = 21,	//网关服务器 注销  MASTER服务器协议
	EGMI_NET_GAME_TO_MASTER_UNREGISTER = 22,	//网关服务器 注销  MASTER服务器协议
	EGMI_NET_LOGIN_TO_MASTER_UNREGISTER = 23,	//登录服务器	注销	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_UNREGISTER = 24,	//世界服务器	注销	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_UNREGISTER = 25,		//网关服务器	注销	游戏服务器协议
	EGMI_NET_GAME_TO_WORLD_UNREGISTER = 26,		//游戏服务器	注销	世界服务器协议
	EGMI_NET_PROXY_TO_WORLD_UNREGISTER = 27,	//网关服务器	注销	WORLD服务器协议

	EGMI_NET_PROXY_TO_MASTER_REFRESH = 31,	//网关服务器 刷新		MASTER服务器协议
	EGMI_NET_GAME_TO_MASTER_REFRESH = 32,	//游戏服务器	刷新	MASTER服务器协议
	EGMI_NET_LOGIN_TO_MASTER_REFRESH = 33,	//登录服务器	刷新	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_REFRESH = 34,	//世界服务器	刷新	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_REFRESH = 35,	//网关服务器	刷新	游戏服务器协议
	EGMI_NET_GAME_TO_WORLD_REFRESH = 36,	//游戏服务器	刷新	世界服务器协议
	EGMI_NET_PROXY_TO_WORLD_REFRESH = 37,	//网关服务器	刷新	WORLD服务器协议

	EGMI_NET_MASTER_SEND_SERVER_TO_SERVER = 40, //master send server to server

	EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT = 50, //proxy 发送 账号链接数据
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT = 51, //proxy 发送 账号链接数据
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT = 52, //proxy 发送 账号链接数据

	EGMI_NET_LOGIN_TO_MASTER_PLAT_LOGIN = 60, //plat login
	EGMI_NET_MASTER_TO_LOGIN_PLAT_LOGIN = 61, //plat login
	EGMI_NET_MSG_END = 1000,
};

enum NFServerEvent
{
	NF_SERVER_EVENT_GAME_DISCONNECT_PROXY = 100,
	NF_SERVER_EVENT_WORLD_DISCONNECT_PROXY = 101,
};