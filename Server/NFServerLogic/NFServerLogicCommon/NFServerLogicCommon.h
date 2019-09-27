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
	EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_LOGIN = 101, //֪ͨ����������� ��ҵ�¼
	EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_LOGIN = 102, //֪ͨ��Ϸ�������� ��ҵ�¼
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_LOGIN = 103, //֪ͨLogin�������� ��ҵ�¼

	EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_DISCONNECT = 111, //֪ͨ����������� ��ҵ�����
	EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_DISCONNECT = 112, //֪ͨ��Ϸ�������� ��ҵ�����
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_DISCONNECT = 113, //֪ͨLogin�������� ��ҵ�����
	EGMI_NET_PROXY_NOTIFY_LOGIN_HANDLE_PLAYER_DISCONNECT = 114, //֪ͨLogin�������� ��ҵ�����

	EGMI_NET_PROXY_NOTIFY_GAME_PLAYER_RECONNECT = 121, //֪ͨ��Ϸ�������� ���������
	EGMI_NET_PROXY_NOTIFY_WORLD_PLAYER_RECONNECT = 122, //֪ͨ����������� ���������
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_RECONNECT = 123, //֪ͨLogin�������� ���������
	
	EGMI_NET_PROXY_NOTIFY_LOGIN_PLAYER_REPORT = 109, //֪ͨ��Ϸ�������� ��ҵĵ�ǰIP
	EGMI_NET_GAME_SEND_PACKET_TO_PROXY = 110, //��Ϸ�������ϰ�������
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