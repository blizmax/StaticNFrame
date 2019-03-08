/**
* 配置文件类.
* 从多个配置文件读取配置到单件类中
*/

#ifndef __CTLIB_CONFIG_H__
#define __CTLIB_CONFIG_H__

// 	配置文件格式如下(sSection和name不区分大小写):
// 	[sSection]
// 	name=value
// 	#注释

#include <iostream>
#include <fstream>
#include "CTBase.h"

namespace CTLib
{

/**
* CTConfig类的返回值定义
*/
typedef enum enConfigRet
{
	ECR_OK = 0,
	ECR_FILE_OPENFAIL,		///< 1:打开文件失败
	ECR_SECTION_NOTEXIST,	///< 2:没找到对应的sSection
	ECR_KEY_NOTEXIST,		///< 3:没找到对应的key
	ECR_PARSELINE_FAIL,		///< 4:读取配置时解析行失败
} ECONFIG_RET;

class CTConfig
{
public:
	typedef map<string, string>		    MapSection;
	typedef map<string, MapSection>		MapFile;
	typedef map<string, MapFile>		MapFiles;

public:
	// piRet 用来返回读配置的结果
	const string GetString(const char* pszFile,
						   const char* pszSection,
						   const char* pszKey,
						   int* piRet = NULL)
	{
		string s(pszFile);

		// 查找文件
		MapFiles::iterator itFiles = m_conf.find(s);
		if (itFiles == m_conf.end())
		{
			int iRet = LoadConfig(pszFile);
			if (iRet)
			{
				SetResult(piRet, iRet);
				return "";
			}
			itFiles = m_conf.find(s);
		}

		// 查找节
		s = pszSection;
        CTStringUtils::MakeLower(s);
		MapFile::iterator itFile = itFiles->second.find(s);
		if (itFile == itFiles->second.end())
		{
			SetResult(piRet, ECR_SECTION_NOTEXIST);
			return "";
		}

		// 查找项
		s = pszKey;
        CTStringUtils::MakeLower(s);
		MapSection::iterator itSection = itFile->second.find(s);
		if (itSection == itFile->second.end())
		{
			SetResult(piRet, ECR_KEY_NOTEXIST);
			return "";
		}
		
		SetResult(piRet, ECR_OK);
		return itSection->second;
	}

    /**
    * 取字符串类型的配置
    * @param[out] piRet 用来返回读配置的结果
    * @note 这个方法只能用在只有一个配置文件，并且要先调用LoadConfig的情况下
    */
    const string GetString(const char* pszSection,
                           const char* pszKey,
                           int* piRet = NULL)
    {
        if (m_conf.size() != 1)
        {
            SetResult(piRet, ECR_FILE_OPENFAIL);
            return "";
        }

        MapFile mapFile = m_conf.begin()->second; // 只有一个配置文件，那就是begin()

        string s = pszSection;
        CTStringUtils::MakeLower(s);
        MapFile::iterator itFile = mapFile.find(s);
        if (itFile == mapFile.end())
        {
            SetResult(piRet, ECR_SECTION_NOTEXIST);
            return "";
        }

        // 查找项
        s = pszKey;
        CTStringUtils::MakeLower(s);
        MapSection::iterator itSection = itFile->second.find(s);
        if (itSection == itFile->second.end())
        {
            SetResult(piRet, ECR_KEY_NOTEXIST);
            return "";
        }

        SetResult(piRet, ECR_OK);
        return itSection->second;
    }

	int GetInt(const char* pszFile,
               const char* pszSection,
               const char* pszKey,
               int* piRet = NULL)
	{
		return atoi(GetString(pszFile, pszSection, pszKey, piRet).c_str());
	}

    /**
    * 取int类型的配置
    * @param[out] piRet 用来返回读配置的结果
    * @note 这个方法只能用在只有一个配置文件，并且要先调用LoadConfig的情况下
    */
    int GetInt(const char* pszSection,
               const char* pszKey,
               int* piRet = NULL)
    {
        return atoi(GetString(pszSection, pszKey, piRet).c_str());
    }

	// 重读某个配置
	int ReloadConfig(const char* pszFile)
	{
		MapFiles::iterator itFiles = m_conf.find(pszFile);
		if (itFiles != m_conf.end())
		{
			m_conf.erase(itFiles);
		}

		return LoadConfig(pszFile);
	}

	// 清除已经读到的所有配置
	void Clear()
	{
		m_conf.clear();
	}

	// 加载配置
	int LoadConfig(const char* pszFile)
	{
		ifstream fin(pszFile);
		if (!fin.is_open())
		{
			return ECR_FILE_OPENFAIL;
		}

		MapFiles::iterator itFiles = m_conf.find(pszFile);
		if (itFiles != m_conf.end())
		{	
			// 配置文件已加载
			return ECR_OK;
		}

		MapFile empty;
		m_conf.insert(MapFiles::value_type(pszFile, empty));
		MapFile& mapFile = m_conf[pszFile];
		
		int line_count = 0;
		string inbuf, subbuf;
		string sSection = "";
		int iRet = 0;
		
		while (!fin.eof())
		{
			// 读取一行配置
			getline(fin, inbuf, '\n');
			line_count ++;
			
			size_t last_pos = inbuf.find_last_of("\r\n");
			if (last_pos != string::npos) 
			{
				subbuf = inbuf.substr(0, last_pos);
			}
			else
			{
				subbuf = inbuf;
			}
			
			// 解析一行配置
			iRet = ParseLine(mapFile, sSection, subbuf);
			if (iRet)
			{
				return iRet;
			}
		}

		return ECR_OK;
	}

protected:
	int ParseLine(MapFile& mapFile, string& sSection, string& line)
	{
		size_t first = 0, last = 0;

		Trim(line, " \t\r\n");

		// 分析是否为注释行, 以#为注释
		first = line.find_first_of("#"); 
		if (first != (string::npos))
		{
			line = line.substr(0, first);
		}

		if (line.empty())
		{
			return ECR_OK;
		}

		// 分析是否为sSection 
		last = line.rfind(']');
		if (line[0] == '[' && last != string::npos)
		{
			// 新的一节
			sSection = line.substr(1,last-1);
            CTStringUtils::MakeLower(sSection);
			MapSection empty;
			mapFile.insert(MapFile::value_type(sSection, empty));
			return ECR_OK;
		}
   
		// 为一正常配置行
		if (sSection.empty())
		{
			return ECR_PARSELINE_FAIL;
		}

		first = line.find('=');
		if (first == string::npos)
		{
			// 没有=
			return ECR_PARSELINE_FAIL;
		}

		string name = line.substr(0, first);
		string value = line.substr(first+1, string::npos);

		Trim(name, " \t\r\n");
		Trim(value, " \t\r\n");
        CTStringUtils::MakeLower(name);

		if (name.empty())
		{
			return ECR_PARSELINE_FAIL;  
		}

		MapSection& mapSection = mapFile[sSection];
		mapSection.insert(MapSection::value_type(name, value));
		return ECR_OK;
	}

	string& Trim(string& line, const char* trim)
	{
		size_t first = line.find_first_not_of(trim);
		size_t last  = line.find_last_not_of(trim);
		if (first == string::npos || last == string::npos)
		{
			line = "";
		}
		else
		{
			line = line.substr(first, last-first+1);
		}

		return line;
	}

	int SetResult(int* piRet, int iRet)
	{
		if (piRet)
		{
			*piRet = iRet;
		}

		return iRet;
	}

private:
	MapFiles	m_conf;
};

} // namespace CTLib

#endif //__CTLIB_CONFIG_H__
