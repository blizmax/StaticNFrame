//------------------------------------------------------------------------ -
//    @FileName         :    NFMysqlPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include "NFMysqlPlugin.h"
#include "NFCMysqlModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFMysqlPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFMysqlPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFMysqlPlugin::GetPluginVersion()
{
    return 0;
}

std::string NFMysqlPlugin::GetPluginName()
{
    return GET_CLASS_NAME(NFMysqlPlugin);
}

void NFMysqlPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, NFIMysqlModule, NFCMysqlModule)
}

void NFMysqlPlugin::Uninstall()
{
    UNREGISTER_MODULE(pPluginManager, NFIMysqlModule, NFCMysqlModule)
}