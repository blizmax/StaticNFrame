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

NFIObject* NFConfigMgr::GetGlobalConfig() const
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetGlobalConfig();
	}

	return nullptr;
}

NFIObject* NFConfigMgr::CreateConfigObject(uint64_t guid, const std::string& className)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->CreateConfigObject(guid, className);
	}

	return nullptr;
}

NFIObject* NFConfigMgr::GetConfigObject(uint64_t guid, const std::string& className)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->GetConfigObject(guid, className);
	}

	return nullptr;
}

bool NFConfigMgr::ExistConfigObject(uint64_t guid, const std::string& className)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->ExistConfigObject(guid, className);
	}

	return false;
}

bool NFConfigMgr::DeleteConfigObject(uint64_t guid, const std::string& className)
{
	if (m_pConfigModule)
	{
		return m_pConfigModule->DeleteConfigObject(guid, className);
	}

	return false;
}


