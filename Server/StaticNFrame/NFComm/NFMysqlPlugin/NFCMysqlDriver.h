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

class NFCMysqlDriver : public NFIMysqlDriver
{
public:
	NFCMysqlDriver(const int nReconnectTime = 60, const int nReconnectCount = -1);
	NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd);
	virtual ~NFCMysqlDriver();


	virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd) override;
	virtual bool Execute() override;

	virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) override;
	virtual bool Query(const std::string& qstr) override;

	virtual mysqlpp::Connection* GetConnection() override;
	virtual void CloseConnection();
	virtual bool Enable() override;

	virtual bool CanReconnect() override;
	virtual bool Reconnect() override;

	virtual bool Updata(const google::protobuf::Message& message) override;
	virtual bool Query(google::protobuf::Message& message) override;
	virtual bool QueryMore(google::protobuf::Message& message) override;
	virtual bool Updata(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) override;
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;
	virtual bool Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) override;
	virtual bool Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey) override;
	virtual bool Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit) override;
	virtual bool Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& valueVec) override;
protected:
	bool IsNeedReconnect();
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

