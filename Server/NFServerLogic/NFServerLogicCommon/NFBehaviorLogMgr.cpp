// -------------------------------------------------------------------------
//    @FileName         :    NFBehaviorLogMgr.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/08
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerLogicCommon
//
// -------------------------------------------------------------------------
#include "NFBehaviorLogMgr.h"

#include <sstream>

#include "NFIBehaviorLogModule.h"


NFBehaviorLogMgr::NFBehaviorLogMgr()
{
	m_pLogModule = nullptr;
}

NFBehaviorLogMgr::~NFBehaviorLogMgr()
{
}

bool NFBehaviorLogMgr::Init(NFIBehaviorLogModule* pSpdlogModule)
{
	m_pLogModule = pSpdlogModule;
	return true;
}

void NFBehaviorLogMgr::UnInit()
{
	m_pLogModule = nullptr;
}

/**
* @brief 记录行为LOG
*
* @param  guid	玩家id
* @param  nickName 玩家名字
* @param  category
* @return bool
*/
void NFBehaviorLogMgr::BehaviorLog(uint64_t guid, const std::string& nickName, const std::string& category, const std::string& tag, uint32_t retCode, const std::string& msg)
{
	if (m_pLogModule)
	{
		m_pLogModule->BehaviorLog(guid, nickName, category, tag, retCode, msg);
	}
}

