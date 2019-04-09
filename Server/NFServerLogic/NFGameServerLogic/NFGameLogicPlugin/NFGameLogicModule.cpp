

#include "NFGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFISqliteModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"

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
	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_GAME);
	if (pConfig == nullptr)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "NFServerCommon::GetAppConfig:NF_ST_GAME failed!");
		return false;
	}

	NFIMysqlModule* pDBModule = m_pPluginManager->FindModule<NFIMysqlModule>();
	ret = pDBModule->AddMysqlServer(NF_ST_GAME, pConfig->mMysqlIp, pConfig->mMysqlPort, pConfig->mMysqlDbName, pConfig->mMysqlUser, pConfig->mMysqlPassword);
	if (ret == false)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "sqlite AddSqliteServer:db_user failed!");
		return false;
	}

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	ret = pNosqlModule->AddConnectSql("nosql", pConfig->mNosqlIp, pConfig->mNosqlPort);
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