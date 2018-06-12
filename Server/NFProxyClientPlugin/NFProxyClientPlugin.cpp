// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFProxyClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFProxyClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFProxyClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyClientPlugin);
}

void NFProxyClientPlugin::Install()
{
}

void NFProxyClientPlugin::Uninstall()
{
}
