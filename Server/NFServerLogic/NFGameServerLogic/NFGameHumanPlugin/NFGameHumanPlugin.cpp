// -------------------------------------------------------------------------
//    @FileName         :    NFGameHumanPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/7
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFGameHumanPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFGameHumanPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFGameHumanPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFGameHumanPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFGameHumanPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGameHumanPlugin);
}

void NFGameHumanPlugin::Install()
{
	//REGISTER_MODULE(m_pPluginManager, NFIGameLogicModule, NFCGameLogicModule);
}

void NFGameHumanPlugin::Uninstall()
{
	//UNREGISTER_MODULE(m_pPluginManager, NFIGameLogicModule, NFCGameLogicModule);
}
