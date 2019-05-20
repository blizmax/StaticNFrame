// -------------------------------------------------------------------------
//    @FileName         :    NFGameHumanPlugin.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/7
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFGamePlayerPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFPlayerControllerModule.h"
#include "NFPlayerModule.h"
#include "NFPlayerServicesModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFGamePlayerPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFGamePlayerPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFGamePlayerPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFGamePlayerPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFGamePlayerPlugin);
}

bool NFGamePlayerPlugin::IsDynamicLoad()
{
	return true;
}

void NFGamePlayerPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFCPlayerControllerModule, NFCPlayerControllerModule);
	REGISTER_MODULE(m_pPluginManager, NFIPlayerModule, NFCPlayerModule);
	REGISTER_MODULE(m_pPluginManager, NFCPlayerServicesModule, NFCPlayerServicesModule);
}

void NFGamePlayerPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFCPlayerControllerModule, NFCPlayerControllerModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIPlayerModule, NFCPlayerModule);
	UNREGISTER_MODULE(m_pPluginManager, NFCPlayerServicesModule, NFCPlayerServicesModule);
}
