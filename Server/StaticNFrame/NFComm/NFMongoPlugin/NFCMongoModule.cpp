// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/05
//    @Email			:    445267987@qq.com
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMongoModule.h"
#include "NFCMongoDriverManager.h"

NFCMongoModule::NFCMongoModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mnLastCheckTime = 0;
	m_pMongoDriverManager = NF_NEW NFCMongoDriverManager();
}

NFCMongoModule::~NFCMongoModule()
{
	NF_SAFE_DELETE(m_pMongoDriverManager);
}

bool NFCMongoModule::Init()
{
	return true;
}

bool NFCMongoModule::Shut()
{
	return true;
}

bool NFCMongoModule::AfterInit()
{
	return true;
}

bool NFCMongoModule::Execute()
{
	if (mnLastCheckTime + 10 > (uint64_t)NFGetSecondTime())
	{
		return true;
	}

	mnLastCheckTime = (uint64_t)NFGetSecondTime();

	if (m_pMongoDriverManager)
	{
		m_pMongoDriverManager->CheckMongo();
	}

	return true;
}

bool NFCMongoModule::AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->AddMongoServer(nServerID, ip, port, dbname);
	}
	return false;
}

bool NFCMongoModule::IsExistCollection(const int nServerID, const std::string& collectionName)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->IsExistCollection(nServerID, collectionName);
	}
	return false;
}

bool NFCMongoModule::CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->CreateCollection(nServerID, collectionName, key);
	}
	return false;
}

bool NFCMongoModule::DropCollection(const int nServerID, const std::string& collectionName)
{
	if (m_pMongoDriverManager)
	{
		return m_pMongoDriverManager->DropCollection(nServerID, collectionName);
	}
	return false;
}