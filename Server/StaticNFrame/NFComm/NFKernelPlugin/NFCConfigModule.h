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
#include "NFComm/NFPluginModule/NFIObject.h"
#include <string>

class NFCConfigModule : public NFIConfigModule, public NFILuaModule
{
public:
	explicit NFCConfigModule(NFIPluginManager* p);

	virtual ~NFCConfigModule();

	bool Awake() override;
	bool Init() override;
	bool AfterInit() override;
	bool BeforeShut() override;
	bool Shut() override;
	bool Execute() override;
	bool OnReloadPlugin() override;
	void ProductFile() override;
	void CreateHeaderFile();
	void CreateSqlFile();
public:
	bool LoadConfig();
	bool LoadPluginConfig();
	bool LoadServerConfig();
	bool LoadClassNode();
	bool LoadDBTable();

	bool LoadLogConfig();
public:
	NFPluginConfig* GetPluginConfig(const std::string& pluginName) override;
	NFServerConfig* GetServerConfig(uint32_t serverId) override;
	std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType) override;
	NFIObject* GetGlobalConfig() const override { return mGlobalConfig; }
	NFClassObject* GetClassObject(const std::string& className) const override;

	NFIObject* CreateConfigObject(uint64_t guid, const std::string& className) override;
	NFIObject* GetConfigObject(uint64_t guid, const std::string& className) override;
	bool ExistConfigObject(uint64_t guid, const std::string& className) override;
	bool DeleteConfigObject(uint64_t guid, const std::string& className) override;
protected:
	std::unordered_map<std::string, NFPluginConfig*> mPluginConfig; //pluginName--key
	std::unordered_map<int, NFServerConfig*> mServerConfig; //serverid--key
	NFIObject* mGlobalConfig;
	std::unordered_map<std::string, std::unordered_map<uint64_t, NFIObject*>> mObjectMap;
	std::unordered_map<std::string, NFClassObject*> mClassObjectConfig;
	std::unordered_map<std::string, NFDBTable*> mDBTableColConfig;
};
