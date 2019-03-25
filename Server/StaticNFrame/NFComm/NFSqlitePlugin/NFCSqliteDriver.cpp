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
	CloseConnection();
}

/**
* @brief 打开数据库
*
* @return bool
*/
bool NFCSqliteDriver::Connect(const std::string& dbname)
{
	m_dbName = dbname;
	/*
	** 获得文件路径
	*/
	std::string dirPath = NFFileUtility::GetFileDirName(dbname);
	if (!dirPath.empty())
	{
		/*
		** 文件路径是否存在，不存在就创建
		*/
		if (!NFFileUtility::IsDir(dirPath))
		{
			NFFileUtility::Mkdir(dirPath);
		}
	}

	/*
	** 确保路径存在后，创建数据库，可读可写，不存在就创建
	*/
	int ret = sqlite3_open_v2(dbname.c_str(), &m_pSqliteDB, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, nullptr);

	if (ret == SQLITE_OK)
	{
		SetPragmaInfo();
		NFLogInfo("sqlite3_open:{} success!", dbname);
		return true;
	}
	else
	{
		NFLogError("sqlite3_open:{} failed! error code:{}", dbname, ret);
		CloseConnection();
		return false;
	}
}

/**
* @brief 设计一些属性
*
* @return void
*/
void NFCSqliteDriver::SetPragmaInfo()
{
	ExecSqlNoTransaction("PRAGMA synchronous = OFF;");
	ExecSqlNoTransaction("PRAGMA auto_vacuum = 0;");
	ExecSqlNoTransaction("PRAGMA cache_size = 8000;");
	ExecSqlNoTransaction("PRAGMA case_sensitive_like = 1;");
	ExecSqlNoTransaction("PRAGMA count_changes = 1;");
	ExecSqlNoTransaction("PRAGMA page_size = 8192;");
	ExecSqlNoTransaction("PRAGMA temp_store = MEMORY;");
}

/**
* @brief 关闭数据库
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
* @brief 通过protobuf反射， 来实现sqlite的建表
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

	//获得表的名字
	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	if (IsExistTable(strTableName))
	{
		std::string prepareSql = NFProtobufCommon::GetSqlitePrepareFromMessage(message);
		m_tableNameToPrepareInsertSql.emplace(strTableName, prepareSql);
		return true;
	}

	const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
	if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

	const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

	std::string strColumnSql = NFProtobufCommon::GetSqliteColumnFromMessage(dbFieldsMessage, firstKey);
	if (strColumnSql.empty()) return false;

	bool result = CreateTable(strTableName, strColumnSql);
	if (result)
	{
		std::string prepareSql = NFProtobufCommon::GetSqlitePrepareFromMessage(message);
		m_tableNameToPrepareInsertSql.emplace(strTableName, prepareSql);
	}
	return result;
}

/**
* @brief 释放存在表
*
* @param  string
* @return bool
*/
bool NFCSqliteDriver::IsExistTable(const std::string& tableName)
{
	if (m_pSqliteDB == nullptr) return false;

	int count = 0;
	std::string sql = "select count(*)  from sqlite_master where type = 'table' and name = '";
	sql += tableName;
	sql += "'";

	bool result = BeginTransaction();
	if (!result)
	{
		CommitTransaction();
		return result;
	}

	sqlite3_stmt* pStmt = nullptr;
	SqlitePrepare(sql, &pStmt);
	if (!result)
	{
		if (pStmt)
		{
			SqliteFinalize(pStmt);
		}
		CommitTransaction();
		return result;
	}

	if (SqliteStep(pStmt))
	{
		count = sqlite3_column_int(pStmt, 0);
	}

	result = SqliteFinalize(pStmt);
	if (!result)
	{
		CommitTransaction();
		return result;
	}

	result = CommitTransaction();
	if (!result)
	{
		return result;
	}

	if (count > 0)
	{
		return true;
	}

	return false;
}

/**
* @brief 创建数据库表
*
* @param  tableName 数据库名字
* @param  columnSql 列字符串 类似"ServerID int, UserDBID bigint"
* @return bool
*/
bool NFCSqliteDriver::CreateTable(const std::string& tableName, const std::string& columnSql)
{
	std::string createTableSql = "create table " + tableName + "(" + columnSql + ")";
	return ExecSqlNoTransaction(createTableSql);
}

/**
* @brief 执行一条语句 没有开启事务
*
* @return void
*/
bool NFCSqliteDriver::ExecSqlNoTransaction(const std::string& sql)
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
		NFLogError("sqlite3_exec error, error code:{}, errmsg:{}", result, errmsg);
		NFLogError("sqlite3_exec error, sql:{}", sql);
		return false;
	}

	return true;
}

