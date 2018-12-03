// -------------------------------------------------------------------------
//    @FileName         :    NFGameLogicPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameLogicPlugin
//
// -------------------------------------------------------------------------

#include "NFGameLogicPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, NFGameLogicPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, NFGameLogicPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFGameLogicPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFGameLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameClientPlugin);
}

void NFGameLogicPlugin::Install()
{
}

void NFGameLogicPlugin::Uninstall()
{
}
