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

//异步mysql UpdateCallBack 回调函数
using ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR = std::function<void(bool result)>;

//异步mysql UpdateMessageCallBack 回调函数
using ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR = std::function<void(bool result)>;

//异步mysql QueryOneCallBack 回调函数
using ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                 const std::map<std::string, std::string>&
                                                                 fieldValueMap)>;

//异步mysql QueryMoreCallBack 回调函数
using ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                  const std::vector<std::map<std::string, std::string>>&
                                                                  fieldValueMap)>;

//异步mysql QueryMoreWithCondCallBack 回调函数
using ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR = std::function<void(
	bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap)>;

//异步mysql QueryMoreWithLimitCallBack 回调函数
using ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR = std::function<void(
	bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap)>;

//异步mysql QueryMoreByLikeCallBack 回调函数
using ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR = std::function<void(
	bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap)>;

//异步mysql QueryOneMessageCallBack 回调函数
using ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR = std::function<void(
	bool result, const google::protobuf::Message& message)>;

//异步mysql QueryMoreMessageCallBack 回调函数
using ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR = std::function<void(
	bool result, const google::protobuf::Message& message)>;

//异步mysql ExecuteMoreCallBack 回调函数
using ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR = std::function<void(bool result,
                                                                    const std::vector<std::map<std::string, std::string>
                                                                    >& fieldValueMap)>;

//异步mysql ExecuteOneCallBack 回调函数
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
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
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddUpdateCallBack(const uint32_t nMsgID,
	                               const NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddUpdateMessageCallBack(const uint32_t nMsgID,
	                                      const NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR>&
	                                      callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryOneCallBack(const uint32_t nMsgID,
	                                 const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryMoreCallBack(const uint32_t nMsgID,
	                                  const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryMoreWithCondCallBack(const uint32_t nMsgID,
	                                          const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR>
	                                          & callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryMoreWithLimitCallBack(const uint32_t nMsgID,
	                                           const NFAsyMysqlCallBack<
		                                           ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryMoreByLikeCallBack(const uint32_t nMsgID,
	                                        const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR>&
	                                        callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryOneMessageCallBack(const uint32_t nMsgID,
	                                        const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR>&
	                                        callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddQueryMoreMessageCallBack(const uint32_t nMsgID,
	                                         const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR>&
	                                         callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddExecuteOneCallBack(const uint32_t nMsgID,
	                                   const NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_ONE_CALLBACK_FUNCTOR>& callBack)
	{
	}

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	virtual void AddExecuteMoreCallBack(const uint32_t nMsgID,
	                                    const NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR>& callBack)
	{
	}

public:
	/**
	 * @brief 添加Mysql链接
	 *
	 * @param  nServerID			ID
	 * @param  strIP				IP地址
	 * @param  nPort				端口
	 * @param  strDBName			数据库名字
	 * @param  strDBUser			数据库用户名
	 * @param  strDBPwd				数据库密码
	 * @param  nRconnectTime		重连间隔
	 * @param  nRconneCount			重连次数
	 * @return bool					成功或失败
	 */
	virtual bool AddMysqlServer(int nServerID, const std::string& strIP, int nPort, std::string strDBName,
	                            std::string strDBUser, std::string strDBPwd, int nRconnectTime = 10,
	                            int nRconneCount = -1) = 0;

	//////////////////////////////////////////////////////////////////////////
	/**
	 * @brief 插入或更新数据
	 *
	 * @param  message			protobuf信息
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool UpdateOne(const google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryOne(google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMore(google::protobuf::Message& message, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询一条数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldValueMap	要保存的数据项
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询一条数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                      const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		查询条件
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                               const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，将以这个列排序
	 * @param  nOffset			距离第一行的偏移
	 * @param  nRows			取出多少列
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
	                                int nRows, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询类似数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKeyName		列值
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                             const std::string& strKeyName, std::vector<std::string>& fieldVec,
	                             uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  qstr			执行sql语句
	 * @param balanceId     平衡ID
	 * @return bool			成功或失败
	 */
	virtual bool ExecuteMore(const std::string& qstr, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  qstr			执行sql语句
	 * @param balanceId     平衡ID
	 * @return bool			成功或失败
	 */
	virtual bool ExecuteOne(const std::string& qstr, uint64_t balanceId = 0) = 0;
};