/**
* @brief 执行一条语句
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

	bool result = BeginTransaction();
	if (!result)
	{
		CommitTransaction();
		return result;
	}
	
	result = ExecSqlNoTransaction(sql);
	if (!result)
	{
		CommitTransaction();
		return result;
	}

	result = CommitTransaction();
	if (!result)
	{
		return result;
	}

	return true;
}

/**
* @brief 执行N条语句
*
* @return void
*/
bool NFCSqliteDriver::ExecSql(const std::vector<std::string>& vecSql)
{
	if (m_pSqliteDB == nullptr)
	{
		NFLogError("m_pSqliteDB == nullptr, Exec Sql Failed!");
		return false;
	}

	bool result = BeginTransaction();
	if (!result)
	{
		CommitTransaction();
		return result;
	}

	for (size_t i = 0; i < vecSql.size(); i++)
	{
		result = ExecSqlNoTransaction(vecSql[1]);
		if (!result)
		{
			CommitTransaction();
			return result;
		}
	}


	result = CommitTransaction();
	if (!result)
	{
		return result;
	}

	return true;
}

/**
* @brief 执行一条插入语句
*
* @return void
*/
bool NFCSqliteDriver::InsertMessage(const google::protobuf::Message& message, bool insertKey)
{
	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	//获得表的名字
	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
	if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

	const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

	bool result = true;
	result = BeginTransaction();
	if (!result)
	{
		return false;
	}

	sqlite3_stmt* pStmt = nullptr;
	result = SqlitePrepare(m_tableNameToPrepareInsertSql[strTableName], &pStmt);
	if (!result)
	{
		if (pStmt)
		{
			SqliteFinalize(pStmt);
		}

		CommitTransaction();
		return result;
	}

	result = InsertOneMessageNoTransactionNoPrepare(dbFieldsMessage, pStmt, insertKey);
	if (!result)
	{
		if (pStmt)
		{
			SqliteFinalize(pStmt);
		}
		CommitTransaction();
		return result;
	}

	result = SqliteFinalize(pStmt);
	if (!result)
	{
		CommitTransaction();
		return result;
	}

	result = CommitTransaction();
	if (!result)
	{
		return false;
	}

	return true;
}

/**
* @brief 执行一条插入语句,没有事务，没有预备编译
*
* @return void
*/
bool NFCSqliteDriver::InsertOneMessageNoTransactionNoPrepare(const google::protobuf::Message& dbFieldsMessage, sqlite3_stmt* pStmt, bool insertKey)
{
	bool result = true;

	result = SqliteReset(pStmt);
	if (!result)
	{
		return result;
	}

	result = SqliteBindStep(dbFieldsMessage, pStmt, insertKey);
	if (!result)
	{
		return result;
	}

	result = SqliteStep(pStmt);
	if (!result)
	{
		return result;
	}

	return true;
}

/**
* @brief 执行N条插入语句
*
* @return void
*/
bool NFCSqliteDriver::InsertMessage(std::vector<google::protobuf::Message*>& vecMessage, bool insertKey)
{
	if (vecMessage.size() <= 0) return true;

	//获得表的名字
	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(*vecMessage[0]);
	if (strTableName.empty()) return false;

	bool result = true;
	result = BeginTransaction();
	if (!result)
	{
		return false;
	}

	sqlite3_stmt* pStmt = nullptr;
	result = SqlitePrepare(m_tableNameToPrepareInsertSql[strTableName], &pStmt);
	if (!result)
	{
		if (pStmt)
		{
			SqliteFinalize(pStmt);
		}

		CommitTransaction();
		return result;
	}

	for (size_t i = 0; i < vecMessage.size(); i++)
	{
		google::protobuf::Message* pMessage = vecMessage[i];
		const google::protobuf::Descriptor* pDesc = pMessage->GetDescriptor();
		if (pDesc == nullptr) return false;

		const google::protobuf::Reflection* pReflect = pMessage->GetReflection();
		if (pReflect == nullptr) return false;

		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(*pMessage, pDbFieldsFieldDesc);

		result = InsertOneMessageNoTransactionNoPrepare(dbFieldsMessage, pStmt, insertKey);
		if (!result)
		{
			if (pStmt)
			{
				SqliteFinalize(pStmt);
			}
			CommitTransaction();
			return result;
		}
	}

	result = SqliteFinalize(pStmt);
	if (!result)
	{
		CommitTransaction();
		return result;
	}

	result = CommitTransaction();
	if (!result)
	{
		return false;
	}

	return true;
}

