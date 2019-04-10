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
#include "protobuf/src/google/protobuf/stubs/logging.h"

/*
** 将一些protobuf的错误输出打印到日志里,方便查错
*/
void ProtobufLogHandler(google::protobuf::LogLevel level, const char* filename, int line,
	const std::string& message)
{
	NFLogError(NF_LOG_SYSTEMLOG, 0, "[{}:{}]:{}", filename, line, message);
}

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
	google::protobuf::SetLogHandler(ProtobufLogHandler);
	return true;
}

void NFLogMgr::UnInit()
{
	google::protobuf::SetLogHandler(nullptr);
	m_pLogModule = nullptr;
}

/**
* @brief 设置默认的LOG的输出等级
*
* @param  log_level log等级
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
* @brief 设置默认的LOG的刷新等级
*
* @param  log_level log等级
* @return bool
*/
void NFLogMgr::SetDefaultFlush(NF_LOG_LEVEL log_level)
{
	if (m_pLogModule)
	{
		m_pLogModule->SetDefaultFlush(log_level);
	}
}

