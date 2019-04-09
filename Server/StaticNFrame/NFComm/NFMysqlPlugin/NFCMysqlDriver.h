// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlDriver.h
//    @Author           :    eliteYang
//    @Date             :    2014-07-28
//    @Module           :    NFCMysqlDriver
//    @Desc             :
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIMysqlDriver.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include <string>
#include <mysqlpp/lib/connection.h>
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#define  NFMYSQLTRYBEGIN try {

#define  NFMYSQLTRYEND(msg) }\
    catch (mysqlpp::BadQuery er) \
    { \
		NFLogError(NF_LOG_SYSTEMLOG, 0, "BadQuery [{}] Error:{}", msg, er.what());\
        return false; \
    } \
    catch (const mysqlpp::BadConversion& er)  \
    { \
		NFLogError(NF_LOG_SYSTEMLOG, 0, "BadConversion [{}] Error:{} retrieved data size:{}, actual size:{}", msg, er.what(), er.retrieved, er.actual_size);\
        return false; \
    } \
    catch (const mysqlpp::Exception& er) \
    { \
		NFLogError(NF_LOG_SYSTEMLOG, 0, "mysqlpp::Exception [{}] Error:{}", msg, er.what());\
        return false; \
    }\
    catch ( ... ) \
    { \
		NFLogError(NF_LOG_SYSTEMLOG, 0, "std::exception [{}] Error:Error:Unknown", msg);\
        return false; \
    }

#define  NFMYSQLTRYENDNULLRETURN(msg) }\
    catch (mysqlpp::BadQuery er) \
    { \
		NFLogError(NF_LOG_SYSTEMLOG, 0, "BadQuery [{}] Error:{}", msg, er.what());\
        std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
        return ; \
    } \
    catch (const mysqlpp::BadConversion& er)  \
    { \
        NFLogError(NF_LOG_SYSTEMLOG, 0, "BadConversion [{}] Error:{} retrieved data size:{}, actual size:{}", msg, er.what(), er.retrieved, er.actual_size);\
        return ; \
    } \
    catch (const mysqlpp::Exception& er) \
    { \
        NFLogError(NF_LOG_SYSTEMLOG, 0, "mysqlpp::Exception [{}] Error:{}", msg, er.what());\
        return ; \
    }\
    catch ( ... ) \
    { \
        NFLogError(NF_LOG_SYSTEMLOG, 0, "std::exception [{}] Error:Error:Unknown", msg);\
        return ; \
    }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**
* @brief mysql������ ������һ��myql����
** ʵ����ͨ��protobuf�ķ�������ȡ���ݣ�ʹ�÷������£�
*****************************�������ݵ�db����*******************************
message message_db_base
{
	optional string table_name = 1; //���ݿ����
	optional uint32 offset = 2;		//�������ݵ�ʱ�� ��������ȥƫ�ƣ� ����ȡ��50����Ҫ��10����ʼȡ
	optional uint32 rows = 3;		//ȡ����������
	optional string order_key = 4;  //ȡ���ݵ�ʱ�������key����ȡ�� ���Ϊ�գ�Ĭ�ϰ���db_fields��һ������ȡ
}
message_db_base�ǻ������ݣ�������ڣ� �����б����� �Լ���ѯ����ʱ������
��������Ҫ��mysql��playerִ��update,insert,select�Ȳ���, player������Щ����charid,account,charname,serverid,career,....
���ǿ��Խ���һ��������message���������ȡ
message mysql_player
{
	optional uint64 charid = 1;
	optional string account = 2;
	optional string charname = 3;
	optional uint32 serverid = 4;
	optional uint32 career = 5;
}
�����charid��accountһ��Ҫ�����е�����Сд��ʽ��ȫһ��

��������Ҫ���²������ݣ����Խ���һ��������message
message update_player
{
	optional message_db_base db_base = 1; //���ݿ��������
	optional mysql_player db_fields = 2;
}
update_player�����������ȡ
optional message_db_base db_base = 1;����Ǳ���ģ�db_base���field���ǹ̶��ģ�=1����ν
optional mysql_player db_fields = 2;mysql_player���������潨����message�������ȡ����db_fields�ǹ̶��ģ������������������й�
�ڱ����б�mail������ԣ�
message table_mail
{
	optional uint64 mailid = 1;
	optional uint64 ownerid = 2;
	optional MixMailData mixinfo = 3;
	optional uint32 mail_type = 4;
}
�����Ҫ���»�������ݵĻ���
message update_mail
{
	optional message_db_base db_base = 1; //���ݿ��������
	optional table_mail db_fields = 2;
}
������룺
NFCMysqlDriver* m_pDriver = new NFCMysqlDriver;
..........//�������ݿ�
update_mail upMail;
upMail.mutable_db_base()->set_table_name("mail");//mail�Ǳ���
upMail.mutable_db_fields()->set_mailid(111);
upMail.mutable_db_fields()->set_ownerid(112);
....
m_pDriver->Update(upMail);
ִ�гɹ���������Ͷ���һ������
�����Ҫʹ��selectȥ���ݣ������Ҫȥһ�����ݣ�
�Ƚ�����ѯ������mailid�Ǳ�mail�������
message query_mail_cond
{
	optional uint64 mailid = 1;
}
Ȼ�󣬽���һ��message
message query_mail
{
	optional message_db_base db_base = 1; //���ݿ��������
	optional table_mail db_fields = 2;
	optional query_mail_cond db_cond = 3;
}
db_base,db_fields,db_cond�⼸���ַ������ܱ�, �������£�
NFCMysqlDriver* m_pDriver = new NFCMysqlDriver;
..........//�������ݿ�
query_mail quMail;
quMail.mutable_db_base()->set_table_name("mail");//mail�Ǳ���
quMail.mutable_db_cond()->set_mailid(111);
....
m_pDriver->Query(quMail);
����ȡ�����Ľ������db_fields��
�����Ҫȡ��������:
message query_mail_cond2
{
	optional uint64 ownerid = 1;
}
message query_mail
{
	optional message_db_base db_base = 1; //���ݿ��������
	repeated table_mail db_fields = 2;
	optional query_mail_cond2 db_cond = 3;
}
��db_fields�ĳ�repeated�� 
query_mail quMail;
quMail.mutable_db_base()->set_table_name("mail");//mail�Ǳ���
quMail.mutable_db_base()->set_offset(10);//mail�Ǳ���
quMail.mutable_db_base()->set_rows(40);//mail�Ǳ���
quMail.mutable_db_base()->set_order_key("mailid");
quMail.mutable_db_cond2()->set_wonerid(112);
....
m_pDriver->QueryMore(quMail);
��˼��ȡ��ownerid=112��mail���ݣ���mailid����ֻҪ��10����ʼ���40������
*/
class NFCMysqlDriver : public NFIMysqlDriver
{
public:
	/**
	 * @brief ���캯��
	 *
	 * @param  nReconnectTime ����ʱ��
	 * @param  nReconnectCount ����������-1��ʾ������
	 * @return  
	 */
	NFCMysqlDriver(const int nReconnectTime = 60, const int nReconnectCount = -1);

