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
bool NFCSqliteDriver::CreateTable(const google::protobuf::Message& message)
{
	if (m_pSqliteDB == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	//获得表的名字
	std::string strTableName = NFProtobufCommon::GetDBNameFromMessage(message);
	if (strTableName.empty()) return false;
	
	std::string strKeyName;

	do
	{
		const google::protobuf::FieldDescriptor* pDbFieldsFieldDesc = pDesc->FindFieldByLowercaseName("db_fields");
		if (pDbFieldsFieldDesc == nullptr || pDbFieldsFieldDesc->cpp_type() != google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) return false;

		const google::protobuf::Message& dbFieldsMessage = pReflect->GetMessage(message, pDbFieldsFieldDesc);

		const google::protobuf::Descriptor* pDbFieldsDesc = dbFieldsMessage.GetDescriptor();
		if (pDbFieldsDesc == nullptr) return false;

		if (pDbFieldsDesc->field_count() <= 0) return false;

		const google::protobuf::FieldDescriptor* pKeyFieldDesc = pDbFieldsDesc->field(0);
		if (pKeyFieldDesc == nullptr) return false;

		strKeyName = pKeyFieldDesc->name();

		for (int i = 0; i < pDbFieldsDesc->field_count(); ++i)
		{
			const google::protobuf::FieldDescriptor* pTemp = pDbFieldsDesc->field(i);
			if (pTemp == nullptr) return false;
		}
	} while (false);

	return true;
}