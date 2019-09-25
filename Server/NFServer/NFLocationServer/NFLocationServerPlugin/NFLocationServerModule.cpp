// -------------------------------------------------------------------------
//    @FileName         :    NFCLocationServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLocationServerModule
//
// -------------------------------------------------------------------------

#include "NFLocationServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCLocationServerModule::NFCLocationServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCLocationServerModule::~NFCLocationServerModule()
{
}

bool NFCLocationServerModule::Init()
{
	NFServerConfig* pServerConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_LOCATION);
	if (!m_pPluginManager->IsLoadAllServer())
	{
		if (pServerConfig && pServerConfig->mServerType != NF_ST_LOCATION)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
			exit(0);
		}
	}

	return true;
}

bool NFCLocationServerModule::AfterInit()
{
	return true;
}

bool NFCLocationServerModule::Execute()
{
	return true;
}

bool NFCLocationServerModule::BeforeShut()
{
	return true;
}

bool NFCLocationServerModule::Shut()
{
	return true;
}