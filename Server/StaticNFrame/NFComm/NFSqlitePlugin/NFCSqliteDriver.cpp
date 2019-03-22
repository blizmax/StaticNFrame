// -------------------------------------------------------------------------
//    @FileName         :    NFCSqliteDriver.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-3-21
//    @Module           :    NFCSqliteDriver
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCSqliteDriver.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"

NFCSqliteDriver::NFCSqliteDriver()
{
	m_pSqliteDB = nullptr;
}

NFCSqliteDriver::~NFCSqliteDriver()
{

}

/**
* @brief �����ݿ�
*
* @return bool
*/
bool NFCSqliteDriver::Connect(const std::string& dbname)
{
	m_dbName = dbname;
	/*
	** ����ļ�·��
	*/
	std::string dirPath = NFFileUtility::GetFileDirName(dbname);
	if (!dirPath.empty())
	{
		/*
		** �ļ�·���Ƿ���ڣ������ھʹ���
		*/
		if (!NFFileUtility::IsDir(dirPath))
		{
			NFFileUtility::Mkdir(dirPath);
		}
	}

	/*
	** ȷ��·�����ں󣬴������ݿ⣬�ɶ���д�������ھʹ���
	*/
	int ret = sqlite3_open_v2(dbname.c_str(), &m_pSqliteDB, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr);

	if (ret == SQLITE_OK)
	{
		NFLogInfo("sqlite3_open:{} success!", dbname);
		return true;
	}
	else
	{
		NFLogError("sqlite3_open:{} failed! error code:{}", dbname, ret);
	}

	CloseConnection();
	return false;
}

/**
* @brief �ر����ݿ�
*
* @return bool
*/
bool NFCSqliteDriver::CloseConnection()
{
	if (m_pSqliteDB)
	{
		int result = sqlite3_close_v2(m_pSqliteDB);
		if (result == SQLITE_OK)
		{
			NFLogInfo("sqlite3_close:{} success!", m_dbName);
			return true;
		}

		NFLogError("sqlite3_close:{} failed! error code:{}", m_dbName, result);
	}
	else
	{
		NFLogError("sqlite3_close:{} failed! m_pSqliteDB == nullptr", m_dbName);
	}

	return false;
}

/**
* @brief ͨ��protobuf���䣬 ��ʵ��sqlite�Ľ���
*
* @param  message
* @return bool
*/
bool NFCSqliteDriver::CreateTable(const google::protobuf::Message& message, bool firstKey)
{
	if (m_pSqliteDB == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	//��ñ������
	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
	if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

	const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

	std::string strColumnSql = NFProtobufCommon::GetSqliteColumnFromMessage(dbFieldsMessage, firstKey);
	if (strColumnSql.empty()) return false;

	return CreateTable(strTableName, strColumnSql);
}

/**
* @brief �������ݿ��
*
* @param  tableName ���ݿ�����
* @param  columnSql ���ַ��� ����"ServerID int, UserDBID bigint"
* @return bool
*/
bool NFCSqliteDriver::CreateTable(const std::string& tableName, const std::string& columnSql)
{
	std::string createTableSql = "create table " + tableName + "(" + columnSql + ")";
	return ExecSql(createTableSql);
}

/**
* @brief ִ��һ�����
*
* @return void
*/

bool NFCSqliteDriver::ExecSql(const std::string& sql)
{
	if (m_pSqliteDB == nullptr)
	{
		NFLogError("m_pSqliteDB == nullptr, Exec Sql ({}) Failed!", sql);
		return false;
	}

	char* errmsg = nullptr;
	int result = SQLITE_OK;

	result = sqlite3_exec(m_pSqliteDB, sql.c_str(), nullptr, nullptr, &errmsg);
	if (result != SQLITE_OK)
	{
		NFLogError("sqlite3_exec error, error code:{}, errmsg:", result, errmsg);
		NFLogError("sqlite3_exec error, sql:", sql);
		return false;
	}

	return true;
}