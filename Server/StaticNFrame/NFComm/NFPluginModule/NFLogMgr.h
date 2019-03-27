// -------------------------------------------------------------------------
//    @FileName         :    NFLogMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFCore/NFPlatform.h"
#include "common/spdlog/fmt/fmt.h"
#include "NFILogModule.h"

/**
* @brief �������ֲ�ܹ�������ֻ����дϵͳ�����LOG
*
*/
enum NF_LOG_ID
{
	NF_LOG_DEFAULT = 0,						//Ĭ��LOG
	NF_LOG_SYSTEMLOG = 1,					//ϵͳLOG
	NF_LOG_ACTOR_PLUGIN = 2,				//Actor ���� 
	NF_LOG_KERNEL_PLUGIN = 3,				//kernel ����
	NF_LOG_LUA_PLUGIN = 4,					//lua ����
	NF_LOG_MONGO_PLUGIN = 5,				//mongo ����
	NF_LOG_MONITOR_PLUGIN = 5,				//monitor ����
	NF_LOG_MYSQL_PLUGIN = 7,				//mysql ����
	NF_LOG_NET_PLUGIN = 8,					//net ����
	NF_LOG_SQLITE_PLUGIN = 9,				//sqlite ����
	NF_LOG_TEST_PLUGIN = 10,				//test ����
	NF_LOG_PLUGIN_MANAGER = 100,				//���������
	NF_LOG_MAX_SYSTEM_PLUGIN = 100,
	NF_LOG_MAX_ID = 10240, //���LOGID
};

class _NFExport NFLogMgr : public NFSingleton<NFLogMgr>
{
public:
	NFLogMgr();
	virtual ~NFLogMgr();
public:
	bool Init(NFILogModule* pSpdlogModule = nullptr);
	void UnInit();
public:
	/**
	* @brief ����ӿ����Ĭ�ϵ�LOG
	*
	* @param  log_level log�ȼ�
	* @param  function log���ں���
	* @param  line log�����ļ���
	* @return bool
	*/
	template<typename... ARGS>
	void Log(uint32_t log_level, const char* function, int line, uint32_t logId, uint64_t guid, const char* my_fmt, const ARGS& ... args)
	{
		if (m_pLogModule)
		{
			std::string str = fmt::format(std::string("[{}:{}] | [{}:{}] |") + my_fmt, function, line, logId, guid, args...);
			LogDefault(log_level, logId, guid, str);
		}
	}

	/**
	* @brief ����ӿ����Ĭ�ϵ�LOG
	*
	* @param  log_level log�ȼ�
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	void LogDefault(uint32_t log_level, uint32_t logId, uint64_t guid, const std::string& log);

	/**
	* @brief ����Ĭ�ϵ�LOG������ȼ�
	*
	* @param  log_level log�ȼ�
	* @return bool
	*/
	void SetDefaultLevel(NF_LOG_LEVEL log_level);

	/**
	* @brief ����Ĭ�ϵ�LOG��ˢ�µȼ�
	*
	* @param  log_level log�ȼ�
	* @return bool
	*/
	void SetDefaultFlush(NF_LOG_LEVEL log_level);
protected:
	NFILogModule* m_pLogModule;
};

#define NFLogDebug(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_DEBUG_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogInfo(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_INFO_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogWarning(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_WARING_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);
#define NFLogError(logID, guid, format, ...) NFLogMgr::Instance()->Log(NLL_ERROR_NORMAL, NF_FUNCTION_LINE, logID, guid, format, ##__VA_ARGS__);

