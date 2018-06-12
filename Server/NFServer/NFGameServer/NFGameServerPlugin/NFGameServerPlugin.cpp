// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFGameServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFGameServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFGameServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameServerPlugin);
}

void NFGameServerPlugin::Install()
{
}

void NFGameServerPlugin::Uninstall()
{
}
