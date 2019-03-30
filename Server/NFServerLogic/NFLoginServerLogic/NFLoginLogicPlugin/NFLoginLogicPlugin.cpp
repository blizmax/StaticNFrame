// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyLogicPlugin
//
// -------------------------------------------------------------------------

#include "NFLoginLogicPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCLoginLogicModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFLoginLogicPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFLoginLogicPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFLoginLogicPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFLoginLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFLoginLogicPlugin);
}

void NFLoginLogicPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFILoginLogicModule, NFCLoginLogicModule);
}

void NFLoginLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFILoginLogicModule, NFCLoginLogicModule);
}
