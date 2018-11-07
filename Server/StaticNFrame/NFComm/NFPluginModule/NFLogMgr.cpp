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
	
}

bool NFLogMgr::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func, int line)
{
	if (m_pLogModule)
	{
		m_pLogModule->Log(nll, func, line, " {} | {} | {} |", ident, strInfo, strDesc);
	}
	return false;
}

NFILogModule* NFLogMgr::GetLogModule()
{
	return m_pLogModule;
}

