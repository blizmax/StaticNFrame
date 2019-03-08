/**
* Windows完成端口封装
*/

#ifndef __CTLIB_IOCP_H__
#define __CTLIB_IOCP_H__

#ifdef WIN32
#include "CTSocketUtils.h"

#include <Mswsock.h>
#pragma comment(lib, "Mswsock.lib")

namespace CTLib
{

enum
{
	IOCP_CLOSE = 0,
	IOCP_ACCEPT,
	IOCP_RECV,
	IOCP_SEND,
	IOCP_CONNECT,
	IOCP_MAX,	// 保证IOCP_MAX在最后
};

// 这个在Windows下无用
#define EPOLLIN		0x001
#define EPOLLOUT	0x004
#define EPOLLERR    0x008
#define EPOLLHUP    0x010

class CTEpoll
{
public:
	CTEpoll() : m_kdpfd(0), m_size(0), m_iWaitSize(0), m_astEvents(0) {}

	~CTEpoll() { Exit(); }

    typedef struct TSocketEventTag
    {
        unsigned int events;
        unsigned int data;
        unsigned int bytes;
    }TSocketEvent;

public:
	int Init(int epollsize, int waitsize)
	{
		m_size = epollsize;
		m_iWaitSize = waitsize;
		m_astEvents = new TSocketEvent[m_iWaitSize];
		if (!m_astEvents)
		{
			CT_ERROR(("new TSocketEvent[%d] failed: %d", m_iWaitSize, CT_ERRNO));
			return -1;
		}

		m_kdpfd = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (!m_kdpfd)
		{
			CT_ERROR(("CreateIoCompletionPort failed %d", CT_ERRNO));
			return -2;
		}

		return 0;
	}

	void Exit()
	{
		if (m_astEvents)
		{
			delete [] m_astEvents;
			m_astEvents = 0;
		}
		if (m_kdpfd > 0)
		{
			CloseHandle(m_kdpfd);
			m_kdpfd = 0;
		}
	}

	int Wait(int timeout)
	{
        DWORD dwNumberOfBytes;
        DWORD dwKey;
        LPOVERLAPPED lpOverlapped;

        int loop = 0;
		for (loop = 0; loop < m_iWaitSize; ++loop)
		{
			if (GetQueuedCompletionStatus(m_kdpfd, &dwNumberOfBytes, &dwKey, &lpOverlapped, timeout))
			{
				m_astEvents[loop].events = static_cast<unsigned int>(lpOverlapped - ms_envets);
				if (dwNumberOfBytes == 0)
				{
					if (m_astEvents[loop].events == IOCP_SEND || m_astEvents[loop].events == IOCP_RECV)
                    {
						m_astEvents[loop].events = IOCP_CLOSE;
                    }
				}
				m_astEvents[loop].data = dwKey;
				m_astEvents[loop].bytes = dwNumberOfBytes;
			}
			else
			{
				int err = static_cast<int>(CT_ERRNO);
				if (err == WAIT_TIMEOUT)
				{
					break;
				}
				else if (err == ERROR_NETNAME_DELETED ||
						 err == ERROR_CONNECTION_REFUSED ||
						 err == ERROR_CONNECTION_ABORTED ||
						 err == ERROR_SEM_TIMEOUT ||
						 err == ERROR_OPERATION_ABORTED) // @@
				{
					m_astEvents[loop].data = dwKey;
					m_astEvents[loop].bytes = dwNumberOfBytes;
					m_astEvents[loop].events = IOCP_CLOSE;
				}
				else
				{
					CT_ERROR(("GetQueuedCompletionStatus failed %d", err));
					return -1;
				}
			}
		}

		return loop;
	}

	int GetData(int i) const
	{
		return m_astEvents[i].data;
	}

	unsigned int GetEvent(int i) const
	{
		return m_astEvents[i].events;
	}

	unsigned int GetBytes(int i) const
	{
		return m_astEvents[i].bytes;
	}

	int Add(SOCKET s, int data, int = 0)
	{
		HANDLE t = CreateIoCompletionPort((HANDLE)s, m_kdpfd, data, 0);
		if (!t)
		{
			CT_ERROR(("CreateIoCompletionPort failed %d", CT_ERRNO));
			return -1;
		}
		return 0;
	}

	int Del(SOCKET = 0, int = 0, int = 0) { return 0; }
	int Mod(SOCKET = 0, int = 0, int = 0) { return 0; }

	void ZeroOverlapped(int events)
	{
		memset(&ms_envets[events], 0, sizeof(ms_envets[events]));
	}

	LPWSAOVERLAPPED GetOverlapped(int events)
	{
		return &(ms_envets[events]);
	}

protected:
	WSAOVERLAPPED			ms_envets[IOCP_MAX];
	HANDLE					m_kdpfd;
	int						m_size;
	int						m_iWaitSize;
	TSocketEvent*			m_astEvents;
	TSocketEvent			m_stEvent;
};

}

#endif //WIN32

#endif //__CTLIB_IOCP_H__
