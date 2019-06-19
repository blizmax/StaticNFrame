#include "NFCMysqlDriver.h"
#include "NFComm/NFCore/NFCommon.h"
#include "Dependencies/common/lexical_cast.hpp"
#include "NFComm/NFPluginModule/NFProtobufCommon.h"

//m_pMysqlConnect在调用Connect会引发多线程的崩溃，必须枷锁
NFMutex NFCMysqlDriver::ConnectLock;

NFCMysqlDriver::NFCMysqlDriver(const int nReconnectTime/* = 60*/, const int nReconnectCount /*= -1*/)
{
	mfCheckReconnect = 0.0f;
	mnDBPort = 0;
	m_pMysqlConnect = nullptr;
	mnReconnectTime = nReconnectTime;
	mnReconnectCount = nReconnectCount;
}

NFCMysqlDriver::NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort,
                               const std::string& strDBUser, const std::string& strDBPwd)
{
	mfCheckReconnect = 0.0f;
	mnDBPort = 0;
	m_pMysqlConnect = nullptr;

	mnReconnectTime = 60;
	mnReconnectCount = -1;

	NFCMysqlDriver::Connect(strDBName, strDBHost, nDBPort, strDBUser, strDBPwd);
}

NFCMysqlDriver::~NFCMysqlDriver()
{
	NFCMysqlDriver::CloseConnection();
}