/**
* @brief
*
* @return bool
*/
bool NFCSqliteDriver::SqliteBindStep(const google::protobuf::Message& message, sqlite3_stmt* pStmt, bool insertKey)
{
	if (m_pSqliteDB == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	for (int i = 0; i < pDesc->field_count(); i++)
	{
		if (i == 0 && insertKey == false)
		{
			continue;
		}

		int result = SQLITE_OK;
		const google::protobuf::FieldDescriptor* pFieldDesc = pDesc->field(i);
		switch (pFieldDesc->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32_t value = pReflect->GetInt32(message, pFieldDesc);
			result = sqlite3_bind_int(pStmt, i + 1, (int)value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64_t value = pReflect->GetInt64(message, pFieldDesc);
			result = sqlite3_bind_int64(pStmt, i + 1, (int64_t)value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32_t value = pReflect->GetUInt32(message, pFieldDesc);
			result = sqlite3_bind_int(pStmt, i + 1, (int)value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64_t value = pReflect->GetUInt64(message, pFieldDesc);
			result = sqlite3_bind_int64(pStmt, i + 1, (uint64_t)value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double value = pReflect->GetDouble(message, pFieldDesc);
			result = sqlite3_bind_double(pStmt, i + 1, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			double value = (double)pReflect->GetFloat(message, pFieldDesc);
			result = sqlite3_bind_double(pStmt, i + 1, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			int32_t value = (int32_t)pReflect->GetBool(message, pFieldDesc);
			result = sqlite3_bind_int(pStmt, i + 1, (int)value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			const google::protobuf::EnumValueDescriptor* pEnumDesc = pReflect->GetEnum(message, pFieldDesc);
			if (pEnumDesc)
			{
				int32_t value = pEnumDesc->number();
				result = sqlite3_bind_int(pStmt, i + 1, (int)value);
			}
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			std::string value = pReflect->GetString(message, pFieldDesc);
			result = sqlite3_bind_text(pStmt, i + 1, value.data(), value.size(), NULL);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			const google::protobuf::Message& data = pReflect->GetMessage(message, pFieldDesc);
			std::string value;
			data.SerializePartialToString(&value);
			result = sqlite3_bind_blob(pStmt, i + 1, value.data(), value.size(), NULL);
		}
		break;
		default:
			break;
		}

		if (result != SQLITE_OK)
		{
			return false;
		}
	}
	return true;
}

/**
* @brief 开启事务
*
* @return bool
*/
bool NFCSqliteDriver::BeginTransaction()
{
	bool result = ExecSqlNoTransaction("begin transaction");
	if (!result)
	{
		NFLogError("begin transaction failed!");
	}
	return result;
}

/**
* @brief 提交事务
*
* @return bool
*/
bool NFCSqliteDriver::CommitTransaction()
{
	bool result = ExecSqlNoTransaction("commit transaction");
	if (!result)
	{
		NFLogError("commit transaction failed!");
	}
	return result;
}

/**
* @brief 编译语句
*
* @return bool
*/
bool NFCSqliteDriver::SqlitePrepare(const std::string& sql, sqlite3_stmt** pStmt)
{
	if (m_pSqliteDB == nullptr)
	{
		NFLogError("m_pSqliteDB == nullptr, SqlitePrepare ({}) Failed!", sql);
		return false;
	}

	int result = SQLITE_OK;

	result = sqlite3_prepare_v2(m_pSqliteDB, sql.data(), sql.size(), pStmt, NULL);
	if (result != SQLITE_OK)
	{
		NFLogError("sqlite3_prepare_v2 error, error code:{}", result);
		NFLogError("sqlite3_prepare_v2 error, sql:{}", sql);
		return false;
	}

	return true;
}

/**
* @brief 释放编译语句
*
* @return bool
*/
bool NFCSqliteDriver::SqliteFinalize(sqlite3_stmt* pStmt)
{
	if (m_pSqliteDB == nullptr)
	{
		NFLogError("m_pSqliteDB == nullptr, SqliteFinalize Failed!");
		return false;
	}

	int result = SQLITE_OK;

	result = sqlite3_finalize(pStmt);
	if (result != SQLITE_OK)
	{
		NFLogError("sqlite3_finalize error, error code:{}", result);
		return false;
	}

	return true;
}

/**
* @brief 释放编译语句
*
* @return bool
*/
bool NFCSqliteDriver::SqliteReset(sqlite3_stmt* pStmt)
{
	if (m_pSqliteDB == nullptr)
	{
		NFLogError("m_pSqliteDB == nullptr, SqliteFinalize Failed!");
		return false;
	}

	int result = SQLITE_OK;

	result = sqlite3_reset(pStmt);
	if (result != SQLITE_OK)
	{
		NFLogError("sqlite3_reset error, error code:{}", result);
		return false;
	}

	return true;
}

/**
* @brief 释放编译语句
*
* @return bool
*/
bool NFCSqliteDriver::SqliteStep(sqlite3_stmt* pStmt)
{
	if (m_pSqliteDB == nullptr)
	{
		NFLogError("m_pSqliteDB == nullptr, SqliteStep Failed!");
		return false;
	}

	int result = SQLITE_OK;

	result = sqlite3_step(pStmt);
	if (result != SQLITE_ROW)
	{
		NFLogError("sqlite3_step error, error code:{}", result);
		return false;
	}

	return true;
}