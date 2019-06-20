// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlModule.h
//    @Author           :    Gao.Yi
//    @Date             :    2012-12-15
//    @Module           :    NFIMysqlModule
//
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

class NFIMysqlModule
	: public NFIModule
{
public:
	/**
	 * @brief ���Mysql����
	 *
	 * @param  nServerID			ID
	 * @param  strIP				IP��ַ
	 * @param  nPort				�˿�
	 * @param  strDBName			���ݿ�����
	 * @param  strDBUser			���ݿ��û���
	 * @param  strDBPwd				���ݿ�����
	 * @param  nRconnectTime		�������
	 * @param  nRconneCount			��������
	 * @return bool					�ɹ���ʧ��
	 */
	virtual bool AddMysqlServer(int nServerID, const std::string& strIP, int nPort, std::string strDBName,
	                            std::string strDBUser, std::string strDBPwd, int nRconnectTime = 10,
	                            int nRconneCount = -1) = 0;

	/**
	 * @brief ������������
	 *
	 * @param  message			protobuf��Ϣ
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool UpdateOne(const google::protobuf::Message& message) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryOne(google::protobuf::Message& message) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMore(google::protobuf::Message& message) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  qstr			ִ��sql���
	 * @param keyvalueMap
	 * @return bool			�ɹ���ʧ��
	 */
	virtual bool Execute(const std::string& qstr, std::vector<std::map<std::string, std::string>>& keyvalueMap) = 0;

	/**
	 * @brief ���»��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  keyvalueMap		Ҫȡ�������ݵ�����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::map<std::string, std::string>& keyvalueMap) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                      const std::vector<std::string>& fieldVec, std::map<std::string, std::string>& valueVec) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	�������������������
	 * @param  nOffset			�����һ�е�ƫ��
	 * @param  nRows			ȡ��������
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
	                                int nRows, const std::vector<std::string>& fieldVec,
	                                std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		�������
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                               const std::vector<std::string>& fieldVec,
	                               std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::vector<std::string>& fieldVec,
	                       std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ɾ������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	�������������������
	 * @param  strKey			����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName,
	                    const std::string& strKey) = 0;

	/**
	 * @brief ��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	�������������������
	 * @param  strKey			����
	 * @param  bExit			�Ƿ����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                    bool& bExit) = 0;

	/**
	 * @brief ��ѯ��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                             const std::string& strKey, const std::vector<std::string>& fieldVec,
	                             std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ���»��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  keyvalueMap		����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool LuaUpdateOne(const std::string& strTableName, const std::string& strKeyColName,
	                          const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap)
	{
		return UpdateOne(strTableName, strKeyColName, strKey, keyvalueMap);
	}

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec		����
	 * @return std::map<std::string, std::string>	��ѯ����
	 */
	virtual std::map<std::string, std::string> LuaQueryOne(const std::string& strTableName,
	                                                       const std::string& strKeyColName, const std::string& strKey,
	                                                       const std::vector<std::string>& fieldVec)
	{
		std::map<std::string, std::string> valueVec;
		if (!QueryOne(strTableName, strKeyColName, strKey, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaQueryOne error!");
		}
		return valueVec;
	}

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec		����
	 * @return std::map<std::string, std::string>	��ѯ����
	 */
	virtual std::vector<std::map<std::string, std::string>> LuaQueryMore(
		const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
		const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMore(strTableName, strKeyColName, strKey, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaQueryMore error!");
		}
		return valueVec;
	}

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����������һ������
	 * @param  nOffset			����ƫ��
	 * @param  nRows			������Ŀ
	 * @param  fieldVec			����
	 * @return std::map<std::string, std::string>	��ѯ����
	 */
	virtual std::vector<std::map<std::string, std::string>> LuaQueryMoreWithLimit(
		const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows,
		const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMoreWithLimit(strTableName, strKeyColName, nOffset, nRows, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "QueryMoreWithLimit error!");
		}
		return valueVec;
	}

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		�������
	 * @param  fieldVec			������
	 * @return std::vector<std::map<std::string, std::string>>	��ѯ����
	 */
	virtual std::vector<std::map<std::string, std::string>> LuaQueryMoreWithCond(
		const std::string& strTableName, const std::string& strWhereSql, const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMoreWithCond(strTableName, strWhereSql, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "QueryMoreWithLimit error!");
		}
		return valueVec;
	}

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @return bool				���
	 */
	virtual bool LuaExists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey)
	{
		bool bExit = false;
		if (!Exists(strTableName, strKeyColName, strKey, bExit))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaExists error!");
		}
		return bExit;
	}

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec		����
	 * @return std::vector<std::map<std::string, std::string>>	��ѯ��������
	 */
	virtual std::vector<std::map<std::string, std::string>> LuaQueryMoreByLike(
		const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
		const std::vector<std::string>& fieldVec)
	{
		std::vector<std::map<std::string, std::string>> valueVec;
		if (!QueryMoreByLike(strTableName, strKeyColName, strKey, fieldVec, valueVec))
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "LuaQueryMoreByLike error!");
		}
		return valueVec;
	}
};
