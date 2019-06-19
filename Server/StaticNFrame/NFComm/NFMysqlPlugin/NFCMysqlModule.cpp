// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/05
//    @Email			:    445267987@qq.com
//    @Module           :    NFMysqlPlugin
//
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlModule.h"
#include "NFCMysqlDriverManager.h"

NFCMysqlModule::NFCMysqlModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mnLastCheckTime = 0;
	m_pMysqlDriverManager = NF_NEW NFCMysqlDriverManager();
}

NFCMysqlModule::~NFCMysqlModule()
{
	NF_SAFE_DELETE(m_pMysqlDriverManager);
}

bool NFCMysqlModule::Init()
{
	this->SetTimer(0, 10000, INFINITY_CALL);
	return true;
}

bool NFCMysqlModule::Shut()
{
	return true;
}

bool NFCMysqlModule::AfterInit()
{
	return true;
}

bool NFCMysqlModule::UpdateOne(const google::protobuf::Message& message)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Update(message);
	}

	return false;
}

bool NFCMysqlModule::QueryOne(google::protobuf::Message& message)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Query(message);
	}

	return false;
}

bool NFCMysqlModule::QueryMore(google::protobuf::Message& message)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMore(message);
	}

	return false;
}

bool NFCMysqlModule::Execute(const std::string& qstr)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Execute(qstr);
	}

	return false;
}

bool NFCMysqlModule::UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Update(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::UpdateOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::map<std::string, std::string>& keyvalueMap)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Update(strTableName, strKeyColName, strKey, keyvalueMap);
	}

	return false;
}

bool NFCMysqlModule::QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryOne(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMore(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryMoreWithCond(const std::string& strTableName, const std::string& strWhereSql, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMoreWithCond(strTableName, strWhereSql, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMoreWithLimit(strTableName, strKeyColName, nOffset, nRows, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryOne(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::map<std::string, std::string>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryOne(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryMore(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMore(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryMoreWithLimit(const std::string& strTableName, const std::string& strKeyColName, int nOffset, int nRows, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMoreWithLimit(strTableName, strKeyColName, nOffset, nRows, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::Execute()
{
	return true;
}

void NFCMysqlModule::OnTimer(uint32_t nTimerID)
{
	if (m_pMysqlDriverManager)
	{
		m_pMysqlDriverManager->CheckMysql();
	}
}

bool NFCMysqlModule::AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
	if (!m_pMysqlDriverManager)
	{
		return false;
	}

	return m_pMysqlDriverManager->AddMysqlServer(nServerID, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
}

bool NFCMysqlModule::Delete(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Delete(strTableName, strKeyColName, strKey);
	}

	return false;
}

bool NFCMysqlModule::Exists(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, bool& bExit)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->Exists(strTableName, strKeyColName, strKey, bExit);
	}

	return false;
}

bool NFCMysqlModule::QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::vector<std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMoreByLike(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

bool NFCMysqlModule::QueryMoreByLike(const std::string& strTableName, const std::string& strKeyColName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::map<std::string, std::string>>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->QueryMoreByLike(strTableName, strKeyColName, strKey, fieldVec, valueVec);
	}

	return false;
}

