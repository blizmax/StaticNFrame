// -------------------------------------------------------------------------
//    @FileName         :    NFConfigMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFSingleton.hpp"

#include <unordered_map>
#include <vector>

#include "NFIConfigModule.h"

class _NFExport NFConfigMgr : public NFSingleton<NFConfigMgr>
{
public:
	NFConfigMgr()
	{
		m_pConfigModule = nullptr;
	}

	virtual ~NFConfigMgr()
	{

	}

	bool Init(NFIConfigModule* pConfigModule = nullptr);
	void UnInit();

	virtual NFPluginConfig* GetPluginConfig(const std::string& pluginName);
	virtual NFServerConfig* GetServerConfig(uint32_t serverId);
	virtual std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType);
	virtual NFIObject* GetGlobalConfig() const;

	virtual NFIObject* CreateConfigObject(uint64_t guid, const std::string& className);
	virtual NFIObject* GetConfigObject(uint64_t guid, const std::string& className);
	virtual bool ExistConfigObject(uint64_t guid, const std::string& className);
	virtual bool DeleteConfigObject(uint64_t guid, const std::string& className);
protected:
	NFIConfigModule* m_pConfigModule;
};

#define GetGlobalConfigObject()  NFConfigMgr::Instance()->GetGlobalConfig()

