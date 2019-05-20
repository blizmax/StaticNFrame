// -------------------------------------------------------------------------
//    @FileName         :    NFGameLhdPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/05/20
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameLhdPlugin
//
// -------------------------------------------------------------------------

#include "NFGameLhdPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFGameLhdPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFGameLhdPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFGameLhdPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFGameLhdPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameLhdPlugin);
}

bool NFGameLhdPlugin::IsDynamicLoad()
{
	return true;
}

void NFGameLhdPlugin::Install()
{

}

void NFGameLhdPlugin::Uninstall()
{

}
