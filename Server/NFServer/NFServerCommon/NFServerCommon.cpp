// -------------------------------------------------------------------------
//    @FileName         :    NFServerCommon.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#include "NFServerCommon.h"

NFServerConfig* NFServerCommon::GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType)
{
	NFServerConfig* pServerConfig = nullptr;
	if (pPluginManager->IsLoadAllServer())
	{
		std::vector<NFServerConfig*> vecServerConfig = NFConfigMgr::Instance()->GetServerConfigFromServerType(eServerType);
		if (!vecServerConfig.empty())
		{
			pServerConfig = vecServerConfig.front();
		}
	}
	else
	{
		pServerConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
	}
	return pServerConfig;
}
