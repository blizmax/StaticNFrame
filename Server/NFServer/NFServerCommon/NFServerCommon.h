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
#include "NFComm/NFPluginModule/NFLogMgr.h"


enum NF_SERVER_LOG_ID
{
	NF_LOG_MIN_SERVER_LOG = 101,			//101-1000��ϵͳ��ܲ�LOG
	NF_LOG_SERVER_CONNECT_SERVER = 101,		//�����������������LOG
	NF_LOG_SERVER_NOT_HANDLE_MESSAGE = 102, //������δ������Ϣ��LOG
	NF_LOG_BEHAVIOR_LOGIC_LOG = 103,		//��ΪLOG��־
	NF_LOG_MAX_SERVER_LOG = 1000,			//
};

class _NFExport NFServerCommon
{
public:
	static NFServerConfig* GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
	static NFServerConfig* GetAppConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
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

//������֮����ڲ�Э���1-1000, ���漰ҵ��
enum NF_EGMI_MsgId
{
	EGMI_NET_MSG_BEGIN = 0,

	//0��Э���������JSONЭ��,������JSON�ַ���
	EGMI_NET_MSG_JSON_MSG = 0,
	//1��Э���Ƿ������ڴ�����Э��
	EGMI_STS_HEART_BEAT = 1,

	EGMI_STS_SERVER_REPORT = 2,

	//3 Master�յ�GMЭ�����ݣ�ת������ķ�����
	EGMI_STS_GM_MSG = 3,
	EGMI_STS_HTTP_MSG = 4,
	EGMI_STS_HTTP_MSG_RET = 5,

	EGMI_STS_ERROR_MSG = 6,
	////////////////////////////////////////////
	EGMI_NET_REGISTER_MASTER = 8, //masterע��Э��
	EGMI_NET_REGISTER_FAILED_MASTER = 9, //֪ͨע��ʧ�ܣ� masterע��Э��
	EGMI_NET_MASTER_SEND_OTHERS_TO_SERVER = 10, //master���ͱ�ķ�������Ϣ����ķ�����

	EGMI_NET_PROXY_TO_MASTER_REGISTER = 11,	//���ط�����	ע��	MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_REGISTER = 12,	//���ط�����	ע��	MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//��¼������	ע��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//���������	ע��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_REGISTER = 15,	//���ط�����	ע��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_REGISTER = 16,	//��Ϸ������	ע��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 17,	//���ط�����	ע��	WORLD������Э��
	EGMI_NET_PROXY_TO_LOGIN_REGISTER = 18,	//���ط�����	ע��	Login������Э��
	EGMI_NET_LOCATION_TO_MASTER_REGISTER = 19,	//Location������	ע��	MASTER������Э��
	EGMI_NET_LOG_TO_MASTER_REGISTER = 20,	//Log������	ע��	MASTER������Э��
	EGMI_NET_WEB_TO_MASTER_REGISTER = 21,	//Web������	ע��	MASTER������Э��

	EGMI_NET_PROXY_TO_MASTER_UNREGISTER = 41,	//���ط����� ע��  MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_UNREGISTER = 42,	//���ط����� ע��  MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_UNREGISTER = 43,	//��¼������	ע��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_UNREGISTER = 44,	//���������	ע��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_UNREGISTER = 45,		//���ط�����	ע��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_UNREGISTER = 46,		//��Ϸ������	ע��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_UNREGISTER = 47,	//���ط�����	ע��	WORLD������Э��
	EGMI_NET_PROXY_TO_LOGIN_UNREGISTER = 48,	//���ط�����	ע��	Login������Э��
	EGMI_NET_LOCATION_TO_MASTER_UNREGISTER = 49,	//Location������	ע��	MASTER������Э��
	EGMI_NET_LOG_TO_MASTER_UNREGISTER = 50,	//Log������	ע��	MASTER������Э��
	EGMI_NET_WEB_TO_MASTER_UNREGISTER = 51,	//Web������	ע��	MASTER������Э��

	EGMI_NET_PROXY_TO_MASTER_REFRESH = 61,	//���ط����� ˢ��		MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_REFRESH = 62,	//��Ϸ������	ˢ��	MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_REFRESH = 63,	//��¼������	ˢ��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_REFRESH = 64,	//���������	ˢ��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_REFRESH = 65,	//���ط�����	ˢ��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_REFRESH = 66,	//��Ϸ������	ˢ��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_REFRESH = 67,	//���ط�����	ˢ��	WORLD������Э��
	EGMI_NET_PROXY_TO_LOGIN_REFRESH = 68,	//���ط�����	ע��	Login������Э��
	EGMI_NET_LOCATION_TO_MASTER_REFRESH = 69,	//Location������	ˢ��	MASTER������Э��
	EGMI_NET_LOG_TO_MASTER_REFRESH = 70,	//Log������	ˢ��	MASTER������Э��
	EGMI_NET_WEB_TO_MASTER_REFRESH = 71,	//Web������	ˢ��	MASTER������Э��

	EGMI_NET_MASTER_SEND_OTHERS_TO_GAME = 80, //master send server to game
	EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD = 81, //master send server to world
	EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY = 82, //master send server to proxy
	EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN = 83, //master send server to login
	EGMI_NET_MASTER_SEND_OTHERS_TO_LOCATION = 84, //master send server to location
	EGMI_NET_MASTER_SEND_OTHERS_TO_LOG = 85, //master send server to log
	EGMI_NET_MASTER_SEND_OTHERS_TO_WEB = 86, //master send server to web

	EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT = 90, //proxy ���� �˺���������
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT = 91, //proxy ���� �˺���������
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT = 92, //proxy ���� �˺���������

	EGMI_NET_MSG_END = 1000,
};

enum NFServerEvent
{
	NF_SERVER_EVENT_GAME_DISCONNECT_PROXY = 100,
	NF_SERVER_EVENT_WORLD_DISCONNECT_PROXY = 101,
};