// -------------------------------------------------------------------------
//    @FileName         :    NFServerCheckPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/11/9
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCheckPlugin
//
// -------------------------------------------------------------------------

#include "NFServerCheckPlugin.h"
#include "NFCServerCheckModule.h"
//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFServerCheckPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFServerCheckPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFServerCheckPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFServerCheckPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFServerCheckPlugin);
}

void NFServerCheckPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFCServerCheckModule, NFCServerCheckModule);
}

void NFServerCheckPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFCServerCheckModule, NFCServerCheckModule);
}