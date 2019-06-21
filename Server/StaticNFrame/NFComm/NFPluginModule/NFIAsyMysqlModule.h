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

//�첽mysql UpdateCallBack �ص�����
using ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR = std::function<void(bool result)>;

//�첽mysql UpdateMessageCallBack �ص�����
using ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR = std::function<void(bool result)>;

//�첽mysql QueryOneCallBack �ص�����
using ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                 const std::map<std::string, std::string>&
                                                                 fieldValueMap)>;

//�첽mysql QueryMoreCallBack �ص�����
using ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                  const std::vector<std::map<std::string, std::string>>&
                                                                  fieldValueMap)>;

//�첽mysql QueryMoreWithCondCallBack �ص�����
using ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR = std::function<void(
	bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap)>;

//�첽mysql QueryMoreWithLimitCallBack �ص�����
using ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR = std::function<void(
	bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap)>;

//�첽mysql QueryMoreByLikeCallBack �ص�����
using ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR = std::function<void(
	bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap)>;

//�첽mysql QueryOneMessageCallBack �ص�����
using ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR = std::function<void(
	bool result, const google::protobuf::Message& message)>;

//�첽mysql QueryMoreMessageCallBack �ص�����
using ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR = std::function<void(
	bool result, const google::protobuf::Message& message)>;

//�첽mysql ExecuteMoreCallBack �ص�����
using ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                    const std::vector<std::map<std::string, std::string>
                                                                    >& fieldValueMap)>;

//�첽mysql ExecuteOneCallBack �ص�����
using ASYC_MYSQL_EXECUTE_ONE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                   const std::map<std::string, std::string>&
                                                                   fieldValueMap)>;

class NFIAsyMysqlModule
	: public NFIAsycModule
{
public:
	template <typename CallBack>
	class NFAsyMysqlCallBack
	{
	public:
		NFAsyMysqlCallBack()
		{
			mModulePtr = nullptr;
		}

		virtual ~NFAsyMysqlCallBack()
		{
		}

	public:
		CallBack mFunctor;
		void* mModulePtr;
	};

public:
	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddUpdateCallBack(const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(bool result))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		callBack.mModulePtr = pBase;

		AddUpdateCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddUpdateMessageCallBack(const uint32_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(bool result))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1);
		callBack.mModulePtr = pBase;

		AddUpdateMessageCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryOneCallBack(const uint32_t nMsgID, BaseType* pBase,
	                         void (BaseType::*handleRecieve)(bool result,
	                                                         const std::map<std::string, std::string>& fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryOneCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryMoreCallBack(const uint32_t nMsgID, BaseType* pBase,
	                          void (BaseType::*handleRecieve)(bool result,
	                                                          const std::vector<std::map<std::string, std::string>>&
	                                                          fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryMoreCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryMoreWithCondCallBack(const uint32_t nMsgID, BaseType* pBase,
	                                  void (BaseType::*handleRecieve)(bool result,
	                                                                  const std::vector<std::map<
		                                                                  std::string, std::string>>& fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryMoreWithCondCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryMoreWithLimitCallBack(const uint32_t nMsgID, BaseType* pBase,
	                                   void (BaseType::*handleRecieve)(bool result,
	                                                                   const std::vector<std::map<
		                                                                   std::string, std::string>>& fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryMoreWithLimitCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryMoreByLikeCallBack(const uint32_t nMsgID, BaseType* pBase,
	                                void (BaseType::*handleRecieve)(bool result,
	                                                                const std::vector<std::map<std::string, std::string>
	                                                                >& fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryMoreByLikeCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryOneMessageCallBack(const uint32_t nMsgID, BaseType* pBase,
	                                void (BaseType::*handleRecieve)(bool result,
	                                                                const google::protobuf::Message& message))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryOneMessageCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddQueryMoreMessageCallBack(const uint32_t nMsgID, BaseType* pBase,
	                                 void (BaseType::*handleRecieve)(bool result,
	                                                                 const google::protobuf::Message& message))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddQueryMoreMessageCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddExecuteOneCallBack(const uint32_t nMsgID, BaseType* pBase,
	                           void (BaseType::*handleRecieve)(bool result,
	                                                           const std::map<std::string, std::string>& fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_ONE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddExecuteOneCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @return void
	 */
	template <typename BaseType>
	void AddExecuteMoreCallBack(const uint32_t nMsgID, BaseType* pBase,
	                            void (BaseType::*handleRecieve)(bool result,
	                                                            const std::vector<std::map<std::string, std::string>>&
	                                                            fieldValueMap))
	{
		NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR> callBack;
		callBack.mFunctor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2);
		callBack.mModulePtr = pBase;

		AddExecuteMoreCallBack(nMsgID, callBack);
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddUpdateCallBack(const uint32_t nMsgID,
	                               const NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddUpdateMessageCallBack(const uint32_t nMsgID,
	                                      const NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR>&
	                                      callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryOneCallBack(const uint32_t nMsgID,
	                                 const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryMoreCallBack(const uint32_t nMsgID,
	                                  const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryMoreWithCondCallBack(const uint32_t nMsgID,
	                                          const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR>
	                                          & callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryMoreWithLimitCallBack(const uint32_t nMsgID,
	                                           const NFAsyMysqlCallBack<
		                                           ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryMoreByLikeCallBack(const uint32_t nMsgID,
	                                        const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR>&
	                                        callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryOneMessageCallBack(const uint32_t nMsgID,
	                                        const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR>&
	                                        callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddQueryMoreMessageCallBack(const uint32_t nMsgID,
	                                         const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR>&
	                                         callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddExecuteOneCallBack(const uint32_t nMsgID,
	                                   const NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_ONE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief ��ӻص�
	 *
	 * @param  nMsgID		�ص���ϢID
	 * @param callBack      �ص�
	 * @return void
	 */
	virtual void AddExecuteMoreCallBack(const uint32_t nMsgID,
	                                    const NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR>& callBack)
	{
	}

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
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                      const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		��ѯ����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  balanceId		ƽ��ID
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                               const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
	                                int nRows, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                             const std::string& strKeyName, std::vector<std::string>& fieldVec,
	                             uint64_t balanceId = 0) = 0;

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
