// -------------------------------------------------------------------------
//    @FileName         :    NFKernelPlugin.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-03-15
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#include "NFKernelPlugin.h"
#include "NFCKernelModule.h"
#include "NFCTimerModule.h"
#include "NFCEventModule.h"
#include "NFCLogModule.h"
#include "NFCConfigModule.h"
#include "NFCallBackModule.h"

//
//
#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
	CREATE_PLUGIN(pm, NFKernelPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
	DESTROY_PLUGIN(pm, NFKernelPlugin)
};

#endif

//////////////////////////////////////////////////////////////////////////

int NFKernelPlugin::GetPluginVersion()
{
	return 0;
}

std::string NFKernelPlugin::GetPluginName()
{
	return GET_CLASS_NAME(NFKernelPlugin);
}

void NFKernelPlugin::Install()
{
	/*
		log ϵͳ��һ��������Ȼ��������
	*/
	REGISTER_MODULE(m_pPluginManager, NFILogModule, NFCLogModule);
	REGISTER_MODULE(m_pPluginManager, NFIConfigModule, NFCConfigModule);
	REGISTER_MODULE(m_pPluginManager, NFITimerModule, NFCTimerModule);
	REGISTER_MODULE(m_pPluginManager, NFIEventModule, NFCEventModule);
	REGISTER_MODULE(m_pPluginManager, NFIKernelModule, NFCKernelModule);
	REGISTER_MODULE(m_pPluginManager, NFICallBackModule, NFCCallBackModule);
}

void NFKernelPlugin::Uninstall()
{
	UNREGISTER_MODULE(m_pPluginManager, NFIKernelModule, NFCKernelModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIEventModule, NFCEventModule);
	UNREGISTER_MODULE(m_pPluginManager, NFITimerModule, NFCTimerModule);
	UNREGISTER_MODULE(m_pPluginManager, NFILogModule, NFCLogModule);
	UNREGISTER_MODULE(m_pPluginManager, NFIConfigModule, NFCConfigModule);
	UNREGISTER_MODULE(m_pPluginManager, NFICallBackModule, NFCCallBackModule);
}

/**
* @brief ���ط���������,������ÿһ��Plugin����Ҫ�������������ֻ��kernelplugin�Ƚ��������
** ��Ϊconfig module ������ط��������ã������һ������
* @return bool
*/
bool NFKernelPlugin::OnReloadPlugin()
{
	NFIConfigModule* pConfigModule = (NFIConfigModule*)m_mapModule["NFIConfigModule"];
	if (pConfigModule)
	{
		if (pConfigModule->OnReloadPlugin() == false)
		{
			return true;
		}
	}

	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		if (m_vecModule[i] != pConfigModule)
		{
			m_vecModule[i]->OnReloadPlugin();
		}
	}

	return true;
}

