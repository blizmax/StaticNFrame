// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlDriver.h
//    @Author           :    chuano.guo
//    @Date             :    2014-07-28
//    @Module           :    NFIMysqlDriver
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef NFI_MYSQL_DRIVER_H
#define NFI_MYSQL_DRIVER_H
//#define MS_HIREDIS
#ifdef _MSC_VER
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
//#include <sys/timeb.h>
#include <sys/types.h>

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#endif

#include <string>
#include <vector>
#include <set>
#include "mysqlpp/lib/mysql++.h"

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

class NFIMysqlDriver
{
public:
	virtual ~NFIMysqlDriver()
	{
	}

	/**
	 * @brief 连接数据库
	 *
	 * @param  strDBName 数据库名字
	 * @param  strDBHost 数据库IP
	 * @param  nDBPort   数据库端口
	 * @param  strDBUser 数据库用户名
	 * @param  strDBPwd  数据库密码
	 * @return bool
	 */
	virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, int nDBPort,
	                     const std::string& strDBUser, const std::string& strDBPwd) = 0;

	/**
	 * @brief 循环执行
	 *
	 * @return bool
	 */
	virtual bool Execute() = 0;

	/**
	 * @brief 获得mysqlpp的连接
	 *
	 * @return mysqlpp::Connection*
	 */
	virtual mysqlpp::Connection* GetConnection() = 0;

	/**
	 * @brief 通过sql语句查询数据
	 *
	 * @param  qstr sql语句
	 * @param  queryResult 查询结果
	 * @return bool 查询成功或失败
	 */
	virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) = 0;

	/**
	 * @brief 执行sql语句
	 *
	 * @param  qstr sql语句
	 * @param  valueVec 返回数据
	 * @return bool 执行成功或失败
	 */
	virtual bool ExecuteOne(const std::string& qstr, std::map<std::string, std::string>& keyvalueMap) = 0;

	/**
	 * @brief 执行sql语句
	 *
	 * @param  qstr sql语句
	 * @param  valueVec 返回数据
	 * @return bool 执行成功或失败
	 */
	virtual bool ExecuteMore(const std::string& qstr, std::vector<std::map<std::string, std::string>>& keyvalueMap) = 0;

	/**
	 * @brief 是否连接有效
	 *
	 * @return bool
	 */
	virtual bool Enable() = 0;

	/**
	 * @brief 重连
	 *
	 * @return bool
	 */
	virtual bool Reconnect() = 0;

	/**
	 * @brief 是否可以重连
	 *
	 * @return bool
	 */
	virtual bool CanReconnect() = 0;

	/**
	 * @brief 通过protobuf反射， 来实现mysql的更新和插入
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool Update(const google::protobuf::Message& message) = 0;

	/**
	 * @brief 通过protobuf反射，实现查询一条数据，并把语句放在protobuf中
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool Query(google::protobuf::Message& message) = 0;

	/**
	 * @brief 通过protobuf反射，实现查询很多数据，并把语句放在protobuf中
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool QueryMore(google::protobuf::Message& message) = 0;

	/**
	 * @brief mysql语句更新和插入
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  strKey			key数据
	 * @param  keyvalueMap		数据
	 * @return bool				成功或失败
	 */
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                    const std::map<std::string, std::string>& keyvalueMap) = 0;

	/**
	 * @brief 查询数据一条数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  strKey			key数据
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                      const std::vector<std::string>& fieldVec, std::map<std::string, std::string>& valueVec) = 0;

	/**
	 * @brief 查询数据许多数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  strKey			key数据
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::vector<std::string>& fieldVec,
	                       std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  nOffset			要取出数据偏移
	 * @param  nRows			要取出数据的数目
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
	                                int nRows, const std::vector<std::string>& fieldVec,
	                                std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		查询条件
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                               const std::vector<std::string>& fieldVec,
	                               std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 删一行数据
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @return bool
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName,
	                    const std::string& strKey) = 0;

	/**
	 * @brief 判断数据是否存在
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @param  bExit
	 * @return bool
	 */
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                    bool& bExit) = 0;

	/**
	 * @brief 根据类似key的数据，取出一堆来
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKeyName
	 * @param  fieldVec
	 * @param  valueVec
	 * @return bool
	 */
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                             const std::string& strKeyName, const std::vector<std::string>& fieldVec,
	                             std::vector<std::map<std::string, std::string>>& valueVec) = 0;
};

#endif // !_NFI_REDIS_DRIVER_MODULE_H_
