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

	//3 Master�յ�GMЭ�����ݣ�ת��������
	EGMI_STS_GM_MSG = 3,
	////////////////////////////////////////////
	
	EGMI_NET_PROXY_TO_MASTER_REGISTER = 11,	//���ط�����	ע��	MASTER������Э��
	EGMI_NET_GAME_TO_MASTER_REGISTER = 12,	//���ط�����	ע��	MASTER������Э��
	EGMI_NET_LOGIN_TO_MASTER_REGISTER = 13,	//��¼������	ע��	MASTER������Э��
	EGMI_NET_WORLD_TO_MASTER_REGISTER = 14,	//���������	ע��	MASTER������Э��
	EGMI_NET_PROXY_TO_GAME_REGISTER = 15,	//���ط�����	ע��	��Ϸ������Э��
	EGMI_NET_GAME_TO_WORLD_REGISTER = 16,	//��Ϸ������	ע��	���������Э��
	EGMI_NET_PROXY_TO_WORLD_REGISTER = 17,	//���ط�����	ע��	WORLD������Э��

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

	EGMI_NET_MASTER_SEND_SERVER_TO_SERVER = 40, //master send server to server

	EGMI_NET_PROXY_TO_GAME_ACCOUNT_CONNECT = 50, //proxy ���� �˺���������
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_DISCONNECT = 51, //proxy ���� �˺���������
	EGMI_NET_PROXY_TO_GAME_ACCOUNT_RECONNECT = 52, //proxy ���� �˺���������

	EGMI_NET_LOGIN_TO_MASTER_PLAT_LOGIN = 60, //plat login
	EGMI_NET_MASTER_TO_LOGIN_PLAT_LOGIN = 61, //plat login
	EGMI_NET_MSG_END = 1000,
};

enum NFServerEvent
{
	NF_SERVER_EVENT_GAME_DISCONNECT_PROXY = 100,
	NF_SERVER_EVENT_WORLD_DISCONNECT_PROXY = 101,
};