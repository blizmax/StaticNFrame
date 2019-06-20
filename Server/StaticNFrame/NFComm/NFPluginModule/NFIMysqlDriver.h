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
	 * @brief �������ݿ�
	 *
	 * @param  strDBName ���ݿ�����
	 * @param  strDBHost ���ݿ�IP
	 * @param  nDBPort   ���ݿ�˿�
	 * @param  strDBUser ���ݿ��û���
	 * @param  strDBPwd  ���ݿ�����
	 * @return bool
	 */
	virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, int nDBPort,
	                     const std::string& strDBUser, const std::string& strDBPwd) = 0;

	/**
	 * @brief ѭ��ִ��
	 *
	 * @return bool
	 */
	virtual bool Execute() = 0;

	/**
	 * @brief ���mysqlpp������
	 *
	 * @return mysqlpp::Connection*
	 */
	virtual mysqlpp::Connection* GetConnection() = 0;

	/**
	 * @brief ͨ��sql����ѯ����
	 *
	 * @param  qstr sql���
	 * @param  queryResult ��ѯ���
	 * @return bool ��ѯ�ɹ���ʧ��
	 */
	virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) = 0;

	/**
	 * @brief ִ��sql���
	 *
	 * @param  qstr sql���
	 * @param  valueVec ��������
	 * @return bool ִ�гɹ���ʧ��
	 */
	virtual bool ExecuteOne(const std::string& qstr, std::map<std::string, std::string>& keyvalueMap) = 0;

	/**
	 * @brief ִ��sql���
	 *
	 * @param  qstr sql���
	 * @param  valueVec ��������
	 * @return bool ִ�гɹ���ʧ��
	 */
	virtual bool ExecuteMore(const std::string& qstr, std::vector<std::map<std::string, std::string>>& keyvalueMap) = 0;

	/**
	 * @brief �Ƿ�������Ч
	 *
	 * @return bool
	 */
	virtual bool Enable() = 0;

	/**
	 * @brief ����
	 *
	 * @return bool
	 */
	virtual bool Reconnect() = 0;

	/**
	 * @brief �Ƿ��������
	 *
	 * @return bool
	 */
	virtual bool CanReconnect() = 0;

	/**
	 * @brief ͨ��protobuf���䣬 ��ʵ��mysql�ĸ��ºͲ���
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool Update(const google::protobuf::Message& message) = 0;

	/**
	 * @brief ͨ��protobuf���䣬ʵ�ֲ�ѯһ�����ݣ�����������protobuf��
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool Query(google::protobuf::Message& message) = 0;

	/**
	 * @brief ͨ��protobuf���䣬ʵ�ֲ�ѯ�ܶ����ݣ�����������protobuf��
	 *
	 * @param  message
	 * @return bool
	 */
	virtual bool QueryMore(google::protobuf::Message& message) = 0;

	/**
	 * @brief mysql�����ºͲ���
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  strKey			key����
	 * @param  keyvalueMap		����
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool Update(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                    const std::map<std::string, std::string>& keyvalueMap) = 0;

	/**
	 * @brief ��ѯ����һ������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  strKey			key����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			Ҫȡ��������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                      const std::vector<std::string>& fieldVec, std::map<std::string, std::string>& valueVec) = 0;

	/**
	 * @brief ��ѯ�����������
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  strKey			key����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			Ҫȡ��������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey,
	                       const std::vector<std::string>& fieldVec,
	                       std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  nOffset			Ҫȡ������ƫ��
	 * @param  nRows			Ҫȡ�����ݵ���Ŀ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			Ҫȡ��������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
	                                int nRows, const std::vector<std::string>& fieldVec,
	                                std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strWhereSql		��ѯ����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			Ҫȡ��������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
	                               const std::vector<std::string>& fieldVec,
	                               std::vector<std::map<std::string, std::string>>& valueVec) = 0;

	/**
	 * @brief ɾһ������
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @return bool
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName,
	                    const std::string& strKey) = 0;

	/**
	 * @brief �ж������Ƿ����
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
	 * @brief ��������key�����ݣ�ȡ��һ����
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
