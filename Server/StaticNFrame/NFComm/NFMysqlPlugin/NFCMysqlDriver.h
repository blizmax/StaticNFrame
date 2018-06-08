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

#define  NFMYSQLTRYBEGIN try {

#define  NFMYSQLTRYEND(msg) }\
    catch (mysqlpp::BadQuery er) \
    { \
        std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
        return false; \
    } \
    catch (const mysqlpp::BadConversion& er)  \
    { \
        std::cout << "BadConversion [" << msg << "] Error:" << er.what() << " retrieved data size:" << er.retrieved << ", actual size:" << er.actual_size << std::endl; \
        return false; \
    } \
    catch (const mysqlpp::Exception& er) \
    { \
        std::cout << "mysqlpp::Exception [" << msg << "] Error:" << er.what() << std::endl; \
        return false; \
    }\
    catch ( ... ) \
    { \
        std::cout << "std::exception [" <<msg << "] Error:Unknown " << std::endl; \
        return false; \
    }

#define  NFMYSQLTRYENDNULLRETURN(msg) }\
    catch (mysqlpp::BadQuery er) \
    { \
        std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
        return ; \
    } \
    catch (const mysqlpp::BadConversion& er)  \
    { \
        std::cout << "BadConversion [" << msg << "] Error:" << er.what() << " retrieved data size:" << er.retrieved << ", actual size:" << er.actual_size << std::endl; \
        return ; \
    } \
    catch (const mysqlpp::Exception& er) \
    { \
        std::cout << "mysqlpp::Exception ["<<msg<<"] Error:" << er.what() << std::endl; \
        return ; \
    }\
    catch ( ... ) \
    { \
        std::cout << "std::exception [" << msg << "] Error:Unknown " << std::endl; \
        return ; \
    }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/**
* @brief mysql驱动， 里面有一个myql连接
** 实现了通过protobuf的反射来存取数据，使用方法如下：
*****************************保存数据到db方案*******************************
message message_db_base
{
	optional string table_name = 1; //数据库表名
	optional uint32 offset = 2;		//查找数据的时候， 多条数据去偏移， 比如取出50条，要第10条开始取
	optional uint32 rows = 3;		//取出多条数据
	optional string order_key = 4;  //取数据的时候按照这个key排序取， 如果为空，默认按照db_fields第一条排序取
}
message_db_base是基础数据，必须存在， 里面有表名， 以及查询数据时的条件
假设我们要对mysql表player执行update,insert,select等操作, player表有这些属性charid,account,charname,serverid,career,....
我们可以建立一个这样的message，名字随便取
message mysql_player
{
	optional uint64 charid = 1;
	optional string account = 2;
	optional string charname = 3;
	optional uint32 serverid = 4;
	optional uint32 career = 5;
}
这里的charid，account一定要跟表中的列名小写形式完全一样

假设我们要更新插入数据，可以建立一个这样的message
message update_player
{
	optional message_db_base db_base = 1; //数据库基础数据
	optional mysql_player db_fields = 2;
}
update_player这个名字任你取
optional message_db_base db_base = 1;这个是必须的，db_base这个field名是固定的，=1无所谓
optional mysql_player db_fields = 2;mysql_player是你在上面建立的message，随便你取名，db_fields是固定的，必须叫这个，更程序有关
在比如有表mail，你可以：
message table_mail
{
	optional uint64 mailid = 1;
	optional uint64 ownerid = 2;
	optional MixMailData mixinfo = 3;
	optional uint32 mail_type = 4;
}
如果你要更新或插入数据的话，
message update_mail
{
	optional message_db_base db_base = 1; //数据库基础数据
	optional table_mail db_fields = 2;
}
具体代码：
NFCMysqlDriver* m_pDriver = new NFCMysqlDriver;
..........//连接数据库
update_mail upMail;
upMail.mutable_db_base()->set_table_name("mail");//mail是表名
upMail.mutable_db_fields()->set_mailid(111);
upMail.mutable_db_fields()->set_ownerid(112);
....
m_pDriver->Update(upMail);
执行成功后，数据里就多了一条数据
如果你要使用select去数据，如果你要去一条数据：
先建立查询条件，mailid是表mail里的列名
message query_mail_cond
{
	optional uint64 mailid = 1;
}
然后，建立一个message
message query_mail
{
	optional message_db_base db_base = 1; //数据库基础数据
	optional table_mail db_fields = 2;
	optional query_mail_cond db_cond = 3;
}
db_base,db_fields,db_cond这几个字符都不能变, 代码如下：
NFCMysqlDriver* m_pDriver = new NFCMysqlDriver;
..........//连接数据库
query_mail quMail;
quMail.mutable_db_base()->set_table_name("mail");//mail是表名
quMail.mutable_db_cond()->set_mailid(111);
....
m_pDriver->Query(quMail);
最终取出来的结果放在db_fields里
如果你要取多条数据:
message query_mail_cond2
{
	optional uint64 ownerid = 1;
}
message query_mail
{
	optional message_db_base db_base = 1; //数据库基础数据
	repeated table_mail db_fields = 2;
	optional query_mail_cond2 db_cond = 3;
}
将db_fields改成repeated， 
query_mail quMail;
quMail.mutable_db_base()->set_table_name("mail");//mail是表名
quMail.mutable_db_base()->set_offset(10);//mail是表名
quMail.mutable_db_base()->set_rows(40);//mail是表名
quMail.mutable_db_base()->set_order_key("mailid");
quMail.mutable_db_cond2()->set_wonerid(112);
....
m_pDriver->QueryMore(quMail);
意思是取出ownerid=112的mail数据，以mailid排序，只要从10条开始后的40条数据
*/
class NFCMysqlDriver : public NFIMysqlDriver
{
public:
	/**
	 * @brief 构造函数
	 *
	 * @param  nReconnectTime 重连时间
	 * @param  nReconnectCount 重连次数，-1表示无数次
	 * @return  
	 */
	NFCMysqlDriver(const int nReconnectTime = 60, const int nReconnectCount = -1);

