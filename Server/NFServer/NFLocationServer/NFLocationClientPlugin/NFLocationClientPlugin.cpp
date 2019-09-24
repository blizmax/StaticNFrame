// -------------------------------------------------------------------------
//    @FileName         :    NFLocationClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFLocationClientPlugin
//
// -------------------------------------------------------------------------

#include "NFLocationClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFLocationClient_MasterModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLocationClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLocationClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFLocationClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFLocationClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLocationClientPlugin);
}

void NFLocationClientPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFILocationClient_MasterModule, NFCLocationClient_MasterModule)
}

void NFLocationClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFILocationClient_MasterModule, NFCLocationClient_MasterModule)
}
