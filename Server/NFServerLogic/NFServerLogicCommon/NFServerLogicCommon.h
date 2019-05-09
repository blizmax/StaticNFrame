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
	EGMI_NET_WORLD_NOTIFY_GAME_CHANGE_PROXY = 101,	//通知游戏服务器,玩家的网关被改变了
	EGMI_NET_WORLD_NOTIFY_PROXY_CHANGE_GAME = 102,	//通知网关服务器,玩家的游戏服被改变了
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