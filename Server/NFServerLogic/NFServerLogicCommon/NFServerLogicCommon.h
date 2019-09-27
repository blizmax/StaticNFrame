// -------------------------------------------------------------------------
//    @FileName         :    NFServerLogicCommon.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerLogicCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

enum NF_GAME_DB_ID
{
	NF_GAME_BEHAVIOR_LOG_DB = 1,
};

enum NF_SERVER_LOGIC_LOG_ID
{
	NF_LOG_BEGIN_SERVER_LOGIC_ID = 1001,			//1001以后是逻辑框架层
	NF_LOG_PROXY_RECV_MSG_LOG = 1001,		//网关接受消息LOG
	NF_LOG_PROXY_SEND_MSG_LOG = 1002,		//网关发送消息LOG
	NF_LOG_WORLD_RECV_MSG_LOG = 1003,		//WORLD接受消息LOG
	NF_LOG_WORLD_SEND_MSG_LOG = 1004,		//WORLD发送消息LOG
	NF_LOG_GAME_RECV_MSG_LOG = 1005,		//GAME接受消息LOG
	NF_LOG_GAME_SEND_MSG_LOG = 1006,		//GAME发送消息LOG
	NF_LOG_LOGIN_MODULE_LOG = 1102,			//登陆相关LOG打印

	NF_SERVER_LOGIC_LOG_MAX_ID = 10240, //最大LOGID
};

//服务器之间的内部协议号100-1000, 用来处理服务器与服务器之间的逻辑
enum NF_EGMI_SERVER_TO_SERVER_MsgId
{
	EGMI_NET_SERVER_TO_SERVERMSG_BEGIN = 100,
	EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN = 101, //通知世界服务器， 玩家登录
	EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_LOGIN = 102, //通知游戏服务器， 玩家登录
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_LOGIN = 103, //通知Login服务器， 玩家登录

	EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_DISCONNECT = 111, //通知世界服务器， 玩家掉线了
	EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_DISCONNECT = 112, //通知游戏服务器， 玩家掉线了
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_DISCONNECT = 113, //通知Login服务器， 玩家掉线了
	EGMI_NET_PROXY_NOTIFY_LOGIN_HANDLE_PLAYER_DISCONNECT = 114, //通知Login服务器， 玩家掉线了

	EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_RECONNECT = 121, //通知游戏服务器， 玩家重连接
	EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_RECONNECT = 122, //通知世界服务器， 玩家重连接
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_RECONNECT = 123, //通知Login服务器， 玩家重连接
	
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_REPORT = 109, //通知游戏服务器， 玩家的当前IP
	EGMI_NET_GAME_SEND_PACKET_TO_PROXY = 110, //游戏服务器合包给网关
	EGMI_NET_SERVER_TO_SERVER_MSG_END = 1000,
};

enum NF_MYSQL_EVENT_TYPE
{
	NF_MYSQL_LOGIN_EVENT = 1,
};

enum ReturnCode
{
	RETURN_CODE_SUCCESS = 0,
};


class _NFExport NFServerLogicCommon
{
public:
	NFServerLogicCommon();
};