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
		std::string str = fmt::format(std::string("[{}:{}] | [{}:{}] |") + my_fmt, function, line, logId, guid, args...);
		LogDefault(log_level, logId, guid, str);
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
	virtual void LogDefault(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log) = 0;

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
	virtual void LogOthers(uint32_t logNameId, NF_LOG_LEVEL log_level, const std::string& log) = 0;

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

