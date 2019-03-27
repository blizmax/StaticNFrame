// -------------------------------------------------------------------------
//    @FileName         :    NFLogMgr.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFLogMgr.h"

#include <sstream>

#include "NFILogModule.h"

NFLogMgr::NFLogMgr()
{
	m_pLogModule = nullptr;
}

NFLogMgr::~NFLogMgr()
{
}

bool NFLogMgr::Init(NFILogModule* pSpdlogModule)
{
	m_pLogModule = pSpdlogModule;
	return true;
}

void NFLogMgr::UnInit()
{
	m_pLogModule = nullptr;
}

void NFLogMgr::LogDefault(uint32_t log_level, uint32_t logId, uint64_t guid, const std::string& log)
{
	if (m_pLogModule)
	{
		m_pLogModule->LogDefault((NF_LOG_LEVEL)log_level, logId, guid, log);
	}
}

/**
* @brief ����Ĭ�ϵ�LOG������ȼ�
*
* @param  log_level log�ȼ�
* @return bool
*/
void NFLogMgr::SetDefaultLevel(NF_LOG_LEVEL log_level)
{
	if (m_pLogModule)
	{
		m_pLogModule->SetDefaultLevel(log_level);
	}
}

/**
* @brief ����Ĭ�ϵ�LOG��ˢ�µȼ�
*
* @param  log_level log�ȼ�
* @return bool
*/
void NFLogMgr::SetDefaultFlush(NF_LOG_LEVEL log_level)
{
	if (m_pLogModule)
	{
		m_pLogModule->SetDefaultFlush(log_level);
	}
}