	/**
	 * @brief ���캯��
	 *
	 * @param  strDBName ���ݿ�����
	 * @param  strDBHost ���ݿ�IP
	 * @param  nDBPort   ���ݿ�˿�
	 * @param  strDBUser ���ݿ��û�
	 * @param  strDBPwd  ���ݿ�����
	 * @return  
	 */
	NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd);

	/**
	 * @brief ��������
	 */
	virtual ~NFCMysqlDriver();


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
	virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd) override;

	/**
	 * @brief ѭ��ִ��
	 *
	 * @return bool 
	 */
	virtual bool Execute() override;

	/**
	 * @brief ͨ��sql����ѯ����
	 *
	 * @param  qstr sql���
	 * @param  queryResult ��ѯ���
	 * @return bool ��ѯ�ɹ���ʧ��
	 */
	virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) override;

	/**
	 * @brief ִ��sql���
	 *
	 * @param  qstr sql���
	 * @return bool ִ�гɹ���ʧ��
	 */
	virtual bool Query(const std::string& qstr) override;

	/**
	 * @brief ���mysqlpp������
	 *
	 * @return mysqlpp::Connection* 
	 */
	virtual mysqlpp::Connection* GetConnection() override;

	/**
	 * @brief �ر�����
	 *
	 * @return void 
	 */
	virtual void CloseConnection();

	/**
	 * @brief �Ƿ�������Ч
	 *
	 * @return bool 
	 */
	virtual bool Enable() override;

	/**
	 * @brief �Ƿ��������
	 *
	 * @return bool 
	 */
	virtual bool CanReconnect() override;

	/**
	 * @brief ����
	 *
	 * @return bool 
	 */
	virtual bool Reconnect() override;

	/**
	 * @brief ͨ��protobuf���䣬 ��ʵ��mysql�ĸ��ºͲ���
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool Updata(const google::protobuf::Message& message) override;

	/**
	 * @brief ͨ��protobuf���䣬ʵ�ֲ�ѯһ�����ݣ�����������protobuf��
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool Query(google::protobuf::Message& message) override;

	/**
	 * @brief ͨ��protobuf���䣬ʵ�ֲ�ѯ�ܶ����ݣ�����������protobuf��
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool QueryMore(google::protobuf::Message& message) override;

	/**
	 * @brief mysql�����ºͲ���
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  strKey			key����
	 * @param  fieldVec			Ҫ��������ݵ�����
	 * @param  valueVec			Ҫ���������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool Updata(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  strKey			key����
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			Ҫȡ��������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;

	/**
	 * @brief ��ѯ����
	 *
	 * @param  strTableName		����
	 * @param  strKeyColName	key�����е�����
	 * @param  strKey			key����
	 * @param  nOffset			Ҫȡ������ƫ��
	 * @param  nRows			Ҫȡ�����ݵ���Ŀ
	 * @param  fieldVec			Ҫȡ�������ݵ�����
	 * @param  valueVec			Ҫȡ��������
	 * @return bool				�ɹ���ʧ��
	 */
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;

	/**
	 * @brief ɾһ������
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @return bool 
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) override;

	/**
	 * @brief �ж������Ƿ����
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @param  bExit
	 * @return bool 
	 */
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit) override;

	/**
	 * @brief ��������key�����ݣ�ȡ��һ����
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKeyName
	 * @param  valueVec
	 * @return bool 
	 */
	virtual bool Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& valueVec) override;
protected:
	/**
	 * @brief �Ƿ���Ҫ����
	 *
	 * @return bool 
	 */
	bool IsNeedReconnect();

	/**
	 * @brief ����
	 *
	 * @return bool 
	 */
	bool Connect();

private:
	std::string mstrDBName;
	std::string mstrDBHost;
	int mnDBPort;
	std::string mstrDBUser;
	std::string mstrDBPwd;

	mysqlpp::Connection* m_pMysqlConnect; // mysql���Ӷ���
	float mfCheckReconnect; //���������ʱ��

	int mnReconnectTime;
	int mnReconnectCount;

	static NFMutex ConnectLock; //m_pMysqlConnect�ڵ���Connect���������̵߳ı������������
};

