// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFLocationServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFLocationServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLocationServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLocationServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFLocationServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFLocationServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLocationServerPlugin);
}

void NFLocationServerPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFILocationServerModule, NFCLocationServerModule);
}

void NFLocationServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFILocationServerModule, NFCLocationServerModule);
}
