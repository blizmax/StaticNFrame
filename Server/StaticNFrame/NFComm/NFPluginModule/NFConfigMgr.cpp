// -------------------------------------------------------------------------
//    @FileName         :    NFConfigMgr.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFConfigMgr.h"

#include "NFIConfigModule.h"

bool NFConfigMgr::Init(NFIConfigModule* pConfigModule)
{
	m_pConfigModule = pConfigModule;
	return true;
}

void NFConfigMgr::UnInit()
{
	m_pConfigModule = nullptr;
}

NFPluginConfig* NFConfigMgr::GetPluginConfig(const std::string& pluginName)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetPluginConfig(pluginName);
	}
	return nullptr;
}

NFServerConfig* NFConfigMgr::GetServerConfig(uint32_t serverId)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetServerConfig(serverId);
	}
	return nullptr;
}

std::vector<NFServerConfig*> NFConfigMgr::GetServerConfigFromServerType(uint32_t serverType)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetServerConfigFromServerType(serverType);
	}
	return std::vector<NFServerConfig*>();
}

uint32_t NFConfigMgr::GetLogLevel() const
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetLogLevel();
	}
	return 0;
}

uint32_t NFConfigMgr::GetLogFlushLevel() const
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetLogFlushLevel();
	}
	return 0;
}

const std::vector<LogInfoConfig>& NFConfigMgr::GetLogInfoConfig() const
{
	static std::vector<LogInfoConfig> vec;

	if (m_pConfigModule)
	{
		return m_pConfigModule->GetLogInfoConfig();
	}

	return vec;
}

NFIObject* NFConfigMgr::GetGlobalConfig() const
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetGlobalConfig();
	}

	return nullptr;
}


