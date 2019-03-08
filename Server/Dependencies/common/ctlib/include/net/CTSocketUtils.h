/**
* 封装Socket一些常用操作
*/

#ifndef __CTLIB_SOCKETUTILS_H__
#define __CTLIB_SOCKETUTILS_H__

#include "CTBase.h"

#ifdef WIN32
	#include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")

    // WIN32下没有PF_LOCAL，为了能编译，用PF_INET代替
    #ifndef PF_LOCAL
        #define PF_LOCAL  PF_INET
    #endif
#else
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <sys/socket.h>
    #include <sys/un.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
#endif

#include "base/CTLog.h"
#include "base/CTSingleton.h"

#define CT_ESOCKETEXIT  29999       ///< 自己定义的, 表示要求socket退出

namespace CTLib
{

//=====================================
// Winsock2 初始化和释放
class CTSocketInit
{
public:
	CTSocketInit()
	{
#ifdef WIN32
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		{
			CT_FATAL(("WSAStartup failed %d", CT_WSA_ERRNO));
		}
#endif
	}

	~CTSocketInit()
	{
#ifdef WIN32
		WSACleanup();
#endif
	}
};

//=====================================
// CTSocketExit用来表示主线程要求退出
// 此时子线程的socket操作在select期间检查CTSocketExit, 
// 来实现子线程的快速退出

class CTSocketExit
{
public:
	CTSocketExit() : m_bExit (false) {}
	~CTSocketExit() {}

	void SetExit() { m_bExit = true; }
	bool IsExit() const { return m_bExit; }

protected:
	bool	m_bExit;
};

#define CT_SOCKET_SETEXIT() CTLib::CTSingleton<CTLib::CTSocketExit>::Instance()->SetExit()
#define CT_SOCKET_ISEXIT() CTLib::CTSingleton<CTLib::CTSocketExit>::Instance()->IsExit()

//=====================================
class CTSocketUtils
{
public:
	static int NonblockSocket(SOCKET s)
	{
#ifndef WIN32
		int flag = 1;
		if (ioctl(s, FIONBIO, &flag) &&
			((flag = fcntl(s, F_GETFL, 0)) < 0 || fcntl(s, F_SETFL, flag|O_NONBLOCK) < 0))
		{
			return errno;
		}
#else
		unsigned long ul = 1;
		if(ioctlsocket(s, FIONBIO, &ul))
		{
			return CT_WSA_ERRNO;
		}
#endif
		return 0;
	}

	// 超时的单位是毫秒
	static int Select(SOCKET sock, bool rd, int iTimeOut)
	{
		struct timeval tv = {0};
		struct timeval tv2 = {0};
		struct timeval* ptv = NULL;
		int iLoop = 0;
		const int iSelectTime = 100; // 100毫秒

		if (iTimeOut < 0) // 阻塞
		{
			ptv = NULL;
			iLoop = 0x7FFFFFFF;
		}
		else
		{
			ptv = &tv2;
			if (iTimeOut >= iSelectTime)
			{
				iLoop = CT_CEIL(iTimeOut, iSelectTime);
				tv.tv_sec = iSelectTime / 1000;
				tv.tv_usec = (iSelectTime % 1000) * 1000;
			}
			else
			{
				iLoop = 1;
				tv.tv_sec = iTimeOut / 1000;
				tv.tv_usec = (iTimeOut % 1000) * 1000;
			}
		}

		fd_set fds;
		int iRet = 0;
		fd_set* prd = (rd ? &fds : NULL);
		fd_set* pwr = (rd ? NULL : &fds);
		int nfds = static_cast<int>(sock) + 1;
		for (; iLoop > 0; --iLoop)
		{
			if (ptv != NULL)
			{
				tv2.tv_sec = tv.tv_sec;
				tv2.tv_usec = tv.tv_usec;
			}

			FD_ZERO(&fds);
			FD_SET(sock, &fds);
			iRet = select(nfds, prd, pwr, NULL, ptv);
			if (CT_SOCKET_ISEXIT())
			{
				return CT_ESOCKETEXIT;
			}

			if (iRet > 0)
			{
				return 0;
			}
			else if (iRet == 0)
			{
				continue;
			}
			else
			{
				if (CT_WSA_ERRNO == EINTR)
				{
					continue;
				}

				return CT_WSA_ERRNO;
			}
		}

		return CT_ETIME;
	}

	// 超时的单位是毫秒
	static int SelectPair(SOCKET first, SOCKET second, bool& retfirst,
						  bool rd, int iTimeOut)
	{
		struct timeval tv, tv2;
		struct timeval* ptv = NULL;
		int iLoop = 0;
		const int iSelectTime = 100; // 100毫秒

		if (iTimeOut < 0) // 阻塞
		{
			ptv = NULL;
			iLoop = 0x7FFFFFFF;
		}
		else
		{
			ptv = &tv2;
			if (iTimeOut >= iSelectTime)
			{
				iLoop = CT_CEIL(iTimeOut, iSelectTime);
				tv.tv_sec = iSelectTime / 1000;
				tv.tv_usec = (iSelectTime % 1000) * 1000;
			}
			else
			{
				iLoop = 1;
				tv.tv_sec = iTimeOut / 1000;
				tv.tv_usec = (iTimeOut % 1000) * 1000;
			}
		}

		fd_set fds;
		int iRet = 0;
		int nfds = static_cast<int>(first > second ? first : second) + 1;
		fd_set* prd = (rd ? &fds : NULL);
		fd_set* pwr = (rd ? NULL : &fds);

		for (; iLoop > 0; --iLoop)
		{
			if (ptv != NULL)
			{
				tv2.tv_sec = tv.tv_sec;
				tv2.tv_usec = tv.tv_usec;
			}

			FD_ZERO(&fds);
			FD_SET(first, &fds);
			FD_SET(second, &fds);
			iRet = select(nfds, prd, pwr, NULL, ptv);
			if (CT_SOCKET_ISEXIT())
			{
				return CT_ESOCKETEXIT;
			}

			if (iRet > 0)
			{
				retfirst = (FD_ISSET(first, &fds) != 0);
				return 0;
			}
			else if (iRet == 0)
			{
				continue;
			}
			else
			{
				if (CT_WSA_ERRNO == EINTR)
				{
					continue;
				}

				return CT_WSA_ERRNO;
			}
		}

		return CT_ETIME;
	}
};

} // namespace CTLib

#endif //__CTLIB_SOCKETUTILS_H__
