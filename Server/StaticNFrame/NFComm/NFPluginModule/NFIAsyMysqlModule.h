// -------------------------------------------------------------------------
//    @FileName         :    NFIAsyMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAsyMysqlModule
//
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIAsycModule.h"
#include "google/protobuf/message.h"
#include <vector>
#include <functional>

//�첽mysql�ص�����
typedef std::function<void(bool result)> ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR;

class NFIAsyMysqlModule
	: public NFIAsycModule
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
	virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

	//////////////////////////////////////////////////////////////////////////
	/**
	 * @brief ������������
	 *
	 * @param  message			protobuf��Ϣ
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool UpdateOne(const google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryOne(google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMore(google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯһ������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldValueMap	Ҫ�����������
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯһ������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		��ѯ����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;
	
	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	�������������������
	 * @param  nOffset			�����һ�е�ƫ��
	 * @param  nRows			ȡ��������
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKeyName		��ֵ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  qstr			ִ��sql���
	 * @param balanceId     ƽ��ID
	 * @return bool			�ɹ���ʧ��
	 */
	virtual bool ExecuteMore(const std::string& qstr, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  qstr			ִ��sql���
	 * @param balanceId     ƽ��ID
	 * @return bool			�ɹ���ʧ��
	 */
	virtual bool ExecuteOne(const std::string& qstr, uint64_t balanceId = 0) = 0;
};

