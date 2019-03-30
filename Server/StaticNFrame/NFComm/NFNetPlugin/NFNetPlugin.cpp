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
#include "NFServerNetEventModule.h"

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
	REGISTER_MODULE(m_pPluginManager, NFINetClientModule, NFCNetClientModule);
	REGISTER_MODULE(m_pPluginManager, NFINetServerModule, NFCNetServerModule);
	REGISTER_MODULE(m_pPluginManager, NFIHttpClientModule, NFCHttpClientModule);
	REGISTER_MODULE(m_pPluginManager, NFIHttpServerModule, NFCHttpServerModule);
	REGISTER_MODULE(m_pPluginManager, NFIServerNetEventModule, NFCServerNetEventModule);
}

void NFNetPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFINetClientModule, NFCNetClientModule);
	UNREGISTER_MODULE(m_pPluginManager, NFINetServerModule, NFCNetServerModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIHttpClientModule, NFCHttpClientModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIHttpServerModule, NFCHttpServerModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIServerNetEventModule, NFCServerNetEventModule);
}

