// -------------------------------------------------------------------------
//    @FileName         :    NFWorldClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFWorldClientPlugin
//
// -------------------------------------------------------------------------

#include "NFWorldClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFWorldClient_MasterModule.h"
#include "NFWorldClient_ProxyModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFWorldClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFWorldClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFWorldClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFWorldClientPlugin);
}

void NFWorldClientPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIWorldClient_MasterModule, NFCWorldClient_MasterModule);
	REGISTER_MODULE(pPluginManager, NFIWorldClient_ProxyModule, NFCWorldClient_ProxyModule);
}

void NFWorldClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIWorldClient_MasterModule, NFCWorldClient_MasterModule);
	UNREGISTER_MODULE(pPluginManager, NFIWorldClient_ProxyModule, NFCWorldClient_ProxyModule);
}
