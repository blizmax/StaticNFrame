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

	RETURN_CODE_JETTON_NOT_ENOUGH = 1008, //����������ʱ�򣬳��벻��
	RETURN_CODE_EXCEED = 1009, //��������ʱ�����볬��������������
	RETURN_CODE_ENTER_TABLE_FAILED = 1010, //��������ʧ��
	RETURN_CODE_RACE_NOT_EXIST = 1011, //
	RETURN_CODE_TABLE_NOT_EXIST = 1012,
	RETURN_CODE_PLAYER_NOT_EXIST = 1013, //�������Ҳ�����Ӧ����ҵ���Ϣ
	RETURN_CODE_TABLEPLAYER_NOT_EXIST = 1014, //��ǰ����û����ҵ���Ϣ
	RETURN_CODE_SITDOWN = 1015, //�����Ѿ�������

	RETURN_CODE_MONEY_NOT_ENOUGH = 1022, //Ǯ����

	RETURN_CODE_ACCOUNT_LOGIN_ERROR = 1068, //�˺ŵ�½ʧ��
};
