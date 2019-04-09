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
		mMongoPort = 0;
		mServerPort = 0;
		mMaxConnectNum = 100;
		mWorkThreadNum = 1;
		mSecurity = false;
		mWebSocket = false;
		mHttpPort = 0;
		mMysqlPort = 0;
		mNosqlPort = 0;
	}

public:
	std::string mServerName;	//服务器名字
	uint32_t mServerType;	//服务器类型
	uint32_t mServerId;		//服务器唯一ID
	std::string mGameName;	//游戏名字
	std::string mMongoIp;	//mongodb 地址
	uint32_t	mMongoPort;	//mongodb port
	std::string mMongoDbName; //mongo dbname数据库名字
	std::string mMysqlIp;
	uint32_t	mMysqlPort;
	std::string mMysqlDbName;
	std::string mMysqlUser;
	std::string mMysqlPassword;
	std::string mNosqlIp;
	uint32_t	mNosqlPort;
	std::string mServerIp;  //服务器监听IP
	uint32_t mServerPort;	//服务器监听端口
	uint32_t mHttpPort;		//服务器http监听ip
	uint32_t mMaxConnectNum;	//最大连接数目
	uint32_t mWorkThreadNum;	//线程数目
	bool mSecurity;				//是否加密
	bool mWebSocket;			//是否开启websocket功能
};

class NFIConfigModule : public NFIModule
{
public:
	virtual NFPluginConfig* GetPluginConfig(const std::string& pluginName) = 0;
	virtual NFServerConfig* GetServerConfig(uint32_t serverId) = 0;
	virtual std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType) = 0;
	virtual uint32_t GetLogLevel() const = 0;
	virtual uint32_t GetLogFlushLevel() const = 0;
	virtual const std::vector<LogInfoConfig>& GetLogInfoConfig() const = 0;
};
