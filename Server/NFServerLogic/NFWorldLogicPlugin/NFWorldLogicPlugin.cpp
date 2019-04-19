// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyLogicPlugin
//
// -------------------------------------------------------------------------

#include "NFWorldLogicPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCWorldLogicModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFWorldLogicPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFWorldLogicPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFWorldLogicPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFWorldLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFWorldLogicPlugin);
}

void NFWorldLogicPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIWorldLogicModule, NFCWorldLogicModule);
}

void NFWorldLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIWorldLogicModule, NFCWorldLogicModule);
}
