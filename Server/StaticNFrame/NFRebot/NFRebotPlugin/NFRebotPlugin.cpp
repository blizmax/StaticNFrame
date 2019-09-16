// -------------------------------------------------------------------------
//    @FileName         :    NFTestPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFRebotPlugin
//
// -------------------------------------------------------------------------

#include "NFRebotPlugin.h"
#include "NFCRebotModule.h"
//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFRebotPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFRebotPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFRebotPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFRebotPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFRebotPlugin);
}

void NFRebotPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFCRebotModule, NFCRebotModule);
}

void NFRebotPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFCRebotModule, NFCRebotModule);
}