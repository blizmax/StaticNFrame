// -------------------------------------------------------------------------
//    @FileName         :    NFCConfigModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFKernelPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIConfigModule.h"

#include "NFComm/NFPluginModule/NFILuaModule.h"

class NFCConfigModule : public NFIConfigModule, public NFILuaModule
{
public:
	explicit NFCConfigModule(NFIPluginManager* p);

	virtual ~NFCConfigModule();

	virtual bool Awake() override;
	virtual bool Init() override;
	virtual bool AfterInit() override;
	virtual bool BeforeShut() override;
	virtual bool Shut() override;
	virtual bool Execute() override;
	virtual bool OnReloadPlugin() override;
public:
	bool LoadConfig();
	bool LoadPluginConfig();
	bool LoadServerConfig();
	bool LoadLogConfig();
public:
	virtual NFPluginConfig* GetPluginConfig(const std::string& pluginName);
	virtual NFServerConfig* GetServerConfig(uint32_t serverId);
	virtual std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType);
	virtual uint32_t GetLogLevel() const { return mLogLevel; }
	virtual uint32_t GetLogFlushLevel() const { return mLogFlushLevel; }
	virtual const std::vector<LogInfoConfig>& GetLogInfoConfig() const { return mLogInfoConfig; }
protected:
	std::unordered_map<std::string, NFPluginConfig*> mPluginConfig; //pluginName--key
	std::unordered_map<int, NFServerConfig*> mServerConfig; //serverid--key
	uint32_t mLogLevel;
	uint32_t mLogFlushLevel;
	std::vector<LogInfoConfig> mLogInfoConfig;
};
