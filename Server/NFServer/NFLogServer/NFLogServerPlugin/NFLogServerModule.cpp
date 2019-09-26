// -------------------------------------------------------------------------
//    @FileName         :    NFCLogServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLogServerModule
//
// -------------------------------------------------------------------------

#include "NFLogServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"

NFCLogServerModule::NFCLogServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCLogServerModule::~NFCLogServerModule()
{
}

bool NFCLogServerModule::Init()
{
	NFServerConfig* pServerConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_LOG);
	if (!m_pPluginManager->IsLoadAllServer())
	{
		if (pServerConfig && pServerConfig->mServerType != NF_ST_LOG)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "server config error, server id not match the server type!");
			exit(0);
		}
	}

	return true;
}

bool NFCLogServerModule::AfterInit()
{
	return true;
}

bool NFCLogServerModule::Execute()
{
	return true;
}

bool NFCLogServerModule::BeforeShut()
{
	return true;
}

bool NFCLogServerModule::Shut()
{
	return true;
}