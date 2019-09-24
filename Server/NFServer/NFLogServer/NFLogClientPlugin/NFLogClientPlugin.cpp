// -------------------------------------------------------------------------
//    @FileName         :    NFLogClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFLogClientPlugin
//
// -------------------------------------------------------------------------

#include "NFLogClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFLogClient_MasterModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFLogClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFLogClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFLogClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFLogClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLogClientPlugin);
}

void NFLogClientPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFILogClient_MasterModule, NFCLogClient_MasterModule)
}

void NFLogClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFILogClient_MasterModule, NFCLogClient_MasterModule)
}
