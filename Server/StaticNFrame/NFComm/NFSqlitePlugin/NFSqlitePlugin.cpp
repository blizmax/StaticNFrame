//------------------------------------------------------------------------ -
//    @FileName         :    NFSqlitePlugin.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-03-21
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include "NFSqlitePlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCSqliteModule.h"
#include "NFCAsyncSqliteModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFSqlitePlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFSqlitePlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFSqlitePlugin::GetPluginVersion()
{
	return 0;
}

std::string NFSqlitePlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFSqlitePlugin);
}

void NFSqlitePlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFISqliteModule, NFCSqliteModule);
	//REGISTER_MODULE(pPluginManager, NFIAsynMongoModule, NFCAsynMongoModule);
}

void NFSqlitePlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFISqliteModule, NFCSqliteModule);
	//UNREGISTER_MODULE(pPluginManager, NFIAsynMongoModule, NFCAsynMongoModule);
}

