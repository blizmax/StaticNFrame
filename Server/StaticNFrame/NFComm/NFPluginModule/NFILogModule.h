// -------------------------------------------------------------------------
//    @FileName         :    NFILogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOG_MODULE_H
#define NFI_LOG_MODULE_H

#include "NFIModule.h"
#include "common/spdlog/fmt/fmt.h"

class LogInfoConfig;

enum NF_LOG_LEVEL
{
	NLL_TRACE_NORMAL = 0,
	NLL_DEBUG_NORMAL = 1,
	NLL_INFO_NORMAL = 2,
	NLL_WARING_NORMAL = 3,
	NLL_ERROR_NORMAL = 4,
	NLL_CRITICAL_NORMAL = 5,
	NLL_OFF_NORMAL = 6,
};

/**
* @brief �������ֲ�ܹ�������ֻ����дϵͳ�����LOG
*
*/
enum NF_LOG_ID
{
	//0-100�ǻ�����ܲ�LOG
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

	//-----------------------------------------------------------
	NF_LOG_LOAD_CONFIG = 21,				//��������
	NF_LOG_PROTOBUF_PARSE = 22,				//protobuf��������
	NF_LOG_RECV_MSG = 23,					//������Ϣ
	NF_LOG_RECV_MSG_JSON_PRINTF = 24,		//������ϢJson���
	NF_LOG_PLUGIN_MANAGER = 100,			//���������
	NF_LOG_MAX_SYSTEM_PLUGIN = 100,
	//-----------------------------------------------------------
	NF_LOG_BEGIN_SERVER_LOG = 101,			//101-1000��ϵͳ��ܲ�LOG
	NF_LOG_END_SERVER_LOG = 1000,			//
	//-----------------------------------------------------------
	NF_LOG_BEGIN_LOGIC_LOG = 1001,			//1001�Ժ����߼���ܲ�

	NF_LOG_BEHAVIOR_LOGIC_LOG = 1002,		//��ΪLOG��־

	NF_LOG_MAX_ID = 10240, //���LOGID
};

class NFILogModule
	: public NFIModule
{
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
	void Log(NF_LOG_LEVEL log_level, const char* function, int line, uint32_t logId, uint64_t guid, const char* my_fmt, const ARGS& ... args)
	{
		std::string str = fmt::format(my_fmt, args...);
		LogDefault(log_level, function, line, logId, guid, str);
	}

	/**
	* @brief ����ӿ����Ĭ�ϵ�LOG
	*
	* @param  log_level log�ȼ�
	* @param  function
	* @param  line
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LogDefault(NF_LOG_LEVEL log_level, const char* function, int line, uint32_t logId, uint64_t guid, const std::string& log) = 0;

	/**
	* @brief ����ӿ����Ĭ�ϵ�LOG
	*
	* @param  log_level log�ȼ�
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LogDefault(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log) = 0;

	/**
	* @brief �Ƿ������Ӧ�ȼ���logId��log
	*
	* @param  log_level log�ȼ�
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @return bool
	*/
	virtual bool IsLogIdEnable(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid) = 0;

	/**
	* @brief ����Ĭ�ϵ�LOG������ȼ�
	*
	* @param  log_level log�ȼ�
	* @return bool
	*/
	virtual void SetDefaultLevel(NF_LOG_LEVEL log_level) = 0;

	/**
	* @brief ����Ĭ�ϵ�LOG��ˢ�µȼ�
	*
	* @param  log_level log�ȼ�
	* @return bool
	*/
	virtual void SetDefaultFlush(NF_LOG_LEVEL log_level) = 0;

	/**
	* @brief ����log������
	*
	* @param  vecLogConfig
	* @return
	*/
	virtual void SetDefaultLogConfig(const std::vector<LogInfoConfig>& vecLogConfig) = 0;

	/**
	* @brief �������LOGϵͳ
	*
	* @param  logNameId log�ļ�Ψһ��ID
	* @param  logName log�ļ���
	* @param  async �Ƿ��첽�����߳�
	* @return bool
	*/
	virtual void CreateOthersLogger(uint32_t logNameId, const std::string& logName, bool async) = 0;
	
	/**
	* @brief ͨ��logNameId�����LOG
	*
	* @param  logNameId log�ļ�Ψһ��ID
	* @param  log_level log�ȼ�
	* @param  log
	* @return bool
	*/
	virtual void LogOthers(uint32_t logNameId, NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log) = 0;

	/**
	* @brief ����LUAϵͳ��LOG
	*
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaDebug(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_DEBUG_NORMAL, logId, guid, strInfo);
	}

	/**
	* @brief ����LUAϵͳ��LOG
	*
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaInfo(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_INFO_NORMAL, logId, guid, strInfo);
	}

	/**
	* @brief ����LUAϵͳ��LOG
	*
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaWarn(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_WARING_NORMAL, logId, guid, strInfo);
	}

	/**
	* @brief ����LUAϵͳ��LOG
	*
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LuaError(uint32_t logId, uint64_t guid, const std::string& str)
	{
		std::string strInfo = fmt::format("[Lua] | {}", str);
		LogDefault(NLL_ERROR_NORMAL, logId, guid, strInfo);
	}
};

#endif

