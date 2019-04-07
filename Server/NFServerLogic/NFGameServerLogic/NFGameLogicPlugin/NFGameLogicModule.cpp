

#include "NFGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFISqliteModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

NFCGameLogicModule::NFCGameLogicModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCGameLogicModule::~NFCGameLogicModule()
{

}

bool NFCGameLogicModule::Init()
{
	bool ret = true;
	NFISqliteModule* pSqliteModule = m_pPluginManager->FindModule<NFISqliteModule>();
	ret = pSqliteModule->AddSqliteServer(NF_ST_GAME, "db_user");
	if (ret == false)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "sqlite AddSqliteServer:db_user failed!");
		return false;
	}

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	ret = pNosqlModule->AddConnectSql("nosql", "127.0.0.1");
	if (ret == false)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "redis connect 127.0.0.1 server failed!");
		return false;
	}
	return true;
}

bool NFCGameLogicModule::AfterInit()
{
	return true;
}

bool NFCGameLogicModule::Execute()
{
	return true;
}

bool NFCGameLogicModule::BeforeShut()
{
	return true;
}

bool NFCGameLogicModule::Shut()
{
	return true;
}