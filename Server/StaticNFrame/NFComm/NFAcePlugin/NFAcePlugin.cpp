//------------------------------------------------------------------------ -
//    @FileName         :    NFAcePlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-01-22
//    @Module           :    NFAcePlugin
//
// -------------------------------------------------------------------------

#include "NFAcePlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
//#include "NFCNetClientModule.h"
//#include "NFCNetServerModule.h"
//#include "NFCHttpClientModule.h"
//#include "NFCHttpServerModule.h"
//#include "NFServerNetEventModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFAcePlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFAcePlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFAcePlugin::GetPluginVersion()
{
	return 0;
}

std::string NFAcePlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFAcePlugin);
}

void NFAcePlugin::Install()
{
	//REGISTER_MODULE(pPluginManager, NFINetClientModule, NFCNetClientModule);
	//REGISTER_MODULE(pPluginManager, NFINetServerModule, NFCNetServerModule);
	//REGISTER_MODULE(pPluginManager, NFIHttpClientModule, NFCHttpClientModule);
	//REGISTER_MODULE(pPluginManager, NFIHttpServerModule, NFCHttpServerModule);
	//REGISTER_MODULE(pPluginManager, NFIServerNetEventModule, NFCServerNetEventModule);
}

void NFAcePlugin::Uninstall()
{
	//UNREGISTER_MODULE(pPluginManager, NFINetClientModule, NFCNetClientModule);
	//UNREGISTER_MODULE(pPluginManager, NFINetServerModule, NFCNetServerModule);
	//UNREGISTER_MODULE(pPluginManager, NFIHttpClientModule, NFCHttpClientModule);
	//UNREGISTER_MODULE(pPluginManager, NFIHttpServerModule, NFCHttpServerModule);
	//UNREGISTER_MODULE(pPluginManager, NFIServerNetEventModule, NFCServerNetEventModule);
}