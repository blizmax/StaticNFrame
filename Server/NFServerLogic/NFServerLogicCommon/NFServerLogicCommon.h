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
	NF_LOG_BEGIN_SERVER_LOGIC_ID = 1001,			//1001�Ժ����߼���ܲ�
	NF_LOG_PROXY_RECV_MSG_LOG = 1001,		//���ؽ�����ϢLOG
	NF_LOG_PROXY_SEND_MSG_LOG = 1002,		//���ط�����ϢLOG
	NF_LOG_LOGIN_MODULE_LOG = 1102,			//��½���LOG��ӡ

	NF_SERVER_LOGIC_LOG_MAX_ID = 10240, //���LOGID
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