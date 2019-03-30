// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFProxyClient_MasterModule.h"
#include "NFProxyClient_GameModule.h"
#include "NFProxyClient_WorldModule.h"
#include "NFProxyClient_LoginModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFProxyClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFProxyClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFProxyClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyClientPlugin);
}

void NFProxyClientPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_MasterModule, NFCProxyClient_MasterModule);
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_GameModule, NFCProxyClient_GameModule);
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_WorldModule, NFCProxyClient_WorldModule);
	REGISTER_MODULE(m_pPluginManager, NFIProxyClient_LoginModule, NFCProxyClient_LoginModule);
}

void NFProxyClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_MasterModule, NFCProxyClient_MasterModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_GameModule, NFCProxyClient_GameModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_WorldModule, NFCProxyClient_WorldModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyClient_LoginModule, NFCProxyClient_LoginModule);
}
