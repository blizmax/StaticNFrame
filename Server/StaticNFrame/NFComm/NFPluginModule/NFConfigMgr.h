// -------------------------------------------------------------------------
//    @FileName         :    NFConfigMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFILuaScriptModule.h"
#include "NFComm/NFCore/NFSingleton.hpp"
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
		mMasterId = 0;
		mWorldId = 0;
		mZoneId = 0;
		mServerPort = 0;
		mMaxConnectNum = 100;
		mWorkThreadNum = 1;
		mSecurity = false;
	}

public:
	std::string mServerName;
	uint32_t mServerType;
	uint32_t mServerId;
	uint32_t mMasterId;
	uint32_t mWorldId;
	uint32_t mZoneId;
	std::string mServerIp;
	uint32_t mServerPort;
	uint32_t mMaxConnectNum;
	uint32_t mWorkThreadNum;
	bool mSecurity;
};

class NFConfigMgr : public NFSingleton<NFConfigMgr>, public NFILuaScriptModule
{
public:
	NFConfigMgr()
	{
	}

	virtual ~NFConfigMgr()
	{
	}

public:
	int Init(NFIPluginManager* p)
	{
		pPluginManager = p;
		Init();
		return 0;
	}

	int UnInit()
	{
		Shut();
		return 0;
	}

	virtual bool Init() override;

	virtual bool Shut() override;

	NFPluginConfig* GetPluginConfig(const std::string& pluginName);
	NFServerConfig* GetServerConfig(uint32_t serverId);
	std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType);
protected:
	std::unordered_map<std::string, NFPluginConfig*> mPluginConfig; //pluginName--key
	std::unordered_map<int, NFServerConfig*> mServerConfig; //serverid--key
};
