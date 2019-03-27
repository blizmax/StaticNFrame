// -------------------------------------------------------------------------
//    @FileName         :    NFCLogModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFCore/NFPlatform.h"

#include "common/spdlog/spdlog.h"
#include "common/spdlog/fmt/fmt.h"

#include <unordered_map>
#include <unordered_set>

class NFCLogModule : public NFILogModule
{
public:
	NFCLogModule(NFIPluginManager* p);

	virtual ~NFCLogModule();

	virtual bool Awake() override;
	virtual bool Shut() override;

	/**
	* @brief ����ӿ����Ĭ�ϵ�LOG
	*
	* @param  log_level log�ȼ�
	* @param  logId LOGѡ��ID�������������
	* @param  guid һ�������ID��ĳЩ����£�ֻ�����һ����ҵ�LOG
	* @param  log
	* @return bool
	*/
	virtual void LogDefault(NF_LOG_LEVEL log_level, uint32_t logId, uint64_t guid, const std::string& log);

	/**
	* @brief ����Ĭ�ϵ�LOG������ȼ�
	*
	* @param  log_level log�ȼ�
	* @return bool
	*/
	virtual void SetDefaultLevel(NF_LOG_LEVEL log_level);

	/**
	* @brief ����Ĭ�ϵ�LOG��ˢ�µȼ�
	*
	* @param  log_level log�ȼ�
	* @return bool
	*/
	virtual void SetDefaultFlush(NF_LOG_LEVEL log_level);

	/**
	* @brief �������LOGϵͳ
	*
	* @param  logNameId log�ļ�Ψһ��ID
	* @param  logName log�ļ���
	* @param  async �Ƿ��첽�����߳�
	* @return bool
	*/
	virtual void CreateOthersLogger(uint32_t logNameId, const std::string& logName, bool async);

	/**
	* @brief ͨ��logNameId�����LOG
	*
	* @param  logNameId log�ļ�Ψһ��ID
	* @param  log_level log�ȼ�
	* @param  log
	* @return bool
	*/
	virtual void LogOthers(uint32_t logNameId, NF_LOG_LEVEL log_level, const std::string& log);
protected:
	/*����Ĭ��ϵͳLOGϵͳ*/
	void CreateDefaultLogger();
private:
	std::shared_ptr<spdlog::logger> m_defaultLogger;
	std::unordered_map<uint32_t, std::shared_ptr<spdlog::logger>> m_loggerMap;
	std::unordered_set<std::string> m_loggerName;
};


