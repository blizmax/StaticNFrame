// -------------------------------------------------------------------------
//    @FileName         :    NFTestPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFTestPlugin.h"
#include "NFCTestKernelModule.h"
#include "NFCTestClientModule.h"
#include "NFCTestServerModule.h"
#include "NFCTestActorModule.h"
#include "NFCTestObjectModule.h"
//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFTestPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFTestPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFTestPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFTestPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFTestPlugin);
}

void NFTestPlugin::Install()
{
	//REGISTER_MODULE(pPluginManager, NFITestKernelModule, NFCTestKernelModule);
	//REGISTER_MODULE(pPluginManager, NFCTestClientModule, NFCTestClientModule);
	//REGISTER_MODULE(pPluginManager, NFCTestServerModule, NFCTestServerModule);
	REGISTER_MODULE(pPluginManager, NFCTestActorModule, NFCTestActorModule);
	REGISTER_MODULE(pPluginManager, NFCTestObjectModule, NFCTestObjectModule);
}

void NFTestPlugin::Uninstall()
{
	//UNREGISTER_MODULE(pPluginManager, NFITestKernelModule, NFCTestKernelModule);
	//UNREGISTER_MODULE(pPluginManager, NFCTestClientModule, NFCTestClientModule);
	//UNREGISTER_MODULE(pPluginManager, NFCTestServerModule, NFCTestServerModule);
	UNREGISTER_MODULE(pPluginManager, NFCTestActorModule, NFCTestActorModule);
	UNREGISTER_MODULE(pPluginManager, NFCTestObjectModule, NFCTestObjectModule);
}