// -------------------------------------------------------------------------
//    @FileName         :    NFGameClientPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#include "NFCommonClient_MasterPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCCommonClient_MasterModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFCommonClient_MasterPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFCommonClient_MasterPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFCommonClient_MasterPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFCommonClient_MasterPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFCommonClient_MasterPlugin);
}

void NFCommonClient_MasterPlugin::Install()
{
	REGISTER_ALONE_MODULE(m_pPluginManager, NFICommonClient_MasterModule, NFCCommonClient_MasterModule);
}

void NFCommonClient_MasterPlugin::Uninstall()
{
	UNREGISTER_ALONE_MODULE(m_pPluginManager, NFICommonClient_MasterModule, NFCCommonClient_MasterModule);
}

