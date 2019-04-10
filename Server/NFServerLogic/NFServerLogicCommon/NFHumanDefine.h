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
	RETURN_CODE_ACCOUNT_NO_EXIST = 1001, //�˺Ų�����
	RETURN_CODE_PASSWORD_NOT_MATCH = 1002,
	RETURN_CODE_CREATE_PLAYER_FAIL = 1003, //�������ʧ��
	RETURN_CODE_AWARD_ITEMBASE_NOT = 1004, //��¼��������Ʒ������
	RETURN_CODE_AWARD_REDIS_NOT = 1005, //�����ֲ������û���Ʒ��Ϣ
	RETURN_CODE_LOGIN_AWARD_FINISH = 1006, //�Ѿ���ȡ��
	RETURN_CODE_LOGIN_AWARD_EXPIRED = 1007, //ǩ���Ѿ�������
};
