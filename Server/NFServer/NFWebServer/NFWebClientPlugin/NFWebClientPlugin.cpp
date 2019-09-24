// -------------------------------------------------------------------------
//    @FileName         :    NFWebClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFWebClientPlugin
//
// -------------------------------------------------------------------------

#include "NFWebClientPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFWebClient_MasterModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFWebClientPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFWebClientPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFWebClientPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFWebClientPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFWebClientPlugin);
}

void NFWebClientPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIWebClient_MasterModule, NFCWebClient_MasterModule)
}

void NFWebClientPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIWebClient_MasterModule, NFCWebClient_MasterModule)
}
