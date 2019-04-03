// -------------------------------------------------------------------------
//    @FileName         :    NFCNoSqlDriverManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCNoSqlDriverManager
// -------------------------------------------------------------------------
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

#include <algorithm>
#include "NFCNoSqlDriverManager.h"

NFCNoSqlDriverManager::NFCNoSqlDriverManager()
{
}

NFCNoSqlDriverManager::~NFCNoSqlDriverManager()
{

}

bool NFCNoSqlDriverManager::AddNoSqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string& strAuthKey)
{
    std::string  strDnsIp;
    if (!strDns.empty())
    {
        strDnsIp = GetIPByHostName(strDns);
    }

    if (strDnsIp.empty())
    {
        strDnsIp = strIP;
    }

    if (strDnsIp.empty())
    {
        return false;
    }


    if (strDnsIp.empty())
    {
        return false;
    }

    NFINoSqlDriver* pNoSqlDriver = mvNoSql.GetElement(nServerID);
    if (pNoSqlDriver)
    {
        return false;
    }

    NFINoSqlDriver* pInvalidRedisDriver = mvInvalidNoSql.GetElement(nServerID);
    if (pInvalidRedisDriver)
    {
        return false;
    }


    pNoSqlDriver = NF_NEW NFCNoSqlDriver();
    if (pNoSqlDriver->Connect(strDnsIp, nPort, strAuthKey))
    {
        mvNoSql.AddElement(nServerID, pNoSqlDriver);
    }
    else
    {
        mvInvalidNoSql.AddElement(nServerID, pNoSqlDriver);
    }

    return true;
}

NFINoSqlDriver* NFCNoSqlDriverManager::GetNoSqlDriver()
{
    return mvNoSql.First(); // ÔÝÊ±ÏÈ¸øfirst
}

void NFCNoSqlDriverManager::CheckNoSql()
{
    //////////////////////////////////////////////////////////////////////////
    int nServerID = 0;
    std::vector<int> xIntVec;
    for (NFINoSqlDriver* pNoSqlDriver = mvNoSql.First(nServerID); pNoSqlDriver != NULL; pNoSqlDriver = mvNoSql.Next(nServerID))
    {
        if (!pNoSqlDriver->Enable())
        {
            xIntVec.push_back(nServerID);
            mvInvalidNoSql.AddElement(nServerID, pNoSqlDriver);
        }
    }

    for (int i = 0; i < (int)xIntVec.size(); ++i)
    {
        mvNoSql.RemoveElement(xIntVec[i]);
    }
    //////////////////////////////////////////////////////////////////////////
    xIntVec.clear();
    nServerID = 0;

    for (NFINoSqlDriver* pNoSqlDriver = mvInvalidNoSql.First(nServerID); pNoSqlDriver != NULL; pNoSqlDriver = mvInvalidNoSql.Next(nServerID))
    {
        if (!pNoSqlDriver->Enable())
        {
            pNoSqlDriver->ReConnect();
            if (pNoSqlDriver->Enable())
            {
                xIntVec.push_back(nServerID);
                mvNoSql.AddElement(nServerID, pNoSqlDriver);
            }
        }
    }

    for (int i = 0; i < (int)xIntVec.size(); ++i)
    {
        mvInvalidNoSql.RemoveElement(xIntVec[i]);
    }
}

std::string NFCNoSqlDriverManager::GetIPByHostName(const std::string& strHostName)
{
    if (strHostName.empty())
    {
        return std::string();
    }

    hostent* pHost = gethostbyname(strHostName.c_str());
    if (pHost == NULL)
    {
        return std::string();
    }

    if (pHost->h_addr_list[0] != NULL)
    {
        char strIp[32] = { 0 };
        inet_ntop(pHost->h_addrtype, pHost->h_addr_list[0], strIp, sizeof(strIp));
        return std::string(strIp);
    }

    return std::string();
}