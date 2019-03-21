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
bool NFCSqliteDriver::CreateTable(const google::protobuf::Message& message)
{
	if (m_pSqliteDB == nullptr) return false;

	const google::protobuf::Descriptor* pDesc = message.GetDescriptor();
	if (pDesc == nullptr) return false;

	const google::protobuf::Reflection* pReflect = message.GetReflection();
	if (pReflect == nullptr) return false;

	//��ñ������
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