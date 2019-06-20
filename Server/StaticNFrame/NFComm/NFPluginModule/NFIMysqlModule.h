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

	/**
	 * @brief 插入或更新数据
	 *
	 * @param  message			protobuf信息
	 * @return bool				成功或失败
	 */
	virtual bool UpdateOne(const google::protobuf::Message& message) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @return bool				成功或失败
	 */
	virtual bool QueryOne(google::protobuf::Message& message) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @return bool				成功或失败
	 */
	virtual bool QueryMore(google::protobuf::Message& message) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  qstr			执行sql语句
	 * @param keyvalueMap
	 * @return bool			成功或失败
	 */
	virtual bool Execute(const std::string& qstr, std::vector<std::map<std::string, std::string>>& keyvalueMap) = 0;

	/**
	 * @brief 更新或插入数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  keyvalueMap		要取出的数据的列名
	 * @return bool				成功或失败
	 */
	virtual bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::map<std::string, std::string>& keyvalueMap) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                      const std::vector<std::string>& fieldVec, std::map<std::string, std::string>& valueVec) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，将以这个列排序
	 * @param  nOffset			距离第一行的偏移
	 * @param  nRows			取出多少列
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			平衡ID
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
	                                int nRows, const std::vector<std::string>& fieldVec,
	                                std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		条件语句
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                               const std::vector<std::string>& fieldVec,
	                               std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::vector<std::string>& fieldVec,
	                       std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 删除数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，将以这个列排序
	 * @param  strKey			数据
	 * @return bool				成功或失败
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName,
	                    const std::string& strKey) = 0;

	/**
	 * @brief 查找数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，将以这个列排序
	 * @param  strKey			数据
	 * @param  bExit			是否存在
	 * @return bool				成功或失败
	 */
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                    bool& bExit) = 0;

	/**
	 * @brief 查询类似数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			数据
	 * @return bool				成功或失败
	 */
	virtual bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
	                             const std::string& strKey, const std::vector<std::string>& fieldVec,
	                             std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief 更新或插入数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  keyvalueMap		数据
	 * @return bool				成功或失败
	 */
	virtual bool LuaUpdateOne(const std::string& strTableName, const std::string& strKeyColName,
	                          const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap)
	{
		return UpdateOne(strTableName, strKeyColName, strKey, keyvalueMap);
	}

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec		数据
	 * @return std::map<std::string, std::string>	查询数据
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
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec		数据
	 * @return std::map<std::string, std::string>	查询数据
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
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，以这一列排序
	 * @param  nOffset			数据偏移
	 * @param  nRows			数据数目
	 * @param  fieldVec			数据
	 * @return std::map<std::string, std::string>	查询数据
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
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		条件语句
	 * @param  fieldVec			数据列
	 * @return std::vector<std::map<std::string, std::string>>	查询数据
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
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @return bool				结果
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
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  fieldVec		数据
	 * @return std::vector<std::map<std::string, std::string>>	查询到的数据
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
