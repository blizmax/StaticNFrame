/**
* SOCKET封装
* 支持PF_LOCAL/PF_INET
* 支持超时控制
* 支持TCP/UDP
*/

#ifndef __CTLIB_SOCKET_H__
#define __CTLIB_SOCKET_H__

#include "CTBase.h"
#include "CTSocketUtils.h"

namespace CTLib
{

/**
* 带超时控制的socket封装
*/
class CTSocket
{
private:
    int             m_iAF;      ///< Address family specification
    int             m_iType;    ///< TCP/UDP/RAW
	bool			m_bListen;	///< 是监听还是主动连接
    bool            m_bUseSelect;  ///< 是在函数内调用select来控制超时，还是有外部epoll处理

	SOCKET			m_iSocket;
	char        	m_szAddr[128];
    socklen_t       m_iAddrLen;
	int				m_iConnTimeOut;
	int				m_iSendTimeOut;
	int				m_iRecvTimeOut;

public:
	enum
	{
		CTLIB_SOCK_TIMEOUT = 5000,		// 5s
	};

public:
	// 超时时间的单位是毫秒
	CTSocket(int iAF = PF_INET,
             int iType = SOCK_STREAM,
             bool bUseSelect = true,     // 默认使用select
			 int iConnTimeOut = CTLIB_SOCK_TIMEOUT, 
			 int iSendTimeOut = CTLIB_SOCK_TIMEOUT,
			 int iRecvTimeOut = CTLIB_SOCK_TIMEOUT) :
        m_iAF(iAF),
		m_iType(iType),
		m_bListen(false),
        m_bUseSelect(bUseSelect),
		m_iSocket(INVALID_SOCKET),
        m_iAddrLen(0),
		m_iConnTimeOut(iConnTimeOut),
		m_iSendTimeOut(iSendTimeOut),
		m_iRecvTimeOut(iRecvTimeOut)
	{
		CTSingleton<CTSocketInit>::Instance();
		memset(m_szAddr, 0, sizeof(m_szAddr));
	}

	virtual ~CTSocket()
	{
		Close();
	}

public:
	bool IsTCP() const { return m_iType == SOCK_STREAM; }
	bool IsListen() const { return m_bListen; }
	bool IsValid() const { return (m_iSocket != INVALID_SOCKET); }

    int GetAF() const { return m_iAF; }
    int GetType() const { return m_iType; }

    SOCKET GetSocket() const { return m_iSocket; }
	void SetSocket(SOCKET s) { m_iSocket = s; }

    bool IsUseSelect() const { return m_bUseSelect; }
    void SetUseSelect(bool bUseSelect) { m_bUseSelect = bUseSelect; }

	int& ConnTimeOut() { return m_iConnTimeOut; }
	int& RecvTimeOut() { return m_iRecvTimeOut; }
	int& SendTimeOut() { return m_iSendTimeOut; }
    int ConnTimeOut() const { return m_iConnTimeOut; }
    int RecvTimeOut() const { return m_iRecvTimeOut; }
    int SendTimeOut() const { return m_iSendTimeOut; }

public:
    /** 创建socket */
    int Create(bool bIsBlock = false)
    {
        if (IsValid())
        {
            Close();
        }

        m_iSocket = socket(m_iAF, m_iType, 0);
        if (!IsValid())
        {
            return CT_WSA_ERRNO;
        }

        // 设为非阻塞模式
        if (!bIsBlock)
        {
            int iRet = CTSocketUtils::NonblockSocket(m_iSocket);
            if (iRet)
            {
                Close();
                return CT_WSA_ERRNO;
            }
        }

        return 0;
    }

