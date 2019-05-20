// -------------------------------------------------------------------------
//    @FileName         :    NFHumanDefine.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/7
//    @Email			:    445267987@qq.com
//    @Module           :    NFHumanDefine
//
// -------------------------------------------------------------------------

#pragma once

#include "NFMessageDefine/NFMsgDefine.h"
#include "NFServerLogicCommon.h"
#include "NFMessageDefine/st_human.pb.h"
#include "NFMessageDefine/st_human_packet_code.pb.h"
#include "NFMessageDefine/msg_human.pb.h"
#include "NFMessageDefine/db_human.pb.h"

enum HumanReturnCode
{
	RETURN_CODE_ACCOUNT_NO_EXIST = 1001, //账号不存在
	RETURN_CODE_PASSWORD_NOT_MATCH = 1002,
	RETURN_CODE_CREATE_PLAYER_FAIL = 1003, //创建玩家失败
	RETURN_CODE_AWARD_ITEMBASE_NOT = 1004, //登录奖励的物品不存在
	RETURN_CODE_AWARD_REDIS_NOT = 1005, //缓存种不存在用户奖品信息
	RETURN_CODE_LOGIN_AWARD_FINISH = 1006, //已经领取了
	RETURN_CODE_LOGIN_AWARD_EXPIRED = 1007, //签到已经过期了

	RETURN_CODE_JETTON_NOT_ENOUGH = 1008, //进入牌桌的时候，筹码不够
	RETURN_CODE_EXCEED = 1009, //进入牌桌时，筹码超出了牌桌的上限
	RETURN_CODE_ENTER_TABLE_FAILED = 1010, //进入牌桌失败
	RETURN_CODE_RACE_NOT_EXIST = 1011, //
	RETURN_CODE_TABLE_NOT_EXIST = 1012,
	RETURN_CODE_PLAYER_NOT_EXIST = 1013, //缓存中找不到对应的玩家的信息
	RETURN_CODE_TABLEPLAYER_NOT_EXIST = 1014, //当前牌桌没有玩家的信息
	RETURN_CODE_SITDOWN = 1015, //桌子已经满人了

	RETURN_CODE_MONEY_NOT_ENOUGH = 1022, //钱不够

	RETURN_CODE_ACCOUNT_LOGIN_ERROR = 1068, //账号登陆失败
};
