// -------------------------------------------------------------------------
//    @FileName         :    NFCSqliteModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-25
//    @Module           :    NFSqlitePlugin
//
// -------------------------------------------------------------------------

#include "NFCSqliteModule.h"

NFCSqliteModule::NFCSqliteModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	m_pSqliteDriverManager = NF_NEW NFCSqliteDriverManager();
}

NFCSqliteModule::~NFCSqliteModule()
{

}

bool NFCSqliteModule::Init()
{
	/*
	AddSqliteServer(0, "sqlite/test.db");
	CreateTable(0,proto::message::plane_record_info::default_instance());

	uint64_t beginTime = NFGetTime();
	std::vector<google::protobuf::Message*> vecInfo;
	for (int i = 0; i < 400000; i++)
	{
		proto::message::plane_record_info* pInfo = new proto::message::plane_record_info();
		pInfo->mutable_db_fields()->set_id(i + 1);
		vecInfo.push_back(pInfo);
	}
	InsertDifferentMessage(0, vecInfo);

	std::cout << "use time:" << NFGetTime() - beginTime << " ms" << std::endl;
	*/

	return true;
}

bool NFCSqliteModule::AfterInit()
{
	return true;
}

bool NFCSqliteModule::Execute()
{
	return true;
}

bool NFCSqliteModule::Shut()
{
	return true;
}

bool NFCSqliteModule::Finalize()
{
	NF_SAFE_DELETE(m_pSqliteDriverManager);
	return true;
}

/**
* @brief �����ݿ�
*
* @return bool
*/
bool NFCSqliteModule::AddSqliteServer(uint32_t nServerID, const std::string& dbname)
{
	return m_pSqliteDriverManager->AddSqliteServer(nServerID, dbname);
}

/**
* @brief ͨ��protobuf���䣬 ��ʵ��sqlite�Ľ���
*
* @param  message
* @param  firstKey �Ƿ�ѵ�һ����ΪKEY
* @return bool
*/
bool NFCSqliteModule::CreateTable(uint32_t nServerID, const google::protobuf::Message& message, bool firstKey)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		return pDriver->CreateTable(message, firstKey);
	}
	return false;
}

/**
* @brief �������ݿ��
*
* @param  tableName ���ݿ�����
* @param  columnSql ���ַ��� ����"ServerID int, UserDBID bigint"
* @return bool
*/
bool NFCSqliteModule::CreateTable(uint32_t nServerID, const std::string& tableName, const std::string& columnSql)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		return pDriver->CreateTable(tableName, columnSql);
	}
	return false;
}

/**
* @brief ִ��һ�����
*
* @return void
*/
bool NFCSqliteModule::ExecSql(uint32_t nServerID, const std::string& sql)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		return pDriver->ExecSql(sql);
	}
	return false;
}

/**
* @brief ִ��N�����
*
* @return void
*/
bool NFCSqliteModule::ExecSql(uint32_t nServerID, const std::vector<std::string>& sql)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		return pDriver->ExecSql(sql);
	}
	return false;
}

/**
* @brief ִ��һ���������
*
* @return void
*/
bool NFCSqliteModule::InsertMessage(uint32_t nServerID, const google::protobuf::Message& message, bool insertKey)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		return pDriver->InsertMessage(message, insertKey);
	}
	return false;
}

/**
* @brief ִ��N���������
*
* @return void
*/
bool NFCSqliteModule::InsertMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		bool result = pDriver->InsertMessage(vecMessage, insertKey);

		for (size_t i = 0; i < vecMessage.size(); i++)
		{
			NF_SAFE_DELETE(vecMessage[i]);
		}
		vecMessage.clear();
		return result;
	}
	return false;
}

/**
* @brief ִ��N�����ܲ�ͬ��Ĳ�ͬ�������
*
* @return void
*/
bool NFCSqliteModule::InsertDifferentMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		std::map<std::string, std::vector<google::protobuf::Message*>> vecMap;
		for (size_t i = 0; i < vecMessage.size(); i++)
		{
			vecMap[vecMessage[i]->GetTypeName()].push_back(vecMessage[i]);
		}

		for (auto iter = vecMap.begin(); iter != vecMap.end(); iter++)
		{
			bool result = pDriver->InsertMessage(iter->second, insertKey);
			if (!result)
			{
				return result;
			}
		}

		for (size_t i = 0; i < vecMessage.size(); i++)
		{
			NF_SAFE_DELETE(vecMessage[i]);
		}
		vecMessage.clear();
		return true;
	}
	return false;
}