// -------------------------------------------------------------------------
//    @FileName         :    NFWorldServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFWorldServerPlugin
//
// -------------------------------------------------------------------------

#include "NFLoginClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFLoginClient_MasterModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLoginClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLoginClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFLoginClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFLoginClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLoginClientPlugin);
}

void NFLoginClientPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFILoginClient_MasterModule, NFCLoginClient_MasterModule)
}

void NFLoginClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFILoginClient_MasterModule, NFCLoginClient_MasterModule)
}
