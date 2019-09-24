// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#include "NFLogServerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFLogServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFLogServerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFLogServerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFLogServerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFLogServerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLogServerPlugin);
}

void NFLogServerPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFILogServerModule, NFCLogServerModule);
}

void NFLogServerPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFILogServerModule, NFCLogServerModule);
}
