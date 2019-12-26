// -------------------------------------------------------------------------
//    @FileName         :    NFProxyServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFProxyServerModule.h"
#include "NFProxyClient_GameModule.h"
#include "NFProxyClient_WorldModule.h"
#include "NFProxyClient_LoginModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFProxyServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFProxyServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFProxyServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyServerPlugin);
}

void NFProxyServerPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIProxyServerModule, NFCProxyServerModule);
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_GameModule, NFCProxyClient_GameModule);
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_WorldModule, NFCProxyClient_WorldModule);
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_LoginModule, NFCProxyClient_LoginModule);
}

void NFProxyServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyServerModule, NFCProxyServerModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_GameModule, NFCProxyClient_GameModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_WorldModule, NFCProxyClient_WorldModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_LoginModule, NFCProxyClient_LoginModule);
}
