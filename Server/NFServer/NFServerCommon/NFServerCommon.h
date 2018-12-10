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

class NFServerCommon
{
public:
	static NFServerConfig* GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
	static NFServerConfig* GetAppConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);

	static bool MessageToJsonString(const google::protobuf::Message& message, std::string& json);
	static bool JsonStringToMessage(const string& json, google::protobuf::Message& message);
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
	////////////////////////////////////////////
	
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 11,	//网关服务器	注册	世界服务器协议
	EGMI_NET_GAME_TO_WORLD_REGISTER = 12,	//游戏服务器	注册	世界服务器协议
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//登录服务器	注册	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//世界服务器	注册	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_REGISTER = 15,	//网关服务器	注册	游戏服务器协议

	EGMI_NET_PROXY_TO_WORLD_UNREGISTER = 16,	//网关服务器 注销  世界服务器协议
	EGMI_NET_GAME_TO_WORLD_UNREGISTER = 17,	//游戏服务器	注销	世界服务器协议
	EGMI_NET_LOGIN_TO_MASTER_UNREGISTER = 18,	//登录服务器	注销	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_UNREGISTER = 19,	//世界服务器	注销	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_UNREGISTER = 20,	//网关服务器	注销	游戏服务器协议

	EGMI_NET_PROXY_TO_WORLD_REFRESH = 21,	//网关服务器 刷新  世界服务器协议
	EGMI_NET_GAME_TO_WORLD_REFRESH = 22,	//游戏服务器	刷新	世界服务器协议
	EGMI_NET_LOGIN_TO_MASTER_REFRESH = 23,	//登录服务器	刷新	MASTER服务器协议
	EGMI_NET_WORLD_TO_MASTER_REFRESH = 24,	//世界服务器	刷新	MASTER服务器协议
	EGMI_NET_PROXY_TO_GAME_REFRESH = 25,	//网关服务器	刷新	游戏服务器协议

	EGMI_NET_WORLD_TO_PROXY_SEND_GAME = 26, //世界服务器发送游戏服务器信息给网关服务器
	EGMI_NET_MASTER_TO_LOGIN_SEND_WORLD = 26, //Master服务器发送World服务器信息给Login服务器

	EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT = 50, //proxy 发送 账号链接数据
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT = 50, //proxy 发送 账号链接数据
	EGMI_NET_MSG_END = 1000,
};

//////////////////////////////////////////////////////////////////////////
class NFServerData
{
public:
	NFServerData()
	{
		mUnlinkId = 0;
	}

	~NFServerData()
	{
		mUnlinkId = 0;
	}

	uint32_t mUnlinkId;
	NFMsg::ServerInfoReport mServerInfo;
};