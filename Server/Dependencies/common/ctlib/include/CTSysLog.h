/**
* linux syslog类
*/

#ifndef __CTLIB_SYSLOG_H__
#define __CTLIB_SYSLOG_H__

#ifndef WIN32

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/param.h>

namespace CTLib
{

char CT_PRIORITY_NAMES[][16] =
{
    "emerg",
    "alert",
    "crit",
    "err",
    "warning",
    "notice",
    "info",
    "debug"
};

/**
* 封装了写syslog的操作，并默认也写本地日志
* 本地日志写在/var/qqlog/下，按月分目录，按小时分文件。如果目录不存在，自动创建目录。
*/
class CTSysLog
{
private:
    bool        m_bInited;		// 是否调用了openlog
	bool		m_bLocalLog;	// 是否在本地记录日志
    char        m_szBuff[2048];	// 日志建议不要超过1K，避免大包造成性能下降
	char		m_szLocalPath[CT_PATH_MAX];
	struct tm	m_curr;

    struct tm& GetCurrentTime()
    {
        time_t t = time(0);
        m_curr = *localtime(&t);
		return m_curr;
    }

	void TrimLog(char* pszBuff)
	{
		for (int i = 0; pszBuff[i] != '\0'; ++i)
		{
			// |是字段分割符，自动替换为^
			if (pszBuff[i] == '|')
			{
				pszBuff[i] = '^';
			}
			// syslog会把换行替换为空格，因为可能写本地Log，所以就先自己替换了
			else if (pszBuff[i] == '\r' || pszBuff[i] == '\n')
			{
				pszBuff[i] = ' ';
			}
		}
	}

	/**
	* 写本地日志，按月分目录，按小时分文件，自动创建目录
	*
	* @note 如果多个进程/线程同时写本地日志，可能会造成日志错误，但这种概率不大
	* 而且本地日志是备用的，重要性不高，可以接受这种错误
	*/
	int LocalLog(const char* szLine)
	{
		char szLocalFile[PATH_MAX];
		ct_snprintf(szLocalFile, sizeof(szLocalFile), "%s/%04d%02d/%04d%02d%02d-%02d.log",
			m_szLocalPath, m_curr.tm_year+1900, m_curr.tm_mon+1,
			m_curr.tm_year+1900, m_curr.tm_mon+1, m_curr.tm_mday, m_curr.tm_hour);

		mode_t iFileMask = umask(0);
		FILE* pstFile = ct_fopen(szLocalFile, "a+");

		if (pstFile == NULL)	// 一般是目录没有创建，此时自动创建目录
		{
			char szLocalDir[PATH_MAX];
			ct_snprintf(szLocalDir, sizeof(szLocalDir), "%s/%04d%02d",
				m_szLocalPath, m_curr.tm_year+1900, m_curr.tm_mon+1);
			mkdir(szLocalDir, 0777);

			if ((pstFile = fopen(szLocalFile, "a+")) == NULL)
			{
                umask(iFileMask);
				return -1;
			}
		}

		fprintf(pstFile, "%s\n", szLine);
		fclose(pstFile);

        umask(iFileMask);
		return 0;
	}

public:
    CTSysLog() : m_bInited(false), m_bLocalLog(false)
    {
    }

    virtual ~CTSysLog()
    {
    }

    /**
    * 调用openlog
    * @param pszIdent 对应syslog-ng的program()
    * @param iOption  参见man syslog
    * @param iFacility 参见man syslog
	* @param szLocalDir 保存本地日志的路径，为NULL或""时不写本地日志
    */
    void Init(const char *pszIdent, int iOption, int iFacility, const char* szLocalDir = "/var/qqlog/")
    {
        openlog(pszIdent, iOption, iFacility);
        m_bInited = true;

		if (szLocalDir && szLocalDir[0] != 0)
		{
			m_bLocalLog = true;
			ct_snprintf(m_szLocalPath, sizeof(m_szLocalPath), "%s", szLocalDir);
		}
		else
		{
			m_bLocalLog = false;
			m_szLocalPath[0] = 0;
		}
    }

    /**
    * 为了让日志容易查询，要求必须提供QQ号码和应用名
    */
    void VLog(int iPriority, int iUin, const char* pszApp, const char* pszFormat, va_list ap)
    {
        if (!m_bInited)
        {
            Init("CGI", LOG_PID|LOG_NDELAY, LOG_LOCAL3);
        }

		GetCurrentTime();

        // 先输出QQ号、时间、应用名、进程ID
        char* pszLine = m_szBuff;
        int iSize = sizeof(m_szBuff);
        int iRet = ct_snprintf(pszLine, iSize, "%u|%04d%02d%02d%02d%02d%02d|%u|%s|%s|", iUin,
				m_curr.tm_year+1900, m_curr.tm_mon+1, m_curr.tm_mday,
				m_curr.tm_hour, m_curr.tm_min, m_curr.tm_sec,
                getpid(), CT_PRIORITY_NAMES[LOG_PRI(iPriority)], pszApp);
        pszLine += iRet;
        iSize -= iRet;
        
        // 再输出信息
        va_list aplist = ap;
        iRet = ct_vsnprintf(pszLine, iSize, pszFormat, aplist);
        va_end(aplist);

        if (iRet >= iSize)
        {
            m_szBuff[iSize-1] = 0;
        }

		TrimLog(pszLine);

		// 写本地日志
		if (m_bLocalLog)
		{
			LocalLog(m_szBuff);
		}

        syslog(iPriority, m_szBuff);
    }

#define SYSLOG_VA(iPriority, iUin, pszApp, pszFormat)  do { va_list ap; va_start(ap, pszFormat); VLog(iPriority, iUin, pszApp, pszFormat, ap); va_end(ap); } while(0)

    /**
    * 这类日志会被转存到MySQL，供玩家查询。要谨慎，只记录玩的记录
    */
    void LogDB(int iUin, const char* pszApp, const char* pszFormat, ...)
    {
        SYSLOG_VA(LOG_INFO, iUin, pszApp, pszFormat);
    }

    /**
    * Debug类信息，一般是用于内部查询的、操作过程的记录
    */
    void LogDebug(int iUin, const char* pszApp, const char* pszFormat, ...)
    {
        SYSLOG_VA(LOG_DEBUG, iUin, pszApp, pszFormat);
    }

    /**
    * Warning类信息，一般是用于内部查询的、与用户有关的错误信息
    */
    void LogWarning(int iUin, const char* pszApp, const char* pszFormat, ...)
    {
        SYSLOG_VA(LOG_WARNING, iUin, pszApp, pszFormat);
    }

    /**
    * 系统内部严重错误，包括接口错误，一般是用于内部查询的、与系统有关的错误信息
    */
    void LogError(int iUin, const char* pszApp, const char* pszFormat, ...)
    {
        SYSLOG_VA(LOG_ERR, iUin, pszApp, pszFormat);
    }
};

} //namespace CTLib

#endif // WIN32

#endif //__CTLIB_SYSLOG_H__
