//------------------------------------------------------------------------ -
//    @FileName         :    NFMysqlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include "NFMongoPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCMongoModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFMongoPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFMongoPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFMongoPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFMongoPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFMongoPlugin);
}

void NFMongoPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, NFIMongoModule, NFCMongoModule)
}

void NFMongoPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, NFIMongoModule, NFCMongoModule)
}

