// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFWebServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFWebServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFWebServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFWebServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFWebServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFWebServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFWebServerPlugin);
}

void NFWebServerPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIWebServerModule, NFCWebServerModule);
}

void NFWebServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIWebServerModule, NFCWebServerModule);
}
