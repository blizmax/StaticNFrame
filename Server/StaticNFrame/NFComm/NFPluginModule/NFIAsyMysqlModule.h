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

//异步mysql回调函数
typedef std::function<void(bool result)> ASYC_MYSQL_UPDATE_CALLBACK_FUNCTOR;

class NFIAsyMysqlModule
	: public NFIAsycModule
{
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
	virtual bool AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

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
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& fieldValueMap, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		查询条件
	 * @param  fieldVec			要取出的数据的列名
	 * @param  balanceId		平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;
	
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
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

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
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& fieldVec, uint64_t balanceId = 0) = 0;

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

