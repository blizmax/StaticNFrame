// -------------------------------------------------------------------------
//    @FileName         :    NFBattleServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    
//
// -------------------------------------------------------------------------
#include "NFBattleServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFBattleServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFBattleServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFBattleServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFBattleServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFBattleServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFBattleServerPlugin);
}

void NFBattleServerPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIBattleServerModule, NFCBattleServerModule);
}

void NFBattleServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIBattleServerModule, NFCBattleServerModule)
}