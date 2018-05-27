// -------------------------------------------------------------------------
//    @FileName         :    NFLogMgr.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFLogMgr.h"

NFLogMgr::NFLogMgr()
{
	m_pLogModule = nullptr;
}

NFLogMgr::~NFLogMgr()
{
}

bool NFLogMgr::Init(NFILogModule* p)
{
	m_pLogModule = p;
	return true;
}

void NFLogMgr::UnInit()
{
	m_pLogModule = nullptr;
}

bool NFLogMgr::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const int64_t nDesc, const char* func, int line)
{
	if (m_pLogModule)
	{
		return m_pLogModule->LogNormal(nll, ident, strInfo, nDesc, func, line);
	}
	return false;
}

bool NFLogMgr::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func, int line)
{
	if (m_pLogModule)
	{
		return m_pLogModule->LogNormal(nll, ident, strInfo, strDesc, func, line);
	}
	return false;
}

bool NFLogMgr::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::ostringstream& stream, const char* func, int line)
{
	if (m_pLogModule)
	{
		return m_pLogModule->LogNormal(nll, ident, stream, func, line);
	}
	return false;
}

NFILogModule* NFLogMgr::GetLogModule()
{
	return m_pLogModule;
}

