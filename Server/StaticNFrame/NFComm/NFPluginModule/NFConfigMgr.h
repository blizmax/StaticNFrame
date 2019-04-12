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
	virtual uint32_t GetLogLevel() const;
	virtual uint32_t GetLogFlushLevel() const;
	virtual const std::vector<LogInfoConfig>& GetLogInfoConfig() const;
	virtual NFIObject* GetGlobalConfig() const;
protected:
	NFIConfigModule* m_pConfigModule;
};

#define GetGlobalConfigObject()  NFConfigMgr::Instance()->GetGlobalConfig()

