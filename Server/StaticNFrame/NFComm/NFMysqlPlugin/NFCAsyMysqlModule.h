// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlModule.h
//    @Author           :    Gao.Yi
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFCMysqlDriverManager.h"
#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"


/**
 * @brief �첽mysql
 */
class NFCAsyMysqlModule final
	: public NFIAsyMysqlModule
{
public:
	NFCAsyMysqlModule(NFIPluginManager* p);
	virtual ~NFCAsyMysqlModule();

	bool Awake() override;

	bool Init() override;

	bool AfterInit() override;

	bool Execute() override;

	bool BeforeShut() override;

	bool Shut() override;

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
	bool AddMysqlServer(int nServerID, const std::string& strIP, int nPort, std::string strDBName,
	                    std::string strDBUser, std::string strDBPwd, int nRconnectTime = 10,
	                    int nRconneCount = -1) override;
public:
	//////////////////////////////////////////////////////////////////////////
	/**
	 * @brief ������������
	 *
	 * @param  message			protobuf��Ϣ
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	bool UpdateOne(const google::protobuf::Message& message, uint64_t balanceId = 0) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	bool QueryOne(google::protobuf::Message& message, uint64_t balanceId = 0) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	bool QueryMore(google::protobuf::Message& message, uint64_t balanceId = 0) override;

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
	bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) override;

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
	bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	              const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		��ѯ����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                       const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows,
	                        const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                     const std::string& strKeyName, std::vector<std::string>& fieldVec,
	                     uint64_t balanceId = 0) override;

public:
	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @return void
	 */
	void UpdateCallBack(bool result);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @return void
	 */
	void UpdateMessageCallBack(bool result);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  fieldValueMap ���ص�����
	 * @return void
	 */
	void QueryOneCallBack(bool result, const std::map<std::string, std::string>& fieldValueMap);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  fieldValueMap ���ص�����
	 * @return void
	 */
	void QueryMoreCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  fieldValueMap ���ص�����
	 * @return void
	 */
	void QueryMoreWithCondCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  fieldValueMap ���ص�����
	 * @return void
	 */
	void QueryMoreWithLimitCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  fieldValueMap ���ص�����
	 * @return void
	 */
	void QueryMoreByLikeCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  message ���ص�����
	 * @return void
	 */
	void QueryOneMessageCallBack(bool result, const google::protobuf::Message& message);

	/**
	 * @brief ���»�������ݻص�
	 *
	 * @param  result		�ɹ���ʧ��
	 * @param  message ���ص�����
	 * @return void
	 */
	void QueryMoreMessageCallBack(bool result, const google::protobuf::Message& message);
private:
	uint64_t mnLastCheckTime;
};
