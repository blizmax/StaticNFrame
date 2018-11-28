#include "NFCMysqlDriver.h"
#include "NFComm/NFCore/NFCommon.h"
#include "Dependencies/common/lexical_cast.hpp"

//m_pMysqlConnect在调用Connect会引发多线程的崩溃，必须枷锁
NFMutex NFCMysqlDriver::ConnectLock;

NFCMysqlDriver::NFCMysqlDriver(const int nReconnectTime/* = 60*/, const int nReconnectCount /*= -1*/)
{
	mfCheckReconnect = 0.0f;
	mnDBPort = 0;
	m_pMysqlConnect = NULL;
	mnReconnectTime = nReconnectTime;
	mnReconnectCount = nReconnectCount;
}

NFCMysqlDriver::NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
	mfCheckReconnect = 0.0f;
	mnDBPort = 0;
	m_pMysqlConnect = NULL;

	mnReconnectTime = 60;
	mnReconnectCount = -1;

	Connect(strDBName, strDBHost, nDBPort, strDBUser, strDBPwd);
}

NFCMysqlDriver::~NFCMysqlDriver()
{
	CloseConnection();
}

bool NFCMysqlDriver::Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
	mstrDBName = strDBName ;
	mstrDBHost = strDBHost ;
	mnDBPort = nDBPort ;
	mstrDBUser = strDBUser ;
	mstrDBPwd = strDBPwd ;

	return Connect();
}

bool NFCMysqlDriver::Execute()
{
	if (IsNeedReconnect() && CanReconnect())
	{
		Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);
	}

	return true;
}

bool NFCMysqlDriver::Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult)
{
	mysqlpp::Connection* pConection = GetConnection();
	if (pConection)
	{
		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConection->query(qstr);
			//query.execute();

			queryResult = query.store();
			query.reset();

			NFMYSQLTRYEND(qstr)
		return true;
	}

	return false;
}

bool NFCMysqlDriver::Query(const std::string& qstr)
{
	mysqlpp::Connection* pConection = GetConnection();
	if (pConection)
	{
		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConection->query(qstr);
			query.execute();

			query.reset();

			NFMYSQLTRYEND(qstr)
		return true;
	}

	return false;
}

mysqlpp::Connection* NFCMysqlDriver::GetConnection()
{
	return m_pMysqlConnect;
}

void NFCMysqlDriver::CloseConnection()
{
	delete m_pMysqlConnect;
	m_pMysqlConnect = NULL;
}

bool NFCMysqlDriver::Enable()
{
	return !IsNeedReconnect();
}

bool NFCMysqlDriver::CanReconnect()
{
	mfCheckReconnect += 0.1f;

	//30分钟检查断线重连
	if (mfCheckReconnect < mnReconnectTime)
	{
		return false;
	}

	if (mnReconnectCount == 0)
	{
		return false;
	}

	mfCheckReconnect = 0.0f;

	return true;
}

bool NFCMysqlDriver::Reconnect()
{
	CloseConnection();
	Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);

	if (mnReconnectCount > 0)
	{
		mnReconnectCount--;
	}

	return true;
}

bool NFCMysqlDriver::IsNeedReconnect()
{
	//没有配置表
	if (mstrDBHost.length() < 1 || mstrDBUser.length() < 1)
	{
		return false;
	}

	if (NULL == m_pMysqlConnect)
	{
		return true;
	}

	if (!m_pMysqlConnect->connected())
	{
		CloseConnection();
		return true;
	}

	if (!m_pMysqlConnect->ping())
	{
		CloseConnection();
		return true;
	}

	return false;
}

