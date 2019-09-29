// -------------------------------------------------------------------------
//    @FileName         :    NFEventDefine.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFEventObj.h"

enum NFEventDefine
{
	NFEVENT_TEST = 0, //�����¼��� ��������
	NFEVENT_GM = 1, //GM�����¼�
	NFEVENT_LUA_ERROR_LOG = 2, //LUAϵͳ����
	NFEVENT_LUA_FINISH_LOAD = 2, //LUAϵͳ��ɼ���

	NFEVENT_MYSQL_UPDATE_MESSAGE = 10, //mysql ���ݿ� update ����
};

