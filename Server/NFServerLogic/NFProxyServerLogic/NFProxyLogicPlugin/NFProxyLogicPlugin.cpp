// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyLogicPlugin
//
// -------------------------------------------------------------------------

#include "NFProxyLogicPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCProxyLogicModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFProxyLogicPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFProxyLogicPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFProxyLogicPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFProxyLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFProxyLogicPlugin);
}

void NFProxyLogicPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIProxyLogicModule, NFCProxyLogicModule);
}

void NFProxyLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIProxyLogicModule, NFCProxyLogicModule);
}
