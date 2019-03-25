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
	pPluginManager = p;
	m_pSqliteDriverManager = NF_NEW NFCSqliteDriverManager();
}

NFCSqliteModule::~NFCSqliteModule()
{
	NF_SAFE_DELETE(m_pSqliteDriverManager);
}

bool NFCSqliteModule::Init()
{
	m_pSqliteDriverManager->AddSqliteServer(0, "sqlite/test.db");
	auto pDriver = m_pSqliteDriverManager->GetSqliteDriver();
	if (pDriver)
	{
		pDriver->CreateTable(proto::message::plane_record_info::default_instance());

		uint64_t beginTime = NFGetTime();
		std::vector<google::protobuf::Message*> vecInfo;
		for (int i = 0; i < 100000; i++)
		{
			proto::message::plane_record_info* pInfo = new proto::message::plane_record_info();
			pInfo->mutable_db_fields()->set_id(i + 1);
			vecInfo.push_back(pInfo);
		}
		pDriver->InsertMessage(vecInfo, false);

		std::cout << "use time:" << NFGetTime() - beginTime << " ms" << std::endl;
	}
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
	return true;
}

/**
* @brief 打开数据库
*
* @return bool
*/
bool NFCSqliteModule::AddSqliteServer(uint32_t nServerID, const std::string& dbname)
{
	return m_pSqliteDriverManager->AddSqliteServer(nServerID, dbname);
}

/**
* @brief 通过protobuf反射， 来实现sqlite的建表
*
* @param  message
* @param  firstKey 是否把第一列作为KEY
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
* @brief 创建数据库表
*
* @param  tableName 数据库名字
* @param  columnSql 列字符串 类似"ServerID int, UserDBID bigint"
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
* @brief 执行一条语句
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
* @brief 执行N条语句
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
* @brief 执行一条插入语句
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
* @brief 执行N条插入语句
*
* @return void
*/
bool NFCSqliteModule::InsertMessage(uint32_t nServerID, std::vector<google::protobuf::Message*>& vecMessage, bool insertKey)
{
	NFCSqliteDriver* pDriver = m_pSqliteDriverManager->GetSqliteDriver(nServerID);
	if (pDriver)
	{
		return pDriver->InsertMessage(vecMessage, insertKey);
	}
	return false;
}