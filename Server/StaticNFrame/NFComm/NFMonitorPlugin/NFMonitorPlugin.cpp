//------------------------------------------------------------------------ -
//    @FileName         :    NFSqlitePlugin.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-03-21
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include "NFMonitorPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFMonitorPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFMonitorPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFMonitorPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFMonitorPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFMonitorPlugin);
}

void NFMonitorPlugin::Install()
{
	//REGISTER_MODULE(pPluginManager, NFISqliteModule, NFCSqliteModule);
	//REGISTER_MODULE(pPluginManager, NFIAsynMongoModule, NFCAsynMongoModule);
}

void NFMonitorPlugin::Uninstall()
{
	//UNREGISTER_MODULE(pPluginManager, NFISqliteModule, NFCSqliteModule);
	//UNREGISTER_MODULE(pPluginManager, NFIAsynMongoModule, NFCAsynMongoModule);
}

