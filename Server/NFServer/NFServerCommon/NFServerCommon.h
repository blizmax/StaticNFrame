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
	
	EGMI_NET_PROXY_TO_MASTER_REGISTER = 11,	//���ط�����	ע��	MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_REGISTER = 12,	//���ط�����	ע��	MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//��¼������	ע��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//���������	ע��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_REGISTER = 15,	//���ط�����	ע��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_REGISTER = 16,	//��Ϸ������	ע��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 17,	//���ط�����	ע��	WORLD������Э��
	EGMI_NET_PROXY_TO_LOGIN_REGISTER = 18,	//���ط�����	ע��	Login������Э��

	EGMI_NET_PROXY_TO_MASTER_UNREGISTER = 21,	//���ط����� ע��  MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_UNREGISTER = 22,	//���ط����� ע��  MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_UNREGISTER = 23,	//��¼������	ע��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_UNREGISTER = 24,	//���������	ע��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_UNREGISTER = 25,		//���ط�����	ע��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_UNREGISTER = 26,		//��Ϸ������	ע��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_UNREGISTER = 27,	//���ط�����	ע��	WORLD������Э��

	EGMI_NET_PROXY_TO_MASTER_REFRESH = 31,	//���ط����� ˢ��		MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_REFRESH = 32,	//��Ϸ������	ˢ��	MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_REFRESH = 33,	//��¼������	ˢ��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_REFRESH = 34,	//���������	ˢ��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_REFRESH = 35,	//���ط�����	ˢ��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_REFRESH = 36,	//��Ϸ������	ˢ��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_REFRESH = 37,	//���ط�����	ˢ��	WORLD������Э��

	EGMI_NET_MASTER_SEND_OTHERS_TO_GAME = 40, //master send server to game
	EGMI_NET_MASTER_SEND_OTHERS_TO_WORLD = 41, //master send server to world
	EGMI_NET_MASTER_SEND_OTHERS_TO_PROXY = 42, //master send server to proxy
	EGMI_NET_MASTER_SEND_OTHERS_TO_LOGIN = 43, //master send server to server

	EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT = 50, //proxy ���� �˺���������
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT = 51, //proxy ���� �˺���������
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT = 52, //proxy ���� �˺���������

	EGMI_NET_MSG_END = 1000,
};

enum NFServerEvent
{
	NF_SERVER_EVENT_GAME_DISCONNECT_PROXY = 100,
	NF_SERVER_EVENT_WORLD_DISCONNECT_PROXY = 101,
};