	/** 连接 */
	int Connect(const sockaddr* pstAddr, socklen_t iAddrLen)
	{
        CT_ASSERT(iAddrLen <= sizeof(m_szAddr));
		memcpy(m_szAddr, pstAddr, iAddrLen);
        m_iAddrLen = iAddrLen;

        int iRet = 0;
        if (!IsValid()) // 如果没有创建socket
        {
            iRet = Create();
            if (iRet)
            {
                return iRet;
            }
        }

		if (!IsTCP())
		{
			// UDP不用connect
			return 0;
		}

		// 连接
		iRet = connect(m_iSocket, pstAddr, iAddrLen);
		if (iRet < 0)
		{
			int iErrNo = CT_WSA_ERRNO;
			if (iErrNo != CT_EINPROGRESS && iErrNo != CT_EWOULDBLOCK)
			{
				Close();
				return iErrNo;
			}
		}

        if (IsUseSelect())
        {
            /*
            //--Begin: Commented by LincLiu on 2009-10-30 to fix the falsely detected connection status
        	
            //Use select to detect the connection status of this socket, if it's return both readable and 
            //writable, the connection fails with errno, otherwise it's writable only.
            
            iRet = CTSocketUtils::Select(m_iSocket, false, m_iConnTimeOut);

            //--End: Commented by LincLiu on 2009-10-30 to fix the falsely detected connection status
		    */

            //--Begin: Added by LincLiu on 2009-10-30 to fix the falsely detected connection status
            fd_set bsReadSet;
            fd_set bsWriteSet;
            while(1)
            {
                FD_ZERO(&bsReadSet);
                FD_ZERO(&bsWriteSet);
                FD_SET(m_iSocket, &bsReadSet);
                FD_SET(m_iSocket, &bsWriteSet);
                int iMaxFD = m_iSocket + 1;
                struct timeval tv;
                struct timeval *pTv;
                if (0 >= m_iConnTimeOut)
                {
                    pTv = NULL;
                }
                else
                {
                    tv.tv_sec = m_iConnTimeOut / 1000;
                    tv.tv_usec = (m_iConnTimeOut % 1000) * 1000;
                    pTv = &tv;
                }
                iRet = select(iMaxFD, &bsReadSet, &bsWriteSet, NULL, pTv);
                if (0 == iRet || CT_WSA_ERRNO == EINTR)
                {//Timeout without a success or failure indication, let select it again.
                    continue;
                }
                else if(0 > iRet)
                {
                    iRet = CT_WSA_ERRNO;
                    break;					
                }
                if (FD_ISSET(m_iSocket, &bsReadSet) && FD_ISSET(m_iSocket, &bsWriteSet))
                {//This connection failed! break here. Because this stat is not an error of select
                 //operation, so the errno is not set to indicate the status of socket "connect", so
                 //we can not rely on the errno any more. Just set it to ECONNREFUSED (115)
                    int iErr = 0;
                    socklen_t iLen = sizeof(iErr);
                    int iSockOptRet = getsockopt(m_iSocket, SOL_SOCKET, SO_ERROR, &iErr, &iLen);
                    if ( iSockOptRet < 0 || (iErr != 0 && iSockOptRet == 0))
                    {//Berkeley implementation returns zero but set iErr to the errno of this connection
                     //attempt. Solaris implementation return -1 instead. We merge this two possibilities
                     //to avoid portability problems.
                        iRet = ECONNREFUSED;                       
                    }
                    else
                    {
                        iRet = 0;
                    }
                    break;
                }
                else if (FD_ISSET(m_iSocket, &bsWriteSet))				
                {//Connection is established successfully
                    iRet = 0;
                    break;					
                }
            }
			//--End: Added by LincLiu on 2009-10-30 to fix the falsely detected connection status
		    if (iRet)
		    {
			    Close();
			    return iRet;
		    }
        }

		return 0;
	}

	/** PF_INET连接 */
	int Connect(const char* pszIPAddr, unsigned short ushPort)
	{
		if (!pszIPAddr)
		{
			return -1;
		}

        sockaddr_in stTempAddr;
		memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));

		stTempAddr.sin_family = PF_INET;
		stTempAddr.sin_port = htons(ushPort);
		stTempAddr.sin_addr.s_addr = inet_addr(pszIPAddr);
		if (stTempAddr.sin_addr.s_addr == INADDR_NONE)
		{
			return -2;
		}

		return Connect((struct sockaddr*)&stTempAddr, sizeof(stTempAddr));
	}

    /** PF_LOCAL连接  */
#ifndef WIN32
    int Connect(const char* pszPathName)
    {
        if (!pszPathName)
        {
            return -1;
        }

        struct sockaddr_un stTempAddr;
        memset((void*)&stTempAddr, 0, sizeof(stTempAddr));

        stTempAddr.sun_family = PF_LOCAL;
        ct_snprintf(CT_STRSIZE(stTempAddr.sun_path), "%s", pszPathName);

        return Connect((struct sockaddr*)&stTempAddr, sizeof(stTempAddr));
    }
