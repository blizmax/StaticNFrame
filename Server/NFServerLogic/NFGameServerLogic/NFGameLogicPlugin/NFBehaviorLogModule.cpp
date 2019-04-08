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

NFCBehaviorLogModule::NFCBehaviorLogModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_pLogModule = nullptr;
	NFBehaviorLogMgr::GetSingletonPtr()->Init(this);
}

NFCBehaviorLogModule::~NFCBehaviorLogModule()
{

}

bool NFCBehaviorLogModule::Init()
{
	m_pLogModule = m_pPluginManager->FindModule<NFILogModule>();
	if (m_pLogModule)
	{
		m_pLogModule->CreateOthersLogger(NF_LOG_BEHAVIOR_LOGIC_LOG, "behavior_log", false);
	}

	NFISqliteModule* pSqliteModule = m_pPluginManager->FindModule<NFISqliteModule>();
	bool ret = pSqliteModule->AddSqliteServer(NF_ST_GAME, "sqlite/behavior_log");
	if (ret == false)
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "sqlite AddSqliteServer:sqlite/behavior_log failed!");
		return false;
	}
	return true;
}

bool NFCBehaviorLogModule::Shut()
{
	return true;
}

void NFCBehaviorLogModule::BehaviorLog(const std::string& cid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg)
{
	std::string str = NF_FORMAT("{} | {} | {} | {} | {} | {});", cid, nickName, category, tag, retCode, msg);
	m_pLogModule->LogOthers(NF_LOG_BEHAVIOR_LOGIC_LOG, NLL_INFO_NORMAL, NF_LOG_BEHAVIOR_LOGIC_LOG, 0, str);
}