bool NFCMysqlDriver::Connect()
{
	m_pMysqlConnect = new mysqlpp::Connection();
	if (NULL == m_pMysqlConnect)
	{
		return false;
	}
	NFMYSQLTRYBEGIN
		m_pMysqlConnect->set_option(new mysqlpp::MultiStatementsOption(true));
		m_pMysqlConnect->set_option(new mysqlpp::SetCharsetNameOption("utf8mb4"));
		m_pMysqlConnect->set_option(new mysqlpp::ReconnectOption(true));
		m_pMysqlConnect->set_option(new mysqlpp::ConnectTimeoutOption(60));

		{
			//m_pMysqlConnect在调用Connect会引发多线程的崩溃，必须枷锁
			NFMutexLock lock(&ConnectLock);
			if (!m_pMysqlConnect->connect(mstrDBName.c_str(), mstrDBHost.c_str(), mstrDBUser.c_str(), mstrDBPwd.c_str(), mnDBPort))
			{
				CloseConnection();
				// 连接失败
				return false;
			}
		}

		// 设置超时时间为24小时
		mysqlpp::Query query = m_pMysqlConnect->query("set interactive_timeout = 24*3600");
		query.execute();
		query.reset();
		NFMYSQLTRYEND("Connect faild")
	return true;
}

