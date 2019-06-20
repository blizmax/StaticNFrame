// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCMysqlModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"

class NFCMysqlModule final
	: public NFIMysqlModule, public NFTimerObj
{
public:
	NFCMysqlModule(NFIPluginManager* p);
	virtual ~NFCMysqlModule();


	bool Init() override;
	bool Shut() override;
	bool Execute() override;
	void OnTimer(uint32_t nTimerID) override;
	bool AfterInit() override;

	//////////////////////////////////////////////////////////////////////////
	/**
	 * @brief ������������
	 *
	 * @param  message			protobuf��Ϣ
	 * @return bool				�ɹ���ʧ��
	 */
	bool UpdateOne(const google::protobuf::Message& message) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @return bool				�ɹ���ʧ��
	 */
	bool QueryOne(google::protobuf::Message& message) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  message			protobuf��Ϣ�� ��Ҫȡ��������protobuf��������ȡ������������ֵ
	 * @return bool				�ɹ���ʧ��
	 */
	bool QueryMore(google::protobuf::Message& message) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  qstr			ִ��sql���
	 * @return bool			�ɹ���ʧ��
	 */
	bool Execute(const std::string& qstr, std::vector<std::map<std::string, std::string>>& keyvalueMap) override;

	/**
	 * @brief ���»��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	����
	 * @param  strKey			��ֵ
	 * @param  keyvalueMap		Ҫȡ�������ݵ�����
	 * @return bool				�ɹ���ʧ��
	 */
	bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::map<std::string, std::string>& keyvalueMap) override;

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
	bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	              const std::vector<std::string>& fieldVec, std::map<std::string, string>& valueVec) override;

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
	bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::vector<std::string>& fieldVec,
	               std::vector<std::map<std::string, std::string>>& valueVec) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		�������
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			����
	 * @return bool				�ɹ���ʧ��
	 */
	bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                       const std::vector<std::string>& fieldVec,
	                       std::vector<std::map<std::string, std::string>>& valueVec) override;

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
	bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows,
	                        const std::vector<std::string>& fieldVec,
	                        std::vector<std::map<std::string, std::string>>& valueVec) override;

	/**
	 * @brief ɾ������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	�������������������
	 * @param  strKey			����
	 * @return bool				�ɹ���ʧ��
	 */
	bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) override;

	/**
	 * @brief ��������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	�������������������
	 * @param  strKey			����
	 * @param  bExit			�Ƿ����
	 * @return bool				�ɹ���ʧ��
	 */
	bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	            bool& bExit) override;

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
	bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                     const std::vector<std::string>& fieldVec,
	                     std::vector<std::map<std::string, std::string>>& valueVec) override;

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
private:
	NFIMysqlDriverManager* m_pMysqlDriverManager;

	uint64_t mnLastCheckTime;
};
