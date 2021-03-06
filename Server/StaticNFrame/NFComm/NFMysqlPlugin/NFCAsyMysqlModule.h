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
 * @brief 异步mysql
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
	bool AddMysqlServer(int nServerID, const std::string& strIP, int nPort, std::string strDBName,
	                    std::string strDBUser, std::string strDBPwd, int nRconnectTime = 10,
	                    int nRconneCount = -1) override;
public:
	//////////////////////////////////////////////////////////////////////////
	/**
	 * @brief 插入或更新数据
	 *
	 * @param  message			protobuf信息
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	bool UpdateOne(const google::protobuf::Message& message, uint64_t balanceId = 0) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	bool QueryOne(google::protobuf::Message& message, uint64_t balanceId = 0) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	bool QueryMore(google::protobuf::Message& message, uint64_t balanceId = 0) override;

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
	bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) override;

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
	bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	              const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		查询条件
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                       const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows,
	                        const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) override;

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
	bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                     const std::string& strKeyName, std::vector<std::string>& fieldVec,
	                     uint64_t balanceId = 0) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  qstr			执行sql语句
	 * @param balanceId     平衡ID
	 * @return bool			成功或失败
	 */
	bool ExecuteMore(const std::string& qstr, uint64_t balanceId = 0) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  qstr			执行sql语句
	 * @param balanceId     平衡ID
	 * @return bool			成功或失败
	 */
	bool ExecuteOne(const std::string& qstr, uint64_t balanceId = 0) override;
public:
	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void			
	 */
	void AddUpdateCallBack(uint32_t nMsgID,
	                       const NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR>& callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddUpdateMessageCallBack(uint32_t nMsgID,
	                              const NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR>&
	                              callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryOneCallBack(uint32_t nMsgID,
	                         const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR>& callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryMoreCallBack(uint32_t nMsgID,
	                          const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR>& callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryMoreWithCondCallBack(uint32_t nMsgID,
	                                  const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR>
	                                  & callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryMoreWithLimitCallBack(uint32_t nMsgID,
	                                   const NFAsyMysqlCallBack<
		                                   ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR>& callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryMoreByLikeCallBack(uint32_t nMsgID,
	                                const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR>&
	                                callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryOneMessageCallBack(uint32_t nMsgID,
	                                const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR>&
	                                callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddQueryMoreMessageCallBack(uint32_t nMsgID,
	                                 const NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR>&
	                                 callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddExecuteOneCallBack(uint32_t nMsgID,
	                           const NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_ONE_CALLBACK_FUNCTOR>& callBack) override;

	/**
	 * @brief 添加回调
	 *
	 * @param  nMsgID		回调消息ID
	 * @param callBack      回调
	 * @return void
	 */
	void AddExecuteMoreCallBack(uint32_t nMsgID,
	                            const NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR>& callBack) override;
public:
	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @return void
	 */
	void UpdateCallBack(bool result);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @return void
	 */
	void UpdateMessageCallBack(bool result);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  fieldValueMap 返回的数据
	 * @return void
	 */
	void QueryOneCallBack(bool result, const std::map<std::string, std::string>& fieldValueMap);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  fieldValueMap 返回的数据
	 * @return void
	 */
	void QueryMoreCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  fieldValueMap 返回的数据
	 * @return void
	 */
	void QueryMoreWithCondCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  fieldValueMap 返回的数据
	 * @return void
	 */
	void QueryMoreWithLimitCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  fieldValueMap 返回的数据
	 * @return void
	 */
	void QueryMoreByLikeCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  message 返回的数据
	 * @return void
	 */
	void QueryOneMessageCallBack(bool result, const google::protobuf::Message& message);

	/**
	 * @brief 更新或插入数据回调
	 *
	 * @param  result		成功或失败
	 * @param  message 返回的数据
	 * @return void
	 */
	void QueryMoreMessageCallBack(bool result, const google::protobuf::Message& message);

	/**
	 * @brief 查询数据
	 *
	 * @param  result			成功或失败
	 * @param fieldValueMap     返回的数据
	 * @return bool			成功或失败
	 */
	void ExecuteMoreCallBack(bool result, const std::vector<std::map<std::string, std::string>>& fieldValueMap);

	/**
	 * @brief 查询数据
	 *
	 * @param  result			成功或失败
	 * @param  fieldValueMap     返回的数据
	 * @return bool			成功或失败
	 */
	void ExecuteOneCallBack(bool result, const std::map<std::string, std::string>& fieldValueMap);
private:
	uint64_t mnLastCheckTime;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR>> m_updateCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_UPDATE_MESSAGE_CALLBACK_FUNCTOR>>
	m_updateMessageCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_CALLBACK_FUNCTOR>> m_queryOneCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_CALLBACK_FUNCTOR>> m_queryMoreCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_COND_CALLBACK_FUNCTOR>>
	m_queryMoreWithCondCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_WITH_LIMIT_CALLBACK_FUNCTOR>>
	m_queryMoreWithLimitCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_BY_LIKE_CALLBACK_FUNCTOR>>
	m_queryMoreByLikeCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_ONE_MESSAGE_CALLBACK_FUNCTOR>>
	m_queryOneMessageCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_QUERY_MORE_MESSAGE_CALLBACK_FUNCTOR>>
	m_queryMoreMessageCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_MORE_CALLBACK_FUNCTOR>> m_executeMoreCallBackMap;
	std::unordered_map<uint32_t, NFAsyMysqlCallBack<ASYC_MYSQL_EXECUTE_ONE_CALLBACK_FUNCTOR>> m_executeOneCallBackMap;
};
