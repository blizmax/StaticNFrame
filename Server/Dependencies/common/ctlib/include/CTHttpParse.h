//===============================================
// CHHttpParse.h
// Http响应解析类
//
//===============================================

#ifndef __CTLIB_HTTPPARSE_H__
#define __CTLIB_HTTPPARSE_H__

#include "CTBase.h"

namespace CTLib
{

typedef enum enHttpParseResult
{
	EHTR_OK = 0,
	EHTR_NOHEAD,	// 没有\r\n\r\n
	EHTR_NOLINE,	// 没有\r\n
	EHTR_METHED,	// Method不是GET/POST/CONNECT/OPTIONS/HEAD
	EHTR_NOSPLIT,	// 没有:
}
EHttpParseResult;

class CTHttpParse
{
public:
	typedef std::pair<std::string, std::string>	TKeyValue;
	typedef std::vector<TKeyValue>				TKeyValueAry;

protected:

	TKeyValueAry	m_staKeyValues;
	TKeyValueAry	m_staSetCookies;
	std::string		m_strHttpData;
	bool			m_bReq;

public:
	int Decode(const char* szBuf, int iBufLen)
	{
		m_staKeyValues.clear();
		m_staSetCookies.clear();
		m_strHttpData = "";

		std::string strLine;
		int iBegin = 0;
		int iRet = 0;
		const int iSkip = 2;
		bool bFirstLine = true;

		char* pHeadEnd = strstr(szBuf, "\r\n\r\n");
		if (pHeadEnd == NULL)
		{
			return EHTR_NOHEAD;
		}
		m_strHttpData.assign(pHeadEnd + 4, iBufLen - (pHeadEnd + 4 - szBuf));

		char* p = NULL;
		for (iBegin = 0; ; iBegin = (p - szBuf) + iSkip)
		{
			p = strstr(szBuf + iBegin, "\r\n");
			if (p == NULL)
			{
				return EHTR_NOLINE;
			}
			else if (p > pHeadEnd) // \r\n\r\n表示http头结束
			{
				return EHTR_OK;
			}

			strLine.assign(szBuf + iBegin, p - szBuf - iBegin);
			if (bFirstLine)
			{
				iRet = DecodeFirstLine(strLine);
				bFirstLine = false;
			}
			else
			{
				iRet = DecodeLine(strLine);
			}

			if (iRet)
			{
				return iRet;
			}
		}

		return EHTR_OK;
	}

	// 解析Http头的第一行, GET / HTTP/1.1
	int DecodeFirstLine(std::string& strLine)
	{
		std::string str;
		TKeyValue stMethod, stUrl, stVersion;
		CTStringUtils::SplitPair(strLine, " ", stMethod.second, str);

		if (strncasecmp(stMethod.second.c_str(), "HTTP", 4) == 0)
		{
			stMethod.first = "Version";
			stUrl.first = "ResultCode";
			stVersion.first = "ResultString";
			m_bReq = false;
		}
		else
		{
			if (strcasecmp(stMethod.second.c_str(), "GET") == 0 ||
				strcasecmp(stMethod.second.c_str(), "POST") == 0 ||
				strcasecmp(stMethod.second.c_str(), "CONNECT") == 0 ||
				strcasecmp(stMethod.second.c_str(), "OPTIONS") == 0 ||
				strcasecmp(stMethod.second.c_str(), "HEAD") == 0)
			{
				stMethod.first = "Method";
				stUrl.first = "URI";
				stVersion.first = "Version";
				m_bReq = true;
			}
			else
			{
				return EHTR_METHED;
			}
		}

		CTStringUtils::SplitPair(str, " ", stUrl.second, stVersion.second);

		m_staKeyValues.push_back(stMethod);
		m_staKeyValues.push_back(stUrl);
		m_staKeyValues.push_back(stVersion);

		return EHTR_OK;
	}

	// 解析Http头的其他行
	int DecodeLine(std::string& strLine)
	{
		TKeyValue stNode;

		CTStringUtils::SplitPair(strLine, ": ", stNode.first, stNode.second);
		if (stNode.second.empty())
		{
			return EHTR_NOLINE;
		}

		// 把Cookie信息放到stSetCookies中
		if (strcasecmp(stNode.first.c_str(), "Set-Cookie") == 0)
		{
			TKeyValue stCookie;
			CTStringUtils::SplitPair(stNode.second, "=", stCookie.first, stCookie.second);
			if (!stCookie.second.empty())
			{
				m_staSetCookies.push_back(stCookie);
			}
		}

		m_staKeyValues.push_back(stNode);
		return EHTR_OK;
	}

	bool Get(const char* szKey, std::string& strValue) const
	{
		for (unsigned int i = 0; i < m_staKeyValues.size(); ++i)
		{
			if (strcasecmp(szKey, m_staKeyValues[i].first.c_str()) == 0)
			{
				strValue = m_staKeyValues[i].second;
				return true;
			}
		}
		return false;
	}

	TKeyValueAry& GetSetCookies()
	{
		return m_staSetCookies;
	}

	bool Set(const char* szKey, const std::string& strValue)
	{
		for (unsigned int i = 0; i < m_staKeyValues.size(); ++i)
		{
			if (strcasecmp(szKey, m_staKeyValues[i].first.c_str()) == 0)
			{
				m_staKeyValues[i].second = strValue;
				return true;
			}
		}
		return false;
	}

	// 返回Decode中Http内容的长度, 因为分包的缘故, <= Content-Length
	int GetDataLength() const
	{
		return m_strHttpData.size();
	}

	// 返回当前是Req还是Ack
	bool GetType() const
	{
		return m_bReq;
	}

	int Encode(std::string& strBuf) const
	{
		int iRet = EncodeHead(strBuf);
		if (iRet)
		{
			return iRet;
		}

		strBuf += m_strHttpData;
		return 0;
	}

	int EncodeHead(std::string& strBuf) const
	{
		if (m_staKeyValues.size() <= 3)
		{
			return -1;
		}

		strBuf = m_staKeyValues[0].second + " " +
				 m_staKeyValues[1].second + " " +
				 m_staKeyValues[2].second + "\r\n";

		for (unsigned int i = 3; i < m_staKeyValues.size(); ++i)
		{
			strBuf += m_staKeyValues[i].first + ": " +
					  m_staKeyValues[i].second + "\r\n";
		}

		strBuf += "\r\n";
		return 0;
	}
};

}

#endif //__CTLIB_HTTPPARSE_H__
