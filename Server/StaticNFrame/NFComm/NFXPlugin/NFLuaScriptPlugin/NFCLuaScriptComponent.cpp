// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptComponent.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/08/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#include "NFCLuaScriptComponent.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

bool NFCLuaScriptComponent::Init()
{
	return true;
}

bool NFCLuaScriptComponent::AfterInit()
{
	return true;
}

bool NFCLuaScriptComponent::ReadyExecute()
{
	return true;
}

bool NFCLuaScriptComponent::Execute()
{
	return true;
}

bool NFCLuaScriptComponent::BeforeShut()
{
	return true;
}

bool NFCLuaScriptComponent::Shut()
{
	return true;
}

bool NFCLuaScriptComponent::Finalize()
{
	return true;
}

bool NFCLuaScriptComponent::Register()
{
	LuaIntf::LuaBinding(l).beginClass<NFIPluginManager>("NFIPluginManager")
		.addFunction("GetAppName", &NFIPluginManager::GetAppName)
		.addFunction("GetAppID", (int (NFIPluginManager::*)() const)&NFIPluginManager::GetAppID)
		.addFunction("GetInitTime", &NFIPluginManager::GetInitTime)
		.addFunction("GetNowTime", &NFIPluginManager::GetNowTime)
		.addFunction("IsLoadAllServer", &NFIPluginManager::IsLoadAllServer)
		.addFunction("GetConfigPath", &NFIPluginManager::GetConfigPath)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFILogModule>("NFILogModule")
		.addFunction("SetDefaultLevel", &NFILogModule::SetDefaultLevel)
		.addFunction("SetDefaultFlush", &NFILogModule::SetDefaultFlush)
		.addFunction("LuaDebug", &NFILogModule::LuaDebug)
		.addFunction("LuaInfo", &NFILogModule::LuaInfo)
		.addFunction("LuaWarn", &NFILogModule::LuaWarn)
		.addFunction("LuaError", &NFILogModule::LuaError)
		.endClass();
	return true;
}

void NFCLuaScriptComponent::LoadScript()
{
	TryAddPackagePath(m_pPluginManager->GetLuaScriptPath());
	TryLoadScriptFile("init.lua");

	TryRunGlobalScriptFunc("init_script_system", m_pPluginManager, this);
}