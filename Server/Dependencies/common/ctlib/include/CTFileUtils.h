/**
* 读写文件的封装类
*/

#ifndef __CTLIB_FILEUTILS_H__
#define __CTLIB_FILEUTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string>
#include "CTBase.h"

namespace CTLib
{

/**
* 读写文件的封装类
*/
class CTFileUtils
{
public:
	/**
	* 得到文件的长度
	* @param pszFileName in 要计算长度的文件
	* @param iFileSize out 保存了文件的长度
	* @return 0表示成功, 其他表示失败
	*/
	static int GetFileSize(const char* pszFileName, long& lFileSize)
	{
		lFileSize = 0;

#ifdef WIN32
        struct _stat32 stStat;
#else
        struct stat stStat;
#endif
		int iRet = ct_stat(pszFileName, &stStat);
		if (iRet)
		{
			return iRet;
		}

		lFileSize = stStat.st_size;
		return 0;
	}

	/**
	* 把文件的数据全部读入缓冲区中
	* @param pszFileName in 要读取的文件
	* @param ppszBuf out 保存文件内容的缓冲区，这个缓冲区是ReadFile内部new出来的，调用者需要手动delete
	* @param iBufLen out 保存了文件的长度
	* @return 0表示成功, 其他表示失败
	*/
	static int ReadFile(const char* pszFileName, char** ppszBuf, size_t& iBufLen)
	{
        // 参数错误
        if (!pszFileName)
        {
            return -1;
        }

        // 打开文件失败
        FILE * pstFile = ct_fopen(pszFileName, "rb");
        if (!pstFile)
        {
            return -2;
        }

		int iRet = 0;
		do
		{
			// 获取文件长度
			iRet = fseek(pstFile, 0L, SEEK_END);
			if (iRet)
			{
				iRet = -3;
				break;
			}

			iBufLen = ftell(pstFile);
			if (iBufLen < 0)
			{
				iRet = -4;
				break;
			}

			iRet = fseek(pstFile, 0L, SEEK_SET);
			if (iRet)
			{
				iRet = -5;
				break;
			}

			char* pszBuf = new char[iBufLen + 1];
			*ppszBuf = pszBuf;
			if (pszBuf == NULL)
			{
				iRet = -6;
				break;
			}

			size_t iLeft = iBufLen;
			size_t iRead = 0;
			size_t iCount = 0;

			// 循环读出文件数据		
			while (!feof(pstFile) && iRead < iBufLen)
			{
				iCount = fread(pszBuf + iRead, sizeof(char), iLeft, pstFile);
				iLeft -= iCount;
				iRead += iCount;

				iRet = ferror(pstFile);
				if (iRet)
				{
					break;
				}
			}

			iBufLen = iRead;
			pszBuf[iRead] = 0;
		}
		while(0);

        fclose(pstFile);
        return iRet;
    }

	/**
	* 把文件的数据全部读入缓冲区中
	* @param pszFileName in 要读取的文件
	* @param sBuf out 保存文件内容的缓冲区
	* @return 0表示成功, 其他表示失败
	*/
	static int ReadFile(const char* pszFileName, std::string& sBuf)
	{
		sBuf.clear();

		char* pszBuf = NULL;
		size_t iBufLen = 0;
		int iRet = ReadFile(pszFileName, &pszBuf, iBufLen);
		if (iRet == 0 && pszBuf != NULL && iBufLen > 0)
		{
			sBuf.assign(pszBuf, iBufLen);
		}

		if (pszBuf)
		{
			delete [] pszBuf;
		}

		return iRet;
	}

	/**
	* 把缓冲区的数据全部写入文件
	* @return 0表示成功, 其他表示失败
	* @note 文件如果不存在会自动创建，如果存在会被清空
	*/
	static int WriteFile(const char* pszFileName, const char* pszBuf, size_t iBufLen)
	{
		// 参数错误
		if (!pszFileName)
		{
			return -1;
		}

#ifndef WIN32
        mode_t iMode = umask(0);
		FILE* pstFile = ct_fopen(pszFileName, "w+b");
        umask(iMode);
#else
        FILE* pstFile = ct_fopen(pszFileName, "w+b");
#endif
		if (pstFile == NULL)
		{
			return -2;
		}

		size_t iLeft = iBufLen;
		size_t iWrite = 0;
		size_t iCount = 0;
		int iRet = 0;

		// 循环读出文件数据
		while (iWrite < iBufLen)
		{
			iCount = fwrite(pszBuf + iWrite, sizeof(char), iLeft, pstFile);
			iLeft -= iCount;
			iWrite += iCount;

			iRet = ferror(pstFile);
			if (iRet)
			{
				break;
			}
		}

		fflush(pstFile);
		fclose(pstFile);

		return iRet;
	}

	/**
	* 把缓冲区的数据全部写入文件
	* @return 0表示成功, 其他表示失败
	* @note 文件如果不存在会自动创建，如果存在会被清空
	*/
	static int WriteFile(const char* pszFileName, const std::string& sBuf)
	{
		return WriteFile(pszFileName, sBuf.c_str(), sBuf.size());
	}
};

} // namespace CTLib

#endif //__CTLIB_FILEUTILS_H__
