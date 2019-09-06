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
#include "NFComm/NFPluginModule/NFLogMgr.h"


enum NF_SERVER_LOG_ID
{
	NF_LOG_MIN_SERVER_LOG = 101,			//101-1000是系统框架层LOG
	NF_LOG_SERVER_CONNECT_SERVER = 101,		//服务器与服务器连接LOG
	NF_LOG_SERVER_NOT_HANDLE_MESSAGE = 102, //服务器未处理消息的LOG
	NF_LOG_BEHAVIOR_LOGIC_LOG = 103,		//行为LOG日志
	NF_LOG_MAX_SERVER_LOG = 1000,			//
};

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

	//3 Master收到GM协议数据，转发给别的服务器
	EGMI_STS_GM_MSG = 3,
	EGMI_STS_HTTP_MSG = 4,
	EGMI_STS_HTTP_MSG_RET = 5,

	EGMI_STS_ERROR_MSG = 6,
	////////////////////////////////////////////
	
	EGMI_NET_PROXY_TO_MASTER_REGISTER = 11,	//网关服务器	注册	MASTER服务器协议
	EGMI_NET_GAME_TO_MASTER_REGISTER = 12,	//网关服务器	注册	MASTER服务器协议
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//登录服务器	注册	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//世界服务器	注册	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_REGISTER = 15,	//网关服务器	注册	游戏服务器协议
	EGMI_NET_GAME_TO_WORLD_REGISTER = 16,	//游戏服务器	注册	世界服务器协议
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 17,	//网关服务器	注册	WORLD服务器协议
	EGMI_NET_PROXY_TO_LOGIN_REGISTER = 18,	//网关服务器	注册	Login服务器协议

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

	EGMI_NET_MASTER_SEND_OTHERS_TO_GAME = 40, //master send server to game
	EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD = 41, //master send server to world
	EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY = 42, //master send server to proxy
	EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN = 43, //master send server to server

	EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT = 50, //proxy 发送 账号链接数据
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT = 51, //proxy 发送 账号链接数据
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT = 52, //proxy 发送 账号链接数据

	EGMI_NET_MSG_END = 1000,
};

enum NFServerEvent
{
	NF_SERVER_EVENT_GAME_DISCONNECT_PROXY = 100,
	NF_SERVER_EVENT_WORLD_DISCONNECT_PROXY = 101,
};