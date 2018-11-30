// -------------------------------------------------------------------------
//    @FileName         :    NFWorldServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFWorldServerPlugin
//
// -------------------------------------------------------------------------

#include "NFWorldServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFWorldServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFWorldServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFWorldServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFWorldServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFWorldServerPlugin);
}

void NFWorldServerPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIWorldServerModule, NFCWorldServerModule)
}

void NFWorldServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIWorldServerModule, NFCWorldServerModule)
}