#else
    int Connect(const char* CT_UNUSED(pszPathName))
    {
        return 0;
    }
#endif

	/** 监听 */
    int Listen(const sockaddr* pstAddr, socklen_t iAddrLen)
    {
        CT_ASSERT(iAddrLen <= sizeof(m_szAddr));
        memcpy(m_szAddr, pstAddr, iAddrLen);
        m_iAddrLen = iAddrLen;

		m_bListen = true;

		int iRet = 0;
        if (!IsValid()) // 如果没有创建socket
        {
            iRet = Create();
            if (iRet)
            {
                return iRet;
            }
        }

		// bind
		iRet = 1;
		setsockopt(m_iSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&iRet, sizeof(iRet));

		// 只有TCP才要这样
		if (IsTCP())
		{
			setsockopt(m_iSocket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&iRet, sizeof(iRet));
			struct linger ling = {0, 0};
			setsockopt(m_iSocket, SOL_SOCKET, SO_LINGER, (const char*)&ling, sizeof(ling));

#ifndef WIN32
			//int val = 20;
			//setsockopt(m_iSocket, SOL_TCP, TCP_DEFER_ACCEPT, (const char*)&val, sizeof(val));
#endif
		}

		if (bind(m_iSocket, pstAddr, iAddrLen) < 0)
		{
			Close();
			return CT_WSA_ERRNO;
		}

		// listen
		if (IsTCP())
		{
			if (listen(m_iSocket, 1024) < 0)
			{
				Close();
				return CT_WSA_ERRNO;
			}
		}

		return 0;
	}

    /** PF_INET监听 */
	int Listen(const char *pszIPAddr, unsigned short ushPort)
	{
		if (!pszIPAddr)
		{
			return -1;
		}

        sockaddr_in stTempAddr;
		memset((void *)&stTempAddr, 0, sizeof(sockaddr_in));

        stTempAddr.sin_family = PF_INET;
		stTempAddr.sin_port = htons(ushPort);
		stTempAddr.sin_addr.s_addr = inet_addr(pszIPAddr);
		if (stTempAddr.sin_addr.s_addr == INADDR_NONE)
		{
			return -2;
		}

		return Listen((struct sockaddr*)&stTempAddr, sizeof(stTempAddr));
	}

    /** PF_LOCAL监听 */
#ifndef WIN32
    int Listen(const char* pszPathName)
    {
        if (!pszPathName)
        {
            return -1;
        }

        struct sockaddr_un stTempAddr;
        memset((void*)&stTempAddr, 0, sizeof(stTempAddr));

        stTempAddr.sun_family = PF_LOCAL;
        ct_snprintf(CT_STRSIZE(stTempAddr.sun_path), "%s", pszPathName);

        unlink(pszPathName);
        return Listen((struct sockaddr*)&stTempAddr, sizeof(stTempAddr));
    }
#else
    int Listen(const char* CT_UNUSED(pszPathName))
    {
        return 0;
    }
