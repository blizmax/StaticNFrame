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
	 * @brief 插入或更新数据
	 *
	 * @param  message			protobuf信息
	 * @return bool				成功或失败
	 */
	bool UpdateOne(const google::protobuf::Message& message) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @return bool				成功或失败
	 */
	bool QueryOne(google::protobuf::Message& message) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  message			protobuf信息， 需要取得数据是protobuf的域名，取出来的数据是值
	 * @return bool				成功或失败
	 */
	bool QueryMore(google::protobuf::Message& message) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  qstr			执行sql语句
	 * @return bool			成功或失败
	 */
	bool Execute(const std::string& qstr, std::vector<std::map<std::string, std::string>>& keyvalueMap) override;

	/**
	 * @brief 更新或插入数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名
	 * @param  strKey			列值
	 * @param  keyvalueMap		要取出的数据的列名
	 * @return bool				成功或失败
	 */
	bool UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::map<std::string, std::string>& keyvalueMap) override;

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
	bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	              const std::vector<std::string>& fieldVec, std::map<std::string, string>& valueVec) override;

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
	bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	               const std::vector<std::string>& fieldVec,
	               std::vector<std::map<std::string, std::string>>& valueVec) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strWhereSql		条件语句
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			数据
	 * @return bool				成功或失败
	 */
	bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                       const std::vector<std::string>& fieldVec,
	                       std::vector<std::map<std::string, std::string>>& valueVec) override;

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
	bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows,
	                        const std::vector<std::string>& fieldVec,
	                        std::vector<std::map<std::string, std::string>>& valueVec) override;

	/**
	 * @brief 删除数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，将以这个列排序
	 * @param  strKey			数据
	 * @return bool				成功或失败
	 */
	bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) override;

	/**
	 * @brief 查找数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	列名，将以这个列排序
	 * @param  strKey			数据
	 * @param  bExit			是否存在
	 * @return bool				成功或失败
	 */
	bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	            bool& bExit) override;

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
	bool QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                     const std::vector<std::string>& fieldVec,
	                     std::vector<std::map<std::string, std::string>>& valueVec) override;

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
private:
	NFIMysqlDriverManager* m_pMysqlDriverManager;

	uint64_t mnLastCheckTime;
};
