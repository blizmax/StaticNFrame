// -------------------------------------------------------------------------
//    @FileName         :    NFProtobufCommon.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/3/2
//    @Email			:    445267987@qq.com
//    @Module           :    NFProtobufCommon
//
// -------------------------------------------------------------------------

#include "NFProtobufCommon.h"
#include "NFLogMgr.h"
#include "common/lexical_cast.hpp"
#include "NFIObject.h"

bool NFProtobufCommon::NFObjectToMessage(NFIObject* pObject, google::protobuf::Message& message)
{
	if (pObject == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	for (int i = 0; i < pDesc->field_count(); i++)
	{
		const google::protobuf::FieldDescriptor* pFieldDesc = pDesc->field(i);
		if (pFieldDesc == nullptr) return false;
		switch (pFieldDesc->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32_t value = pObject->GetNodeInt32(pFieldDesc->name());
			pReflect->SetInt32(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64_t value = pObject->GetNodeInt64(pFieldDesc->name());
			pReflect->SetInt64(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32_t value = pObject->GetNodeUInt32(pFieldDesc->name());
			pReflect->SetUInt32(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64_t value = pObject->GetNodeUInt64(pFieldDesc->name());
			pReflect->SetUInt64(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double value = pObject->GetNodeDouble(pFieldDesc->name());
			pReflect->SetDouble(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			float value = pObject->GetNodeFloat(pFieldDesc->name());
			pReflect->SetFloat(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			bool value = pObject->GetNodeBool(pFieldDesc->name());
			pReflect->SetBool(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			int32_t value = pObject->GetNodeInt32(pFieldDesc->name());
			pReflect->SetEnumValue(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			std::string value = pObject->GetNodeString(pFieldDesc->name());
			pReflect->SetString(&message, pFieldDesc, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			std::string value = pObject->GetNodeString(pFieldDesc->name());
			google::protobuf::Message* pMessageValue = pReflect->MutableMessage(&message, pFieldDesc);
			pMessageValue->ParseFromString(value);
		}
		break;
		default:
			break;
		}
	}

	return true;
}

bool NFProtobufCommon::NFObjectFromMessage(NFIObject* pObject, const google::protobuf::Message& message)
{
	if (pObject == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	for (int i = 0; i < pDesc->field_count(); i++)
	{
		const google::protobuf::FieldDescriptor* pFieldDesc = pDesc->field(i);
		if (pFieldDesc == nullptr) return false;
		switch (pFieldDesc->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32_t value = pReflect->GetInt32(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_INT, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64_t value = pReflect->GetInt64(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_INT, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32_t value = pReflect->GetUInt32(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_INT, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64_t value = pReflect->GetUInt64(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_INT, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double value = pReflect->GetDouble(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_DOUBLE, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			float value = pReflect->GetFloat(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_DOUBLE, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			bool value = pReflect->GetBool(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_BOOLEAN, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			const google::protobuf::EnumValueDescriptor* pEnumDesc = pReflect->GetEnum(message, pFieldDesc);
			if (pEnumDesc)
			{
				pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_INT, pEnumDesc->number()), 0);
			}
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			std::string value = pReflect->GetString(message, pFieldDesc);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_STRING, value), 0);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			const google::protobuf::Message& value = pReflect->GetMessage(message, pFieldDesc);
			std::string msg;
			value.SerializePartialToString(&msg);
			pObject->AddNode(pFieldDesc->name(), NFCData(NF_DT_STRING, msg), 0);
		}
		break;
		default:
			break;
		}
	}

	return true;
}

bool NFProtobufCommon::NFObjectAddTableFromMessage(NFIObject* pObject, const std::string& tableName, const google::protobuf::Message& message)
{
	if (pObject == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::vector<int> vecColType;
	for (int i = 0; i < pDesc->field_count(); i++)
	{
		const google::protobuf::FieldDescriptor* pFieldDesc = pDesc->field(i);
		if (pFieldDesc == nullptr) return false;
		switch (pFieldDesc->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			vecColType.push_back(NF_DT_INT);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			vecColType.push_back(NF_DT_INT);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			vecColType.push_back(NF_DT_INT);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			vecColType.push_back(NF_DT_INT);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			vecColType.push_back(NF_DT_DOUBLE);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			vecColType.push_back(NF_DT_DOUBLE);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			vecColType.push_back(NF_DT_BOOLEAN);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			vecColType.push_back(NF_DT_INT);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			vecColType.push_back(NF_DT_STRING);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			vecColType.push_back(NF_DT_STRING);
		}
		break;
		default:
			break;
		}
	}

	pObject->AddTable(pObject->GetObjectId(), tableName, vecColType, 0);
	return true;
}

bool NFProtobufCommon::NFObjectAddTableFromMessage(NFIObject* pObject, const google::protobuf::Message& message)
{
	if (pObject == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
	if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

	const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

	return NFObjectAddTableFromMessage(pObject, strTableName, dbFieldsMessage);
}

bool NFProtobufCommon::NFObjectAddTableRowsFromMessage(NFIObject* pObject, const std::string& strTableName, const google::protobuf::Message& message)
{
	if (pObject == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	int curRow = pObject->AddTableRow(strTableName);

	for (int i = 0; i < pDesc->field_count(); i++)
	{
		const google::protobuf::FieldDescriptor* pFieldDesc = pDesc->field(i);
		if (pFieldDesc == nullptr) return false;
		switch (pFieldDesc->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32_t value = pReflect->GetInt32(message, pFieldDesc);
			pObject->SetTableInt32(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64_t value = pReflect->GetInt64(message, pFieldDesc);
			pObject->SetTableInt64(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32_t value = pReflect->GetUInt32(message, pFieldDesc);
			pObject->SetTableUInt32(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64_t value = pReflect->GetUInt64(message, pFieldDesc);
			pObject->SetTableUInt32(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double value = pReflect->GetDouble(message, pFieldDesc);
			pObject->SetTableDouble(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			float value = pReflect->GetFloat(message, pFieldDesc);
			pObject->SetTableFloat(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			bool value = pReflect->GetBool(message, pFieldDesc);
			pObject->SetTableBool(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			const google::protobuf::EnumValueDescriptor* pEnumDesc = pReflect->GetEnum(message, pFieldDesc);
			if (pEnumDesc)
			{
				pObject->SetTableInt32(strTableName, curRow, i, pEnumDesc->number());
			}
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			std::string value = pReflect->GetString(message, pFieldDesc);
			pObject->SetTableString(strTableName, curRow, i, value);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			const google::protobuf::Message& value = pReflect->GetMessage(message, pFieldDesc);
			std::string msg;
			value.SerializePartialToString(&msg);
			pObject->SetTableString(strTableName, curRow, i, msg);
		}
		break;
		default:
			break;
		}
	}
	return true;
}

bool NFProtobufCommon::NFObjectAddTableRowsFromMessage(NFIObject* pObject, const google::protobuf::Message& message)
{
	if (pObject == nullptr) return false;

	NFProtobufCommon::NFObjectAddTableFromMessage(pObject, message);

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;

	const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
	if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

	if (pDbFieldsFieldDesc->is_repeated() == false)
	{
		const google::protobuf::Message& repeatedMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);
		NFProtobufCommon::NFObjectAddTableRowsFromMessage(pObject, strTableName, repeatedMessage);
		return true;
	}

	int field_size = pReflect->FieldSize(message, pDbFieldsFieldDesc);
	for (int i = 0; i < field_size; i++)
	{
		const google::protobuf::Message& repeatedMessage = pReflect->GetRepeatedMessage(message, pDbFieldsFieldDesc, i);

		NFProtobufCommon::NFObjectAddTableRowsFromMessage(pObject, strTableName, repeatedMessage);
	}
	
	return true;
}

bool NFProtobufCommon::MessageToJsonString(const google::protobuf::Message& msg, std::string& json)
{
	google::protobuf::util::JsonPrintOptions options;
	options.always_print_enums_as_ints = true;
	options.preserve_proto_field_names = true;

	google::protobuf::util::Status status = google::protobuf::util::MessageToJsonString(msg, &json, options);
	if (!status.ok())
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "MessageToJsonString Error | {}", status.error_message().ToString());
	}
	return status.ok();
}

bool NFProtobufCommon::JsonStringToMessage(const string& json, google::protobuf::Message& msg)
{
	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;
	google::protobuf::util::Status status = google::protobuf::util::JsonStringToMessage(json, &msg, options);
	if (!status.ok())
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "JsonStringToMessage Error | {}", status.error_message().ToString());
	}
	return status.ok();
}

std::string NFProtobufCommon::GetFieldsString(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc)
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

void NFProtobufCommon::SetFieldsString(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc, const std::string& strValue)
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

/*
** sqlite,mysql从message利用反射取出数据库名字
*/
/*如果没有db_base结构，就认为第一列是表名，且是一个字符串
** 比如
message create_sqlite_table
{
	optional std::string table_name = 1;
}

如果字符串为空，就将域名当成表名
create_sqlite_table tableUser;
tableUser.set_table_name("");
这个时候table_name就是表名

如果：
create_sqlite_table tableUser;
tableUser.set_table_name("gaoyi");
那么gaoyi就是表名

** 如果db_base结构存在， db_base的第一列必须是一个字符串，
message message_sqlite_base
{
	optional string table_name = 1; //数据库表名
}

message create_sqlite_table
{
	optional message_sqlite_base db_base = 1;
}

如果db_base第一列table_name有值，那么table_name的值就是表名，不然table_name本身是表名字

message message_sqlite_base
{
optional string xx_table = 1; //数据库表名
}

message create_sqlite_table
{
	optional message_sqlite_base db_base = 1;
}

xx_table为空，那么xx_table就是表名，可以避免赋值
*/
std::string NFProtobufCommon::GetDBNameFromMessage(const google::protobuf::Message& message)
{
	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return std::string();

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return std::string();

	const google::protobuf::FieldDescriptor* pDbBaseFieldDesc = pDesc->FindFieldByLowercaseName("db_base");
	if (pDbBaseFieldDesc == nullptr)
	{
		if (pDesc->field_count() <= 0)
		{
			return std::string();
		}

		//取第一列
		const google::protobuf::FieldDescriptor* pTableNameFieldDesc = pDesc->field(0);
		if (pTableNameFieldDesc == nullptr || pTableNameFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING)
		{
			return std::string();
		}
		
		std::string fieldName = pTableNameFieldDesc->name();
		std::string fieldValue = pReflect->GetString(message, pTableNameFieldDesc);
		if (fieldValue.empty())
		{
			return fieldName;
		}
		return fieldValue;
	}

	if (pDbBaseFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE)
	{
		return std::string();
	}

	{
		const google::protobuf::Message& dbBaseMessage = pReflect->GetMessage(message, pDbBaseFieldDesc);

		const google::protobuf::Descriptor* pDbBaseDesc = dbBaseMessage.GetDescriptor();
		if (pDbBaseDesc == nullptr) return std::string();

		const google::protobuf::Reflection* pDbBaseReflect = dbBaseMessage.GetReflection();
		if (pDbBaseReflect == nullptr) return std::string();

		if (pDbBaseDesc->field_count() <= 0) return std::string();

		//取db_base结构的第一列
		const google::protobuf::FieldDescriptor* pTableNameDesc = pDbBaseDesc->field(0);
		if (pTableNameDesc == nullptr || pTableNameDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_STRING) return std::string();

		std::string fieldName = pTableNameDesc->name();
		std::string fieldValue = pDbBaseReflect->GetString(dbBaseMessage, pTableNameDesc);
		if (fieldValue.empty())
		{
			return fieldName;
		}
		return fieldValue;
	}
}

std::string  NFProtobufCommon::GetSqliteColumnFromMessage(const google::protobuf::Message& message, bool firstKey)
{
	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return std::string();

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return std::string();

	std::string columnSql;
	for (int i = 0; i < pDesc->field_count(); i++)
	{
		const google::protobuf::FieldDescriptor* pFieldDesc = pDesc->field(i);
		std::string columnStr = pFieldDesc->name() + " ";
		if (i == 0 && firstKey)
		{
			if (pFieldDesc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT32 ||
				pFieldDesc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_UINT32 ||
				pFieldDesc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT64 ||
				pFieldDesc->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_UINT64)
			{
				columnStr += " INTEGER PRIMARY KEY AUTOINCREMENT,";
				columnSql += columnStr;
				continue;
			}
		}

		switch (pFieldDesc->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			columnStr += "int";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			columnStr += "bigint";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			columnStr += "int";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			columnStr += "bigint";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			columnStr += "real";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			columnStr += "real";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			columnStr += "int";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			columnStr += "int";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			columnStr += "text";
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
		{
			columnStr += "blob";
		}
		break;
		default:
			break;
		}

		if (i == 0 && firstKey)
		{
			columnStr += " PRIMARY KEY,";
		}

		columnSql += columnStr;
		if (i != pDesc->field_count() - 1)
		{
			columnSql += ",";
		}
	}

	return columnSql;
}

/*
** sqlite从message利用反射,将message的field name生成prepare需要的语句,比如
** insert into UserInOutRecord values(?,?,?,?,?,?)
*/
std::string NFProtobufCommon::GetSqlitePrepareFromMessage(const google::protobuf::Message& message)
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

	const google::protobuf::Descriptor* pFieldsMessageDesc = dbFieldsMessage.GetDescriptor();
	if (pFieldsMessageDesc == nullptr) return std::string();

	std::string sql = "insert into " + strTableName + " values(";
	for (int i = 0; i < pFieldsMessageDesc->field_count(); i++)
	{
		sql += "?";
		if (i != (pFieldsMessageDesc->field_count() - 1))
		{
			sql += ",";
		}
	}

	sql += ")";
	return sql;
}