bool NFCMysqlDriver::Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort,
                             const std::string& strDBUser, const std::string& strDBPwd)
{
	mstrDBName = strDBName;
	mstrDBHost = strDBHost;
	mnDBPort = nDBPort;
	mstrDBUser = strDBUser;
	mstrDBPwd = strDBPwd;

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

bool NFCMysqlDriver::Execute(const std::string& qstr, std::vector<std::map<std::string, std::string>>& valueVec)
{
	mysqlpp::StoreQueryResult queryResult;
	if (NFCMysqlDriver::Query(qstr, queryResult))
	{
		for (size_t i = 0; i < queryResult.num_rows(); ++i)
		{
			valueVec.push_back(std::map<std::string, std::string>());
			std::map<std::string, std::string>& tmpVec = valueVec.back();
			for(size_t index = 0; index < queryResult.num_fields(); index++)
			{
				const std::string& strFieldName = queryResult.field_name(i);
				std::string strValue(queryResult[i][strFieldName.data()].data(), queryResult[i][strFieldName.data()].length());
				tmpVec.emplace(strFieldName, strValue);
			}
			
		}
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
	m_pMysqlConnect = nullptr;
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

	if (nullptr == m_pMysqlConnect)
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
	if (nullptr == m_pMysqlConnect)
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
			if (!m_pMysqlConnect->connect(mstrDBName.c_str(), mstrDBHost.c_str(), mstrDBUser.c_str(), mstrDBPwd.c_str(),
			                              mnDBPort))
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

bool NFCMysqlDriver::Update(const google::protobuf::Message& message)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (pConnection == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	std::string strKeyName;
	std::string strKey;

	//处理db_base， 表名，表key

	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::
			CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

		const google::protobuf::Descriptor* pDbFieldsDesc = dbFieldsMessage.GetDescriptor();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		{
			const google::protobuf::FieldDescriptor* pKeyFieldDesc = pDbFieldsDesc->field(0);
			if (pKeyFieldDesc == nullptr) return false;

			strKeyName = pKeyFieldDesc->name();
			strKey = NFProtobufCommon::GetFieldsString(dbFieldsMessage, pKeyFieldDesc);
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
						query << pTemp->name() << " = " << mysqlpp::quote << NFProtobufCommon::GetFieldsString(
							dbFieldsMessage, pTemp);
					}
					else
					{
						query << "," << pTemp->name() << " = " << mysqlpp::quote << NFProtobufCommon::GetFieldsString(
							dbFieldsMessage, pTemp);
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
						query << mysqlpp::quote << NFProtobufCommon::GetFieldsString(
							dbFieldsMessage, pDbFieldsDesc->field(i));
					}
					else
					{
						query << ", " << mysqlpp::quote << NFProtobufCommon::GetFieldsString(
							dbFieldsMessage, pDbFieldsDesc->field(i));
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

	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	//处理db_base， 表名，表key
	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::
			CPPTYPE_MESSAGE) return false;

		const google::protobuf::Descriptor* pDbFieldsDesc = pDbFieldsFieldDesc->message_type();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		const google::protobuf::FieldDescriptor* pDbCondsFieldDesc = pDesc->FindFieldByLowercaseName("db_cond");
		if (pDbCondsFieldDesc == nullptr || pDbCondsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::
			CPPTYPE_MESSAGE) return false;

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

			uint32_t count = 0;
			for (int i = 0; i < pDbCondsDesc->field_count(); i++)
			{
				const google::protobuf::FieldDescriptor* pTemp = pDbCondsDesc->field(i);
				if (pTemp == nullptr) return false;

				std::string fValue = NFProtobufCommon::GetFieldsString(dbCondsMessage, pTemp);
				if (fValue.empty() == false)
				{
					if (count == 0)
					{
						query << pTemp->name() << " = " << mysqlpp::quote << fValue;
					}
					else
					{
						query << " and " << pTemp->name() << " = " << mysqlpp::quote << fValue;
					}
					count++;
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
					std::string strValue(xResult[i][strFieldName.data()].data(),
					                     xResult[i][strFieldName.data()].length());

					NFProtobufCommon::SetFieldsString(*pDbFieldsMessage, pTemp, strValue);
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
		if (pDbBaseFieldDesc == nullptr || pDbBaseFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::
			CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbBaseMessage = pReflect->GetMessage(message, pDbBaseFieldDesc);

		const google::protobuf::Descriptor* pDbBaseDesc = dbBaseMessage.GetDescriptor();
		if (pDbBaseDesc == nullptr) return false;

		const google::protobuf::Reflection* pDbBaseReflect = dbBaseMessage.GetReflection();
		if (pDbBaseReflect == nullptr) return false;

		const google::protobuf::FieldDescriptor* pTableNameDesc = pDbBaseDesc->FindFieldByLowercaseName("table_name");
		if (pTableNameDesc == nullptr || pTableNameDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING
		) return false;

		const google::protobuf::FieldDescriptor* pOffsetDesc = pDbBaseDesc->FindFieldByLowercaseName("offset");
		if (pOffsetDesc == nullptr || pOffsetDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_UINT32)
			return false;

		const google::protobuf::FieldDescriptor* pRowsDesc = pDbBaseDesc->FindFieldByLowercaseName("rows");
		if (pRowsDesc == nullptr || pRowsDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_UINT32) return
			false;

		const google::protobuf::FieldDescriptor* pOrderKeyDesc = pDbBaseDesc->FindFieldByLowercaseName("order_key");
		if (pOrderKeyDesc == nullptr || pOrderKeyDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING)
			return false;

		strTableName = NFProtobufCommon::GetFieldsString(dbBaseMessage, pTableNameDesc);
		nOffset = NFProtobufCommon::GetFieldsString(dbBaseMessage, pOffsetDesc);
		nRows = NFProtobufCommon::GetFieldsString(dbBaseMessage, pRowsDesc);
		orderKey = NFProtobufCommon::GetFieldsString(dbBaseMessage, pOrderKeyDesc);
	}

	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::
			CPPTYPE_MESSAGE
			|| !pDbFieldsFieldDesc->is_repeated())
			return false;

		const google::protobuf::Descriptor* pDbFieldsDesc = pDbFieldsFieldDesc->message_type();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		const google::protobuf::FieldDescriptor* pDbCondsFieldDesc = pDesc->FindFieldByLowercaseName("db_cond");
		if (pDbCondsFieldDesc == nullptr || pDbCondsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::
			CPPTYPE_MESSAGE) return false;

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

			uint32_t count = 0;
			for (int i = 0; i < pDbCondsDesc->field_count(); i++)
			{
				const google::protobuf::FieldDescriptor* pTemp = pDbCondsDesc->field(i);
				if (pTemp == nullptr) return false;

				std::string fValue = NFProtobufCommon::GetFieldsString(dbCondsMessage, pTemp);
				if (fValue.empty() == false)
				{
					if (count == 0)
					{
						query << pTemp->name() << " = " << mysqlpp::quote << fValue;
					}
					else
					{
						query << " and " << pTemp->name() << " = " << mysqlpp::quote << fValue;
					}
					count++;
				}
			}

			int tempRows = lexical_cast<int>(nRows);
			if (tempRows > 0)
			{
				if (!orderKey.empty())
				{
					query << " ORDER BY " << orderKey << " limit " << lexical_cast<std::string>(nOffset) << "," <<
						lexical_cast<std::string>(nRows) << ";";
				}
				else
				{
					query << " limit " << lexical_cast<std::string>(nOffset) << "," << lexical_cast<std::string>(nRows)
						<< ";";
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
					std::string strValue(xResult[i][strFieldName.data()].data(),
					                     xResult[i][strFieldName.data()].length());

					NFProtobufCommon::SetFieldsString(*pDbFieldsMessage, pTemp, strValue);
				}
			}
		NFMYSQLTRYEND("query error")
	}

	return true;
}

bool NFCMysqlDriver::Update(const std::string& strTableName, const std::string& strKeyColName,
                            const std::string& strKey, const std::vector<std::string>& fieldVec,
                            const std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

bool NFCMysqlDriver::Update(const std::string& strTableName, const std::string& strKeyColName,
                            const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
	{
		return false;
	}

	bool bExist = false;
	if (!Exists(strTableName, strKeyColName, strKey, bExist))
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		if (bExist)
		{
			// update
			query << "UPDATE " << strTableName << " SET ";
			int i = 0;
			for (auto iter = keyvalueMap.begin(); iter != keyvalueMap.end(); ++iter)
			{
				if (i == 0)
				{
					query << iter->first << " = " << mysqlpp::quote << iter->second;
				}
				else
				{
					query << "," << iter->first << " = " << mysqlpp::quote << iter->second;
				}
				i++;
			}

			query << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey << ";";
		}
		else
		{
			// insert
			query << "INSERT INTO " << strTableName << "(" << strKeyColName << ",";
			int i = 0;
			for (auto iter = keyvalueMap.begin(); iter != keyvalueMap.end(); ++iter)
			{
				if (i == 0)
				{
					query << iter->first;
				}
				else
				{
					query << ", " << iter->first;
				}
				i++;
			}

			query << ") VALUES(" << mysqlpp::quote << strKey << ",";
			i = 0;
			for (auto iter = keyvalueMap.begin(); iter != keyvalueMap.end(); ++iter)
			{
				if (i == 0)
				{
					query << mysqlpp::quote << iter->second;
				}
				else
				{
					query << ", " << mysqlpp::quote << iter->second;
				}
				i++;
			}

			query << ");";
		}

		query.execute();
		query.reset();
	NFMYSQLTRYEND("update or insert error")

	return true;
}

bool NFCMysqlDriver::QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql,
                                       const std::vector<std::string>& fieldVec,
                                       std::vector<std::map<std::string, std::string>>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

		query << " FROM " << strTableName << " where " << strWhereSql << ";";

		//query.execute(); // 官网例子不需要execute
		mysqlpp::StoreQueryResult xResult = query.store();
		query.reset();

		if (xResult.empty() || !xResult)
		{
			return false;
		}

		//
		for (size_t i = 0; i < xResult.size(); ++i)
		{
			valueVec.push_back(std::map<std::string, std::string>());
			std::map<std::string, std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.emplace(strFieldName, strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
                                        int nRows, const std::vector<std::string>& fieldVec,
                                        std::vector<std::vector<std::string>>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

		query << " FROM " << strTableName << " ORDER BY " << strKeyColName << " limit " << lexical_cast<std::string
		>(nOffset) << "," << lexical_cast<std::string>(nRows) << ";";

		//query.execute(); // 官网例子不需要execute
		mysqlpp::StoreQueryResult xResult = query.store();
		query.reset();

		if (xResult.empty() || !xResult)
		{
			return false;
		}

		//
		for (size_t i = 0; i < xResult.size(); ++i)
		{
			valueVec.push_back(std::vector<std::string>());
			std::vector<std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.push_back(strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset,
                                        int nRows, const std::vector<std::string>& fieldVec,
                                        std::vector<std::map<std::string, std::string>>& valueVec)
{
	valueVec.clear();
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

		query << " FROM " << strTableName << " ORDER BY " << strKeyColName << " limit " << lexical_cast<std::string
		>(nOffset) << "," << lexical_cast<std::string>(nRows) << ";";

		//query.execute(); // 官网例子不需要execute
		mysqlpp::StoreQueryResult xResult = query.store();
		query.reset();

		if (xResult.empty() || !xResult)
		{
			return false;
		}

		for (size_t i = 0; i < xResult.size(); ++i)
		{
			valueVec.push_back(std::map<std::string, std::string>());
			std::map<std::string, std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.emplace(strFieldName, strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::QueryOne(const std::string& strTableName, const std::string& strKeyColName,
                              const std::string& strKey, const std::vector<std::string>& fieldVec,
                              std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey <<
			" limit 1;";

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

bool NFCMysqlDriver::QueryOne(const std::string& strTableName, const std::string& strKeyColName,
                              const std::string& strKey, const std::vector<std::string>& fieldVec,
                              std::map<std::string, std::string>& valueVec)
{
	valueVec.clear();
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey <<
			" limit 1;";

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
				valueVec.emplace(strFieldName, strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::QueryMore(const std::string& strTableName, const std::string& strKeyColName,
                               const std::string& strKey, const std::vector<std::string>& fieldVec,
                               std::vector<std::vector<std::string>>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT ";
		for (auto iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
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

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey <<
			" ORDER BY " << strKeyColName << ";";

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
			valueVec.push_back(std::vector<std::string>());
			std::vector<std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.push_back(strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::QueryMore(const std::string& strTableName, const std::string& strKeyColName,
                               const std::string& strKey, const std::vector<std::string>& fieldVec,
                               std::vector<std::map<std::string, std::string>>& valueVec)
{
	valueVec.clear();
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
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

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey <<
			" ORDER BY " << strKeyColName << ";";

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
			valueVec.push_back(std::map<std::string, std::string>());
			std::map<std::string, std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.emplace(strFieldName, strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::Delete(const std::string& strTableName, const std::string& strKeyColName,
                            const std::string& strKey)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "DELETE FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey <<
			";";

		query.execute();
		query.reset();
	NFMYSQLTRYEND("delete error")

	return true;
}

bool NFCMysqlDriver::Exists(const std::string& strTableName, const std::string& strKeyColName,
                            const std::string& strKey, bool& bExit)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
	{
		return false;
	}

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT 1 FROM " << strTableName << " WHERE " << strKeyColName << " = " << mysqlpp::quote << strKey <<
			" LIMIT 1;";

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

bool NFCMysqlDriver::QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
                                     const std::string& strKeyName, const std::vector<std::string>& fieldVec,
                                     std::vector<std::vector<std::string>>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
	{
		return false;
	}

	const std::string strLikeKey = "%" + strKeyName + "%";

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT ";
		for (auto iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
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

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " LIKE " << mysqlpp::quote << strLikeKey <<
			";";

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
			valueVec.push_back(std::vector<std::string>());
			std::vector<std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.push_back(strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlDriver::QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName,
                                     const std::string& strKeyName, const std::vector<std::string>& fieldVec,
                                     std::vector<std::map<std::string, std::string>>& valueVec)
{
	valueVec.clear();
	mysqlpp::Connection* pConnection = GetConnection();
	if (nullptr == pConnection)
	{
		return false;
	}

	const std::string strLikeKey = "%" + strKeyName + "%";

	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
		query << "SELECT ";
		for (auto iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
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

		query << " FROM " << strTableName << " WHERE " << strKeyColName << " LIKE " << mysqlpp::quote << strLikeKey <<
			";";

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
			valueVec.push_back(std::map<std::string, std::string>());
			std::map<std::string, std::string>& tmpVec = valueVec.back();
			for (size_t j = 0; j < fieldVec.size(); ++j)
			{
				const std::string& strFieldName = fieldVec[j];
				std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
				tmpVec.emplace(strFieldName, strValue);
			}
		}
	NFMYSQLTRYEND("query error")

	return true;
}
