/**
* Server开发的一些常用函数
*/

#ifndef __CTLIB_SERVERUTILS_H__
#define __CTLIB_SERVERUTILS_H__

#include <signal.h>
#include "CTFileUtils.h"

namespace CTLib
{

/**
* 封装了Server开发的一些常用函数
*/
class CTServerUtils
{
public:
#ifndef WIN32
    static void Ignore(int signum)
    {
        struct sigaction sig;
        sig.sa_handler = SIG_IGN;
        sig.sa_flags = 0;

        sigemptyset(&sig.sa_mask);
        sigaction(signum, &sig, 0);
    }
#endif

    /**
    * 转为守护进程
    */
    static void InitDaemon(void)
    {
#ifndef WIN32
        pid_t pid;

        if ((pid = fork()) != 0)
        {
            exit(0);
        }

        setsid();

        signal(SIGINT,  SIG_IGN);
        signal(SIGHUP,  SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
        signal(SIGTERM, SIG_IGN);
        signal(SIGHUP,  SIG_IGN);
        signal(SIGUSR1, SIG_IGN);
        signal(SIGUSR2, SIG_IGN);
        Ignore(SIGPIPE);
        Ignore(SIGHUP);

        if ((pid = fork()) != 0)
        {
            exit(0);
        }

        umask(0);
#endif
    }

    /**
    * 避免运行多个实例
    */
#ifndef WIN32
    static bool CheckSingleRun(const char* pszAppName)
    {
        bool bRet = false;
        string s;
        string sPidFile;

        do
        {
            // 只要文件名，去掉前面的路径
            s = pszAppName;
            size_t iPos = s.find_last_of('/');
            if (iPos != string::npos)
            {
                s = s.substr(iPos + 1);
            }

            // 读取进程ID
            sPidFile = "./" + s + ".pid";
            CTFileUtils::ReadFile(sPidFile.c_str(), s);
            CTStringUtils::TrimLeft(s, " \r\n\t");
            CTStringUtils::TrimRight(s, " \r\n\t");
            int iPid = CTStringUtils::StrToInt<int>(s.c_str());
            if (iPid <= 0)
            {
                bRet = true;
                break;
            }

            // 看看pid文件中保存的进程是否存在
            s.assign(pszAppName);
            iPos = s.find_last_of('/');
            if (iPos != string::npos)
            {
                s = s.substr(iPos + 1);
            }

            string sCmd;
            CTStringUtils::Format(sCmd, "ps -p %d | grep %s -c", iPid, s.c_str());
            FILE* pstPipe = popen(sCmd.c_str(), "r");
            if (pstPipe == NULL)
            {
                printf("popen(%s) fail\n", sCmd.c_str());
                bRet = false;
                break;
            }

            char szBuf[32];
            fgets(szBuf, sizeof(szBuf) - 1, pstPipe);
            pclose(pstPipe);

            int iCount = atoi(szBuf);

            // 进程已经存在了
            if (iCount > 0)
            {
                printf("%s already running!\n", pszAppName);
                bRet = false;
                break;
            }

            bRet = true;
        }
        while(0);

        if (bRet)
        {
            CTStringUtils::Format(s, "%d\n", getpid());
            int iRet = CTFileUtils::WriteFile(sPidFile.c_str(), s);
            if (iRet)
            {
                printf("cannot write pid: ret=%d errno=%d\n", iRet, CT_ERRNO);
                return false;
            }

            return true;
        }

        return bRet;
    }
#else
    static bool CheckSingleRun(const char* pszAppName)
    {
        return true;
    }
#endif

    /**
    * 删除pid文件.
    */
    static void RemovePidFile(const char* pszAppName)
    {
        // 只要文件名，去掉前面的路径
        string s(pszAppName);
        size_t iPos = s.find_last_of('/');
        if (iPos != string::npos)
        {
            s = s.substr(iPos + 1);
        }

        // 读取进程ID
        string sPidFile = "./" + s + ".pid";
        unlink(sPidFile.c_str());
    }
};

} //namespace CTLib

#endif //__CTLIB_SERVERUTILS_H__
