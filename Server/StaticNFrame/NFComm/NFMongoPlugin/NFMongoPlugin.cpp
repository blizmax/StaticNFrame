//------------------------------------------------------------------------ -
//    @FileName         :    NFMongoPlugin.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-20
//    @Module           :    NFMongoPlugin
//
// -------------------------------------------------------------------------

#include "NFMongoPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFCMongoModule.h"
#include "NFCAsynMongoModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFMongoPlugin)

};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFMongoPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int NFMongoPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFMongoPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFMongoPlugin);
}

void NFMongoPlugin::Install()
{
	REGISTER_MODULE(m_pPluginManager, NFIMongoModule, NFCMongoModule);
	REGISTER_MODULE(m_pPluginManager, NFIAsynMongoModule, NFCAsynMongoModule);
}

void NFMongoPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIMongoModule, NFCMongoModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIAsynMongoModule, NFCAsynMongoModule);
}