	/**
	 * @brief 构造函数
	 *
	 * @param  strDBName 数据库名字
	 * @param  strDBHost 数据库IP
	 * @param  nDBPort   数据库端口
	 * @param  strDBUser 数据库用户
	 * @param  strDBPwd  数据库密码
	 * @return  
	 */
	NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd);

	/**
	 * @brief 析构函数
	 */
	virtual ~NFCMysqlDriver();


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
	virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd) override;

	/**
	 * @brief 循环执行
	 *
	 * @return bool 
	 */
	virtual bool Execute() override;

	/**
	 * @brief 通过sql语句查询数据
	 *
	 * @param  qstr sql语句
	 * @param  queryResult 查询结果
	 * @return bool 查询成功或失败
	 */
	virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) override;

	/**
	 * @brief 执行sql语句
	 *
	 * @param  qstr sql语句
	 * @return bool 执行成功或失败
	 */
	virtual bool Query(const std::string& qstr) override;

	/**
	 * @brief 获得mysqlpp的连接
	 *
	 * @return mysqlpp::Connection* 
	 */
	virtual mysqlpp::Connection* GetConnection() override;

	/**
	 * @brief 关闭连接
	 *
	 * @return void 
	 */
	virtual void CloseConnection();

	/**
	 * @brief 是否连接有效
	 *
	 * @return bool 
	 */
	virtual bool Enable() override;

	/**
	 * @brief 是否可以重连
	 *
	 * @return bool 
	 */
	virtual bool CanReconnect() override;

	/**
	 * @brief 重连
	 *
	 * @return bool 
	 */
	virtual bool Reconnect() override;

	/**
	 * @brief 通过protobuf反射， 来实现mysql的更新和插入
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool Updata(const google::protobuf::Message& message) override;

	/**
	 * @brief 通过protobuf反射，实现查询一条数据，并把语句放在protobuf中
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool Query(google::protobuf::Message& message) override;

	/**
	 * @brief 通过protobuf反射，实现查询很多数据，并把语句放在protobuf中
	 *
	 * @param  message
	 * @return bool 
	 */
	virtual bool QueryMore(google::protobuf::Message& message) override;

	/**
	 * @brief mysql语句更新和插入
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  strKey			key数据
	 * @param  fieldVec			要插入的数据的列名
	 * @param  valueVec			要插入的数据
	 * @return bool				成功或失败
	 */
	virtual bool Updata(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  strKey			key数据
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;

	/**
	 * @brief 查询数据
	 *
	 * @param  strTableName		表名
	 * @param  strKeyColName	key所在列的列名
	 * @param  strKey			key数据
	 * @param  nOffset			要取出数据偏移
	 * @param  nRows			要取出数据的数目
	 * @param  fieldVec			要取出的数据的列名
	 * @param  valueVec			要取出的数据
	 * @return bool				成功或失败
	 */
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;

	/**
	 * @brief 删一行数据
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @return bool 
	 */
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) override;

	/**
	 * @brief 判断数据是否存在
	 *
	 * @param  strTableName
	 * @param  strKeyColName
	 * @param  strKey
	 * @param  bExit
	 * @return bool 
	 */
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit) override;

	/**
	 * @brief 根据类似key的数据，取出一堆来
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
	 * @brief 是否需要重连
	 *
	 * @return bool 
	 */
	bool IsNeedReconnect();

	/**
	 * @brief 连接
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

	mysqlpp::Connection* m_pMysqlConnect; // mysql连接对象
	float mfCheckReconnect; //检查重连的时间

	int mnReconnectTime;
	int mnReconnectCount;

	static NFMutex ConnectLock; //m_pMysqlConnect在调用Connect会引发多线程的崩溃，必须枷锁
};

