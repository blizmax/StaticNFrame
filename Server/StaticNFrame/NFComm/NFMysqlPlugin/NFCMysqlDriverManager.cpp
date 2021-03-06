#ifdef _MSC_VER
#include <WinSock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#endif

#include "NFCMysqlDriver.h"
#include "NFCMysqlDriverManager.h"

NFCMysqlDriverManager::NFCMysqlDriverManager(): mnLastCheckTime(0)
{
}

NFCMysqlDriverManager::~NFCMysqlDriverManager()
{
	for (NFIMysqlDriver* pMysqlDriver = mvMysql.First(); nullptr != pMysqlDriver; pMysqlDriver = mvMysql.Next())
	{
		NF_SAFE_DELETE(pMysqlDriver);
		pMysqlDriver = nullptr;
	}
	mvMysql.ClearAll();
	for (NFIMysqlDriver* pMysqlDriver = mvInvalidMsyql.First(); nullptr != pMysqlDriver; pMysqlDriver = mvInvalidMsyql.
	     Next())
	{
		NF_SAFE_DELETE(pMysqlDriver);
		pMysqlDriver = nullptr;
	}
	mvInvalidMsyql.ClearAll();
}

NFIMysqlDriver* NFCMysqlDriverManager::GetMysqlDriver()
{
	return mvMysql.First(); // ��ʱ�ȸ�first
}

void NFCMysqlDriverManager::CheckMysql()
{
	int nServerID = 0;
	std::vector<int> xIntVec;
	for (NFIMysqlDriver* pMysqlDriver = mvMysql.First(nServerID); pMysqlDriver != nullptr; pMysqlDriver = mvMysql.
	     Next(nServerID))
	{
		if (!pMysqlDriver->Enable())
		{
			xIntVec.push_back(nServerID);
			mvInvalidMsyql.AddElement(nServerID, pMysqlDriver);
		}
	}

	for (int i = 0; i < (int)xIntVec.size(); ++i)
	{
		mvMysql.RemoveElement(xIntVec[i]);
	}
	//////////////////////////////////////////////////////////////////////////
	xIntVec.clear();
	nServerID = 0;

	for (NFIMysqlDriver* pMysqlDriver = mvInvalidMsyql.First(nServerID); pMysqlDriver != nullptr; pMysqlDriver =
	     mvInvalidMsyql.Next(nServerID))
	{
		if (!pMysqlDriver->Enable() && pMysqlDriver->CanReconnect())
		{
			pMysqlDriver->Reconnect();
			if (pMysqlDriver->Enable())
			{
				xIntVec.push_back(nServerID);
				mvMysql.AddElement(nServerID, pMysqlDriver);
			}
		}
	}

	for (int i = 0; i < (int)xIntVec.size(); ++i)
	{
		mvInvalidMsyql.RemoveElement(xIntVec[i]);
	}
}

bool NFCMysqlDriverManager::AddMysqlServer(const int nServerID, const std::string& strIP, const int nPort,
                                           const std::string strDBName, const std::string strDBUser,
                                           const std::string strDBPwd, const int nRconnectTime/* = 10*/,
                                           const int nRconneCount/* = -1*/)
{
	NFIMysqlDriver* pMysqlDriver = mvMysql.GetElement(nServerID);
	if (pMysqlDriver)
	{
		return false;
	}

	NFIMysqlDriver* pInvalidRedisDriver = mvInvalidMsyql.GetElement(nServerID);
	if (pInvalidRedisDriver)
	{
		return false;
	}


	pMysqlDriver = NF_NEW NFCMysqlDriver(nRconnectTime, nRconneCount);
	if (pMysqlDriver->Connect(strDBName, strIP, nPort, strDBUser, strDBPwd))
	{
		mvMysql.AddElement(nServerID, pMysqlDriver);
	}
	else
	{
		mvInvalidMsyql.AddElement(nServerID, pMysqlDriver);
		return false;
	}

	return true;
}
