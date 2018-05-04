// -------------------------------------------------------------------------
//    @FileName         :    NFKernelPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#include "NFTestPlugin.h"
#include "NFCTestKernelModule.h"
#include "NFCTestClientModule.h"

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

const int NFTestPlugin::GetPluginVersion()
{
    return 0;
}

const std::string NFTestPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFTestPlugin);
}

void NFTestPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFITestKernelModule, NFCTestKernelModule);
	REGISTER_MODULE(pPluginManager, NFCTestClientModule, NFCTestClientModule);
}

void NFTestPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFITestKernelModule, NFCTestKernelModule);
	UNREGISTER_MODULE(pPluginManager, NFCTestClientModule, NFCTestClientModule);
}