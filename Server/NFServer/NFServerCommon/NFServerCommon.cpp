// -------------------------------------------------------------------------
//    @FileName         :    NFServerCommon.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#include "NFServerCommon.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

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
		NFServerConfig* pAppConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
		if (pAppConfig == nullptr) return nullptr;

		std::vector<NFServerConfig*> vecServerConfig = NFConfigMgr::Instance()->GetServerConfigFromServerType(eServerType);
		if (!vecServerConfig.empty())
		{
			for (int i = 0; i < vecServerConfig.size(); i++)
			{
				NFServerConfig* pConfig = vecServerConfig[i];
				if (eServerType == NF_ST_MASTER)
				{			
					return pConfig;
				}
				else if (eServerType == NF_ST_LOGIN)
				{
					return pConfig;
				}
				else if (eServerType == NF_ST_WORLD)
				{
					return pConfig;
				}
				else if (eServerType == NF_ST_GAME)
				{
					return pConfig;
				}
				else if (eServerType == NF_ST_PROXY)
				{
					return pConfig;
				}
			}
		}
		
	}
	return pServerConfig;
}

NFServerConfig* NFServerCommon::GetAppConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType)
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
		if (pServerConfig && pServerConfig->mServerType != eServerType)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "GetAppConfig Failed, server id not match the server type!");
		}
	}
	return pServerConfig;
}