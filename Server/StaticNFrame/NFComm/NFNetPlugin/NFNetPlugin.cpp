//------------------------------------------------------------------------ -
//    @FileName         :    NFNetPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-15
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFNetPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCNetClientModule.h"
#include "NFCNetServerModule.h"
#include "NFCHttpClientModule.h"
#include "NFCHttpServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFNetPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFNetPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFNetPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFNetPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFNetPlugin);
}

void NFNetPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFINetClientModule, NFCNetClientModule);
	REGISTER_MODULE(pPluginManager, NFINetServerModule, NFCNetServerModule);
	REGISTER_MODULE(pPluginManager, NFIHttpClientModule, NFCHttpClientModule);
	REGISTER_MODULE(pPluginManager, NFIHttpServerModule, NFCHttpServerModule);
}

void NFNetPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFINetClientModule, NFCNetClientModule);
	UNREGISTER_MODULE(pPluginManager, NFINetServerModule, NFCNetServerModule);
	UNREGISTER_MODULE(pPluginManager, NFIHttpClientModule, NFCHttpClientModule);
	UNREGISTER_MODULE(pPluginManager, NFIHttpServerModule, NFCHttpServerModule);
}

