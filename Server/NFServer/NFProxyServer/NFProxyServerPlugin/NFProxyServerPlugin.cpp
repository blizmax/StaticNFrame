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
	//REGISTER_MODULE(pPluginManager, NFIMysqlModule, NFCMysqlModule)
}

void NFProxyServerPlugin::Uninstall()
{
	//UNREGISTER_MODULE(pPluginManager, NFIMysqlModule, NFCMysqlModule)
}
