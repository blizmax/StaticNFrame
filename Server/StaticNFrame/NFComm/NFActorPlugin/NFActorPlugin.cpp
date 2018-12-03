// -------------------------------------------------------------------------
//    @FileName         :    NFActorPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFActorPlugin
//
// -------------------------------------------------------------------------
#include "NFActorPlugin.h"
#include "NFCActorModule.h"
#include "NFCTaskModule.h"


#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFActorPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFActorPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFActorPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFActorPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFActorPlugin);
}

void NFActorPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIActorModule, NFCActorModule);
	REGISTER_MODULE(pPluginManager, NFITaskModule, NFCTaskModule);
}

void NFActorPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIActorModule, NFCActorModule);
	UNREGISTER_MODULE(pPluginManager, NFITaskModule, NFCTaskModule)
}

