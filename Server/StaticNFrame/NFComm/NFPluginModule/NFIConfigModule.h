// -------------------------------------------------------------------------
//    @FileName         :    NFIConfigModule.h
//    @Author           :    GaoYi
//    @Date             :    2018-12-1
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIModule.h"

#include "NFIPluginManager.h"
#include "NFServerDefine.h"

#include <unordered_map>
#include <vector>


struct NFPluginConfig
{
	NFPluginConfig()
	{
		mServerType = NF_ST_NONE;
	}

public:
	std::vector<std::string> mVecPlugins;
	uint32_t mServerType;
};

struct NFServerConfig
{
public:
	NFServerConfig()
	{
		mServerType = NF_ST_NONE;
		mServerId = 0;
		mGameId = 0;
		mMongoPort = 0;
		mServerPort = 0;
		mMaxConnectNum = 100;
		mWorkThreadNum = 1;
		mSecurity = false;
		mWebSocket = false;
		mHttpPort = 0;
		mServerInnerPort = 0;
	}

public:
	std::string mServerName;	//����������
	uint32_t mServerType;	//����������
	uint32_t mServerId;		//������ΨһID
	uint32_t mGameId;	//��ϷID
	std::string mGameName;	//��Ϸ����
	std::string mMongoIp;	//mongodb ��ַ
	uint32_t	mMongoPort;	//mongodb port
	std::string mMongoDbName; //mongo dbname���ݿ�����
	std::string mServerIp;  //����������IP
	uint32_t mServerPort;	//�����������˿�
	uint32_t mServerInnerPort;	//���ڷ������˿ڣ�ֻҪ��������
	uint32_t mHttpPort;		//������http����ip
	uint32_t mMaxConnectNum;	//���������Ŀ
	uint32_t mWorkThreadNum;	//�߳���Ŀ
	bool mSecurity;				//�Ƿ����
	bool mWebSocket;			//�Ƿ���websocket����
};

class NFIConfigModule : public NFIModule
{
public:
	virtual NFPluginConfig* GetPluginConfig(const std::string& pluginName) = 0;
	virtual NFServerConfig* GetServerConfig(uint32_t serverId) = 0;
	virtual std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType) = 0;
	virtual uint32_t GetLogLevel() const = 0;
	virtual uint32_t GetLogFlushLevel() const = 0;
};
