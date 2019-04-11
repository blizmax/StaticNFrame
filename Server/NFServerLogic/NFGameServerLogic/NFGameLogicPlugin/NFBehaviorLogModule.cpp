// -------------------------------------------------------------------------
//    @FileName         :    NFBehaviorLogModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameLogicPlugin
//
// -------------------------------------------------------------------------

#include "NFBehaviorLogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"
#include "NFComm/NFPluginModule/NFISqliteModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFServerLogic/NFServerLogicCommon/NFServerLogicCommon.h"

#include "NFMessageDefine/db_behavior_log.pb.h"

NFCBehaviorLogModule::NFCBehaviorLogModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_pLogModule = nullptr;
	NFBehaviorLogMgr::GetSingletonPtr()->Init(this);
}

NFCBehaviorLogModule::~NFCBehaviorLogModule()
{

}

bool NFCBehaviorLogModule::Awake()
{
	m_pLogModule = m_pPluginManager->FindModule<NFILogModule>();
	if (m_pLogModule)
	{
		m_pLogModule->CreateOthersLogger(NF_LOG_BEHAVIOR_LOGIC_LOG, "behavior_log", false);
	}

	m_pSqliteModule = m_pPluginManager->FindModule<NFISqliteModule>();
	bool ret = m_pSqliteModule->AddSqliteServer(NF_GAME_BEHAVIOR_LOG_DB, "sqlite/behavior_log.db");
	if (ret == false)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "sqlite AddSqliteServer:sqlite/behavior_log failed!");
		return false;
	}

	ret = m_pSqliteModule->CreateTable(NF_GAME_BEHAVIOR_LOG_DB, NFMsg::db_behavior_log::default_instance());
	if (ret == false)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "sqlite CreateTable:behavior_log failed!");
		return false;
	}

	BehaviorLog(0, "gaoyi", "login", "AccountLogin", 0, "fuck you!");
	return true;
}

bool NFCBehaviorLogModule::Finalize()
{
	return true;
}

void NFCBehaviorLogModule::BehaviorLog(uint64_t guid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg)
{
	std::string str = NF_FORMAT("{} | {} | {} | {} | {} | {});", guid, nickName, category, tag, retCode, msg);
	m_pLogModule->LogOthers(NF_LOG_BEHAVIOR_LOGIC_LOG, NLL_INFO_NORMAL, NF_LOG_BEHAVIOR_LOGIC_LOG, guid, str);

	NFMsg::db_behavior_log msgLog;
	NFMsg::behavior_log_data* pData = msgLog.mutable_db_fields();
	if (pData)
	{
		pData->set_userid(guid);
		pData->set_nickname(nickName);
		pData->set_category(category);
		pData->set_tag(tag);
		pData->set_retcode(retCode);
		pData->set_msg(msg);
	}

	m_pSqliteModule->InsertMessage(NF_GAME_BEHAVIOR_LOG_DB, msgLog);
}