#endif

    /** 接收连接 */
	int Accept(SOCKET& iAcceptSocket, sockaddr* pstAddr = NULL, socklen_t* piAddrLen = NULL)
	{
		if (!IsListen())
		{
			return -1;
		}

		int iRet = 0;
        if (IsUseSelect())
        {
		    iRet = CTSocketUtils::Select(m_iSocket, true, m_iConnTimeOut);
		    if (iRet)
		    {
			    return iRet;
		    }
        }

		iAcceptSocket = accept(m_iSocket, pstAddr, piAddrLen);
		if (iAcceptSocket == INVALID_SOCKET)
		{
			return CT_WSA_ERRNO;
		}

		iRet = CTSocketUtils::NonblockSocket(iAcceptSocket);
		if (iRet)
		{
			closesocket(iAcceptSocket);
			return CT_WSA_ERRNO;
		}

		return 0;
	}

    /**
    * 发送数据, 直到所有数据发送完或出错才返回.
    * @param[out] sendlen 已发送的数据的长度
    * @return 0=发送完成并成功
    */
	int Send(const char* buf, const int buflen, int& sendlen)
	{
		int iRet = 0;
		sendlen = 0;

		while (sendlen < buflen)
		{
            if (IsUseSelect())
            {
			    iRet = CTSocketUtils::Select(m_iSocket, false, m_iSendTimeOut);
			    if (iRet)
			    {
				    return iRet;
			    }
            }

			if (IsTCP())
			{
				iRet = send(m_iSocket, buf + sendlen, buflen - sendlen, 0);
			}
			else
			{
				iRet = sendto(m_iSocket, buf, buflen, 0,
							  (const struct sockaddr*)m_szAddr, m_iAddrLen);
			}

			if (iRet > 0)
			{
				sendlen += iRet;
			}
			else
			{
				int iErrNo = CT_WSA_ERRNO;
				if (iErrNo == EINTR)
				{
					continue;
				}
				else if (iErrNo == CT_EINPROGRESS || iErrNo == CT_EWOULDBLOCK)
				{
					continue;
				}
				else
				{
					return iErrNo;
				}
			}
		}

		return 0;
	}

    /**
    * 与Send()不同的是在CT_EINPROGRESS CT_EWOULDBLOCK的处理.
    * 遇到CT_EINPROGRESS CT_EWOULDBLOCK就返回
    * @param[out] sendlen 已发送的数据的长度
    * @return 0=发送完成并成功 CT_EINPROGRESS或CT_EWOULDBLOCK表示发送了一部分数据
    */
    int SendEx(const char* buf, const int buflen, int& sendlen)
    {
        int iRet = 0;
        sendlen = 0;

        while (sendlen < buflen)
        {
            if (IsUseSelect())
            {
                iRet = CTSocketUtils::Select(m_iSocket, false, m_iSendTimeOut);
                if (iRet)
                {
                    return iRet;
                }
            }

            if (IsTCP())
            {
                iRet = send(m_iSocket, buf + sendlen, buflen - sendlen, 0);
            }
            else
            {
                iRet = sendto(m_iSocket, buf, buflen, 0,
                    (const struct sockaddr*)m_szAddr, m_iAddrLen);
            }

            if (iRet > 0)
            {
                sendlen += iRet;
            }
            else
            {
                int iErrNo = CT_WSA_ERRNO;
                if (iErrNo == EINTR)
                {
                    continue;
                }
                else if (iErrNo == CT_EINPROGRESS || iErrNo == CT_EWOULDBLOCK)
                {
                    return iErrNo;
                }
                else
                {
                    return iErrNo;
                }
            }
        }

        return 0;
    }

    /**
    * 接收数据
    */
	int Recv(char* buf, const int buflen, int& recvlen)
	{
		int iRet = 0;
		recvlen = 0;

        if (IsUseSelect())
        {
		    iRet = CTSocketUtils::Select(m_iSocket, true, m_iRecvTimeOut);
		    if (iRet)
		    {
			    return iRet;
		    }
        }

		while (recvlen < buflen)
	    {
			if (IsTCP())
			{
				iRet = recv(m_iSocket, buf + recvlen, buflen - recvlen, 0);
			}
			else
			{
				iRet = recvfrom(m_iSocket, buf + recvlen, buflen - recvlen, 0, 0, 0);
			}

			if (iRet > 0)
			{
				recvlen += iRet;
			}
			else if (iRet == 0)
			{
                return CT_ECONNRESET;
			}
			else
	        {
				int iErrNo = CT_WSA_ERRNO;
				if (iErrNo == EINTR)
				{
					continue;
				}
				else if (iErrNo == CT_EINPROGRESS || iErrNo == CT_EWOULDBLOCK)
				{
					return 0;
				}
				else
				{
					return iErrNo;
				}
			}
	    }

		return 0;
	}

	void Close()
	{
		if (IsValid())
		{
			closesocket(m_iSocket);
			m_iSocket = INVALID_SOCKET;

#ifndef WIN32
            if (GetAF() == PF_LOCAL && IsListen())
            {
                struct sockaddr_un* pstTempAddr = (struct sockaddr_un*)m_szAddr;
                unlink(pstTempAddr->sun_path);
            }
#endif
		}
	}
};

} // namespace CTLib

#endif //__CTLIB_SOCKET_H__
