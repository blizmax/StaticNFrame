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

	virtual bool Awake() override;
	virtual bool Init() override;
	virtual bool AfterInit() override;
	virtual bool BeforeShut() override;
	virtual bool Shut() override;
	virtual bool Execute() override;
	virtual bool OnReloadPlugin() override;
	virtual void ProductFile() override;
	virtual void CreateHeaderFile();
	virtual void CreateSqlFile();
public:
	bool LoadConfig();
	bool LoadPluginConfig();
	bool LoadServerConfig();
	bool LoadLogConfig();
	bool LoadClassNode();
	bool LoadDBTable();
public:
	virtual NFPluginConfig* GetPluginConfig(const std::string& pluginName);
	virtual NFServerConfig* GetServerConfig(uint32_t serverId);
	virtual std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType);
	virtual NFIObject* GetGlobalConfig() const { return mGlobalConfig; }
	virtual NFClassObject* GetClassObject(const std::string& className) const;
protected:
	std::unordered_map<std::string, NFPluginConfig*> mPluginConfig; //pluginName--key
	std::unordered_map<int, NFServerConfig*> mServerConfig; //serverid--key
	NFIObject* mGlobalConfig;
	std::unordered_map<std::string, NFClassObject*> mClassObjectConfig;
	std::unordered_map<std::string, NFDBTable*> mDBTableColConfig;
};