void SetFieldsString(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc, const std::string& strValue)
{
	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr || pFieldDesc == nullptr) return;

	switch (pFieldDesc->cpp_type())
	{
	case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32_t value = (int32_t)lexical_cast<long long>(strValue);
			pReflect->SetInt32(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64_t value = (int64_t)lexical_cast<long long>(strValue);
			pReflect->SetInt64(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32_t value = (uint32_t)lexical_cast<long long>(strValue);
			pReflect->SetUInt32(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64_t value = (uint64_t)lexical_cast<long long>(strValue);
			pReflect->SetUInt64(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double value = lexical_cast<double>(strValue);
			pReflect->SetDouble(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			float value = lexical_cast<float>(strValue);
			pReflect->SetFloat(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			bool value = (bool)lexical_cast<long long>(strValue);
			pReflect->SetBool(&message, pFieldDesc, value);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			int value = lexical_cast<int>(strValue);
			const google::protobuf::EnumDescriptor* pEnumDesc = pFieldDesc->enum_type();
			if (pEnumDesc == nullptr) return;

			const google::protobuf::EnumValueDescriptor* pEnumValueDesc = pEnumDesc->FindValueByNumber(value);
			if (pEnumValueDesc == nullptr) return;

			pReflect->SetEnum(&message, pFieldDesc, pEnumValueDesc);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			pReflect->SetString(&message, pFieldDesc, strValue);
			return;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			google::protobuf::Message* pMutableMessage = pReflect->MutableMessage(&message, pFieldDesc);
			if (pMutableMessage == nullptr) return;

			pMutableMessage->ParsePartialFromString(strValue);
			return;
		}
		break;
	default:
		break;
	}
	return;
}

std::string GetFieldsString(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc)
{
	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr || pFieldDesc == nullptr) return std::string();

	switch (pFieldDesc->cpp_type())
	{
	case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32_t value = pReflect->GetInt32(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64_t value = pReflect->GetInt64(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32_t value = pReflect->GetUInt32(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64_t value = pReflect->GetUInt64(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double value = pReflect->GetDouble(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			float value = pReflect->GetFloat(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			bool value = pReflect->GetBool(message, pFieldDesc);
			return lexical_cast<std::string>(value);
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			const google::protobuf::EnumValueDescriptor* pEnumDesc = pReflect->GetEnum(message, pFieldDesc);
			if (pEnumDesc)
			{
				return lexical_cast<std::string>(pEnumDesc->number());
			}
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			std::string value = pReflect->GetString(message, pFieldDesc);
			return value;
		}
		break;
	case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			const google::protobuf::Message& value = pReflect->GetMessage(message, pFieldDesc);
			std::string msg;
			value.SerializePartialToString(&msg);
			return msg;
		}
		break;
	default:
		break;
	}
	return std::string();
}

bool NFCMysqlDriver::Updata(const google::protobuf::Message& message)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (pConnection == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::string strTableName;
	std::string strKeyName;
	std::string strKey;

	//处理db_base， 表名，表key
	{
		const google::protobuf::FieldDescriptor* pDbBaseFieldDesc = pDesc->FindFieldByLowercaseName("db_base");
		if (pDbBaseFieldDesc == nullptr || pDbBaseFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbBaseMessage = pReflect->GetMessage(message, pDbBaseFieldDesc);

		const google::protobuf::Descriptor* pDbBaseDesc = dbBaseMessage.GetDescriptor();
		if (pDbBaseDesc == nullptr) return false;

		const google::protobuf::Reflection* pDbBaseReflect = dbBaseMessage.GetReflection();
		if (pDbBaseReflect == nullptr) return false;

		const google::protobuf::FieldDescriptor* pTableNameDesc = pDbBaseDesc->FindFieldByLowercaseName("table_name");
		if (pTableNameDesc == nullptr || pTableNameDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING) return false;

		strTableName = GetFieldsString(dbBaseMessage, pTableNameDesc);
	}

	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

		const google::protobuf::Descriptor* pDbFieldsDesc = dbFieldsMessage.GetDescriptor();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		{
			const google::protobuf::FieldDescriptor* pKeyFieldDesc = pDbFieldsDesc->field(0);
			if (pKeyFieldDesc == nullptr) return false;

			strKeyName = pKeyFieldDesc->name();
			strKey = GetFieldsString(dbFieldsMessage, pKeyFieldDesc);
		}

		bool bExist = false;
		if (!Exists(strTableName, strKeyName, strKey, bExist))
		{
			return false;
		}

		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConnection->query();
			if (bExist)
			{
				// update
				query << "UPDATE " << strTableName << " SET ";
				for (int i = 0; i < pDbFieldsDesc->field_count(); ++i)
				{
					const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(i);
					if (pTemp == nullptr) return false;

					if (i == 0)
					{
						query << pTemp->name() << " = " << mysqlpp::quote << GetFieldsString(dbFieldsMessage, pTemp);
					}
					else
					{
						query << "," << pTemp->name() << " = " << mysqlpp::quote << GetFieldsString(dbFieldsMessage, pTemp);
					}
				}

				query << " WHERE " << strKeyName << " = " << mysqlpp::quote << strKey << ";";
			}
			else
			{
				// insert
				query << "INSERT INTO " << strTableName << "(";
				for (size_t i = 0; i < pDbFieldsDesc->field_count(); ++i)
				{
					const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(i);
					if (pTemp == nullptr) return false;

					if (i == 0)
					{
						query << pTemp->name();
					}
					else
					{
						query << ", " << pTemp->name();
					}
				}

				query << ") VALUES (";
				for (size_t i = 0; i < pDbFieldsDesc->field_count(); ++i)
				{
					const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(i);
					if (pTemp == nullptr) return false;

					if (i == 0)
					{
						query << mysqlpp::quote << GetFieldsString(dbFieldsMessage, pDbFieldsDesc->field(i));
					}
					else
					{
						query << ", " << mysqlpp::quote << GetFieldsString(dbFieldsMessage, pDbFieldsDesc->field(i));
					}
				}

				query << ");";
			}

			query.execute();
			query.reset();
			NFMYSQLTRYEND("update or insert error")
	}

	return true;
}

bool NFCMysqlDriver::Query(google::protobuf::Message& message)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (pConnection == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::string strTableName;

	//处理db_base， 表名，表key
	{
		const google::protobuf::FieldDescriptor* pDbBaseFieldDesc = pDesc->FindFieldByLowercaseName("db_base");
		if (pDbBaseFieldDesc == nullptr || pDbBaseFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbBaseMessage = pReflect->GetMessage(message, pDbBaseFieldDesc);

		const google::protobuf::Descriptor* pDbBaseDesc = dbBaseMessage.GetDescriptor();
		if (pDbBaseDesc == nullptr) return false;

		const google::protobuf::Reflection* pDbBaseReflect = dbBaseMessage.GetReflection();
		if (pDbBaseReflect == nullptr) return false;

		const google::protobuf::FieldDescriptor* pTableNameDesc = pDbBaseDesc->FindFieldByLowercaseName("table_name");
		if (pTableNameDesc == nullptr || pTableNameDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING) return false;

		strTableName = GetFieldsString(dbBaseMessage, pTableNameDesc);
	}

	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Descriptor* pDbFieldsDesc = pDbFieldsFieldDesc->message_type();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		const google::protobuf::FieldDescriptor* pDbCondsFieldDesc = pDesc->FindFieldByLowercaseName("db_cond");
		if (pDbCondsFieldDesc == nullptr || pDbCondsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbCondsMessage = pReflect->GetMessage(message, pDbCondsFieldDesc);

		const google::protobuf::Descriptor* pDbCondsDesc = dbCondsMessage.GetDescriptor();
		if (pDbCondsDesc == nullptr) return false;

		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConnection->query();
			query << "SELECT ";
			for (int i = 0; i < pDbFieldsDesc->field_count(); i++)
			{
				const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(i);
				if (pTemp == nullptr) return false;

				if (i == 0)
				{
					query << pTemp->name();
				}
				else
				{
					query << "," << pTemp->name();
				}
			}

			query << " FROM " << strTableName << " WHERE ";

			for (int i = 0; i < pDbCondsDesc->field_count(); i++)
			{
				const google::protobuf::FieldDescriptor* pTemp = pDbCondsDesc->field(i);
				if (pTemp == nullptr) return false;

				if (i == 0)
				{
					query << pTemp->name() << " = " << mysqlpp::quote << GetFieldsString(dbCondsMessage, pTemp);
				}
				else
				{
					query << " and " << pTemp->name() << " = " << mysqlpp::quote << GetFieldsString(dbCondsMessage, pTemp);
				}
			}

			query << " limit 1;";

			//query.execute(); // 官网例子不需要execute
			mysqlpp::StoreQueryResult xResult = query.store();
			query.reset();

			if (xResult.empty() || !xResult)
			{
				return false;
			}

			// xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
			for (size_t i = 0; i < xResult.size(); ++i)
			{
				google::protobuf::Message* pDbFieldsMessage = pReflect->MutableMessage(&message, pDbFieldsFieldDesc);
				for (size_t j = 0; j < pDbFieldsDesc->field_count(); ++j)
				{
					const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(j);
					if (pTemp == nullptr) return false;

					const std::string& strFieldName = pTemp->name();
					std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());

					SetFieldsString(*pDbFieldsMessage, pTemp, strValue);
				}
			}
			NFMYSQLTRYEND("query error")
	}

	return true;
}

bool NFCMysqlDriver::QueryMore(google::protobuf::Message& message)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (pConnection == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::string strTableName;
	std::string nOffset;
	std::string nRows;
	std::string orderKey;

	//处理db_base， 表名，表key
	{
		const google::protobuf::FieldDescriptor* pDbBaseFieldDesc = pDesc->FindFieldByLowercaseName("db_base");
		if (pDbBaseFieldDesc == nullptr || pDbBaseFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbBaseMessage = pReflect->GetMessage(message, pDbBaseFieldDesc);

		const google::protobuf::Descriptor* pDbBaseDesc = dbBaseMessage.GetDescriptor();
		if (pDbBaseDesc == nullptr) return false;

		const google::protobuf::Reflection* pDbBaseReflect = dbBaseMessage.GetReflection();
		if (pDbBaseReflect == nullptr) return false;

		const google::protobuf::FieldDescriptor* pTableNameDesc = pDbBaseDesc->FindFieldByLowercaseName("table_name");
		if (pTableNameDesc == nullptr || pTableNameDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING) return false;

		const google::protobuf::FieldDescriptor* pOffsetDesc = pDbBaseDesc->FindFieldByLowercaseName("offset");
		if (pOffsetDesc == nullptr || pOffsetDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_UINT32) return false;

		const google::protobuf::FieldDescriptor* pRowsDesc = pDbBaseDesc->FindFieldByLowercaseName("rows");
		if (pRowsDesc == nullptr || pRowsDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_UINT32) return false;

		const google::protobuf::FieldDescriptor* pOrderKeyDesc = pDbBaseDesc->FindFieldByLowercaseName("order_key");
		if (pOrderKeyDesc == nullptr || pOrderKeyDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING) return false;

		strTableName = GetFieldsString(dbBaseMessage, pTableNameDesc);
		nOffset = GetFieldsString(dbBaseMessage, pOffsetDesc);
		nRows = GetFieldsString(dbBaseMessage, pRowsDesc);
		orderKey = GetFieldsString(dbBaseMessage, pOrderKeyDesc);
	}

	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE
			|| !pDbFieldsFieldDesc->is_repeated())
			return false;

		const google::protobuf::Descriptor* pDbFieldsDesc = pDbFieldsFieldDesc->message_type();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		const google::protobuf::FieldDescriptor* pDbCondsFieldDesc = pDesc->FindFieldByLowercaseName("db_cond");
		if (pDbCondsFieldDesc == nullptr || pDbCondsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbCondsMessage = pReflect->GetMessage(message, pDbCondsFieldDesc);

		const google::protobuf::Descriptor* pDbCondsDesc = dbCondsMessage.GetDescriptor();
		if (pDbCondsDesc == nullptr) return false;

		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConnection->query();
			query << "SELECT ";
			for (int i = 0; i < pDbFieldsDesc->field_count(); i++)
			{
				const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(i);
				if (pTemp == nullptr) return false;

				if (i == 0)
				{
					query << pTemp->name();
				}
				else
				{
					query << "," << pTemp->name();
				}
			}

			query << " FROM " << strTableName << " WHERE ";

			for (int i = 0; i < pDbCondsDesc->field_count(); i++)
			{
				const google::protobuf::FieldDescriptor* pTemp = pDbCondsDesc->field(i);
				if (pTemp == nullptr) return false;

				if (i == 0)
				{
					query << pTemp->name() << " = " << mysqlpp::quote << GetFieldsString(dbCondsMessage, pTemp);
				}
				else
				{
					query << " and " << pTemp->name() << " = " << mysqlpp::quote << GetFieldsString(dbCondsMessage, pTemp);
				}
			}

			int tempRows = lexical_cast<int>(nRows);
			if (tempRows > 0)
			{
				if (!orderKey.empty())
				{
					query << " ORDER BY " << orderKey << " limit " << lexical_cast<std::string>(nOffset) << "," << lexical_cast<std::string>(nRows) << ";";
				}
				else
				{
					query << " limit " << lexical_cast<std::string>(nOffset) << "," << lexical_cast<std::string>(nRows) << ";";
				}
			}
			else
			{
				query << ";";
			}

			//query.execute(); // 官网例子不需要execute
			mysqlpp::StoreQueryResult xResult = query.store();
			query.reset();

			if (xResult.empty() || !xResult)
			{
				return false;
			}

			// xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
			for (size_t i = 0; i < xResult.size(); ++i)
			{
				google::protobuf::Message* pDbFieldsMessage = pReflect->AddMessage(&message, pDbFieldsFieldDesc);

				for (size_t j = 0; j < pDbFieldsDesc->field_count(); ++j)
				{
					const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(j);
					if (pTemp == nullptr) return false;

					const std::string& strFieldName = pTemp->name();
					std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());

					SetFieldsString(*pDbFieldsMessage, pTemp, strValue);
				}
			}
			NFMYSQLTRYEND("query error")
	}

	return true;
}

bool NFCMysqlDriver::Updata(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}

	bool bExist = false;
	if (!Exists(strTableName, strKeyColName, strKey, bExist))
	{
		return false;
	}

	if (fieldVec.size() != valueVec.size())
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		if (bExist)
		{
			// update
			query << "UPDATE " << strTableName << " SET ";
			for (size_t i = 0; i < fieldVec.size(); ++i)
			{
				if (i == 0)
				{
					query << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i];
				}
				else
				{
					query << "," << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i];
				}
			}

			query << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey << ";";
		}
		else
		{
			// insert
			query << "INSERT INTO " << strTableName << "(" << strKeyColName << ",";
			for (size_t i = 0; i < fieldVec.size(); ++i)
			{
				if (i == 0)
				{
					query << fieldVec[i];
				}
				else
				{
					query << ", " << fieldVec[i];
				}
			}

			query << ") VALUES(" << mysqlpp::quote << strKey << ",";
			for (size_t i = 0; i < valueVec.size(); ++i)
			{
				if (i == 0)
				{
					query << mysqlpp::quote << valueVec[i];
				}
				else
				{
					query << ", " << mysqlpp::quote << valueVec[i];
				}
			}

			query << ");";
		}

		query.execute();
		query.reset();
		NFMYSQLTRYEND("update or insert error")

	return true;
}

bool NFCMysqlDriver::Query(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT ";
		for (std::vector<std::string>::const_iterator iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
		{
			if (iter == fieldVec.begin())
			{
				query << *iter;
			}
			else
			{
				query << "," << *iter;
			}
		}

		query << " FROM " << strTableName << " ORDER BY " << strKeyColName << " limit " <<  lexical_cast<std::string>(nOffset) << "," << lexical_cast<std::string>(nRows) << ";";

		//query.execute(); // 官网例子不需要execute
		mysqlpp::StoreQueryResult xResult = query.store();
		query.reset();

		if (xResult.empty() || !xResult)
		{
			return false;
		}

		// xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
		for (size_t i = 0; i < xResult.size(); ++i)
		{
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				valueVec.push_back(strValue);
			}
		}
		NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::Query(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT ";
		for (std::vector<std::string>::const_iterator iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
		{
			if (iter == fieldVec.begin())
			{
				query << *iter;
			}
			else
			{
				query << "," << *iter;
			}
		}

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey << ";";

		//query.execute(); // 官网例子不需要execute
		mysqlpp::StoreQueryResult xResult = query.store();
		query.reset();

		if (xResult.empty() || !xResult)
		{
			return false;
		}

		// xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
		for (size_t i = 0; i < xResult.size(); ++i)
		{
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				valueVec.push_back(strValue);
			}
		}
		NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "DELETE FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey << ";";

		query.execute();
		query.reset();
		NFMYSQLTRYEND("delete error")

	return true;
}

bool NFCMysqlDriver::Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT 1 FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey << " LIMIT 1;";

		//query.execute();
		mysqlpp::StoreQueryResult result = query.store();
		query.reset();

		if (!result || result.empty())
		{
			bExit = false;
			return true;
		}

		NFMYSQLTRYEND("exist error")

	bExit = true;
	return true;
}

bool NFCMysqlDriver::Keys(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKeyName, std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}

	const std::string strLikeKey = "%" + strKeyName + "%";

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT " << strKeyColName << " FROM " << strTableName << " WHERE " << strKeyColName << " LIKE " << mysqlpp::quote << strLikeKey << " LIMIT 100;";

		mysqlpp::StoreQueryResult xResult = query.store();
		query.reset();

		if (xResult.empty() || !xResult)
		{
			return false;
		}

		// xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
		for (size_t i = 0; i < xResult.size(); ++i)
		{
			std::string strValue(xResult[i][strKeyColName.data()].data(), xResult[i][strKeyColName.data()].length());
			valueVec.push_back(strValue);
		}

		NFMYSQLTRYEND("exist error")

	return true;
}

