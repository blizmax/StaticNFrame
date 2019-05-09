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
	NF_LOG_WORLD_RECV_MSG_LOG = 1003,		//WORLD������ϢLOG
	NF_LOG_WORLD_SEND_MSG_LOG = 1004,		//WORLD������ϢLOG
	NF_LOG_GAME_RECV_MSG_LOG = 1005,		//GAME������ϢLOG
	NF_LOG_GAME_SEND_MSG_LOG = 1006,		//GAME������ϢLOG
	NF_LOG_LOGIN_MODULE_LOG = 1102,			//��½���LOG��ӡ

	NF_SERVER_LOGIC_LOG_MAX_ID = 10240, //���LOGID
};

//������֮����ڲ�Э���100-1000, ��������������������֮����߼�
enum NF_EGMI_SERVER_TO_SERVER_MsgId
{
	EGMI_NET_SERVER_TO_SERVERMSG_BEGIN = 100,
	EGMI_NET_WORLD_NOTIFY_GAME_CHANGE_PROXY = 101,	//֪ͨ��Ϸ������,��ҵ����ر��ı���
	EGMI_NET_WORLD_NOTIFY_PROXY_CHANGE_GAME = 102,	//֪ͨ���ط�����,��ҵ���Ϸ�����ı���
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