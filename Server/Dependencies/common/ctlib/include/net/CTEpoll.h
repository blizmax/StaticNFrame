/**
* Linux Epoll封装类
*/

#ifndef __CTLIB_EPOLL_H__
#define __CTLIB_EPOLL_H__

#include "CTBase.h"
#include "CTSocket.h"

#ifndef WIN32
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/epoll.h>
#else
    // 为了能在Windows下编译
    #define EPOLLIN		0x001
    #define EPOLLOUT	0x004
    #define EPOLLERR    0x008
    #define EPOLLHUP    0x010
    #define EPOLLET (1 << 31)

    #define EPOLL_CTL_ADD 1 /* Add a file decriptor to the interface.  */
    #define EPOLL_CTL_DEL 2 /* Remove a file decriptor from the interface.  */
    #define EPOLL_CTL_MOD 3 /* Change file decriptor epoll_event structure.  */

    #define epoll_create(x) (0)
    #define epoll_ctl(epfd, op, fd, event) (0)
    #define epoll_wait(epfd, event, waitsize, timeout) (timeout)

    typedef union epoll_data
    {
        void *ptr;
        int fd;
        unsigned int u32;
        uint64 u64;
    } epoll_data_t;

    struct epoll_event
    {
        unsigned int events;      /* Epoll events */
        epoll_data_t data;      /* User data variable */
    };
#endif

namespace CTLib
{

class CTEpoll;
class CTEpollObjectInf
{
    friend class CTEpoll;
protected:
    // 这里声明这两个对象，是为了在Epoll的WaitAndEvent中校验对象是否合法
    CTEpoll*            m_pstEpoll;
    SOCKET              m_iSocket;

public:
    CTEpollObjectInf(): m_pstEpoll(NULL), m_iSocket(INVALID_SOCKET) {}
    virtual ~CTEpollObjectInf() {}

protected:
    virtual void OnEpollEvent(int iEvent) = 0;
};

/**
* Epoll封装类
*/
class CTEpoll
{
public:
    CTEpoll() : m_kdpfd(0), m_size(0), m_iWaitSize(0), m_astEvents(0) {}
	virtual ~CTEpoll() { Exit(); }

    typedef struct epoll_event TSocketEvent;

public:
	int Init(int iEpollSize, int iWaitSize)
	{
		m_size = iEpollSize;
		m_iWaitSize = iWaitSize;
		m_astEvents = new TSocketEvent[m_iWaitSize];
		if (!m_astEvents)
		{
			CT_ERROR(("new TSocketEvent[%d] failed: %d", m_iWaitSize, errno));
			return -1;
		}

		m_kdpfd = epoll_create(m_size);
		if (m_kdpfd < 0)
		{
			CT_ERROR(("epoll_create failed: %d", errno));
			return -2;
		}

        CT_TRACE(("init epoll ok, epoll = %d", m_kdpfd));
		return 0;
	}

    /**
    * 等待事件发生或超时
    * @param[in] iTimeOut 等待的超时时限，单位毫秒
    * @return <0表示出错 =0表示没有时间 >0表示收到并处理的事件个数
    */
	int Wait(int iTimeOut)
	{
		return epoll_wait(m_kdpfd, m_astEvents, m_iWaitSize, iTimeOut);
	}

    /**
    * 等待事件发生或超时，并调用方法.
    * @param[in] iTimeOut 等待的超时时限，单位毫秒
    * @return <0表示出错 =0表示没有时间 >0表示收到并处理的事件个数
    */
    int WaitAndEvent(int iTimeOut)
    {
        int iEventCount = Wait(iTimeOut);
        if (iEventCount < 0)
        {
            return iEventCount;
        }
        else if (iEventCount == 0) // 超时了
        {
            return 0;
        }


        for (int i = 0; i < iEventCount; ++i)
        {
            uint64 ullData = GetData(i);

            //在64位系统下由于ullData只能存放一个指针，所以此处不在进行复制和后续的检查操作
            //unsigned int uiSocket = CT_UINT64_HIGH(ullData);

#ifdef CT_BIT64
            CTEpollObjectInf* pstObjectPos = (CTEpollObjectInf*)ullData;
#else
            CTEpollObjectInf* pstObjectPos = (CTEpollObjectInf*)(CT_UINT64_LOW(ullData));
#endif

            unsigned int uiEvent = GetEvent(i);

            // 判断对象是否合法
            if (pstObjectPos == NULL ||
                //pstObjectPos->m_iSocket != (int)uiSocket ||
                pstObjectPos->m_pstEpoll != this)
            {
                CT_WARNING(("Epoll.Data(%p) invalid, event=%u", pstObjectPos, GetEvent(i)));
                continue;   // 这里不return/break，把所有事件处理完
            }

            pstObjectPos->OnEpollEvent(uiEvent);
        }

        return iEventCount;
    }

	uint64 GetData(int i) const
	{
		CT_ASSERT(i < m_iWaitSize);
		return m_astEvents[i].data.u64;
	}

	unsigned int GetEvent(int i) const
	{
		CT_ASSERT(i < m_iWaitSize);
		return m_astEvents[i].events;
	}

    static bool IsInputEvent(int iEvent) { return (iEvent & EPOLLIN) != 0; }
    static bool IsOutputEvent(int iEvent) { return (iEvent & EPOLLOUT) != 0; }
    static bool IsCloseEvent(int iEvent) { return (iEvent & (EPOLLHUP|EPOLLERR)) != 0; }

	int Add(SOCKET s, uint64 data, unsigned int event)
	{
		m_stEvent.events = event|EPOLLERR|EPOLLHUP;
		m_stEvent.data.u64 = data;
		int iRet = epoll_ctl(m_kdpfd, EPOLL_CTL_ADD, s, &m_stEvent);
        if (iRet)
        {
            CT_WARNING(("epoll_ctl(add) failed: %d, epoll = %d, socket=%d", errno, m_kdpfd, s));
        }

        return iRet;
	}

	int Del(SOCKET s, uint64 data = 0, int event = EPOLLIN)
	{
		m_stEvent.events = 0;
		m_stEvent.data.u64 = data;
		int iRet = epoll_ctl(m_kdpfd, EPOLL_CTL_DEL, s, &m_stEvent);
        if (iRet)
        {
            CT_WARNING(("epoll_ctl(del) failed: %d, epoll = %d, socket=%d", errno, m_kdpfd, s));
        }

        return iRet;
	}

	int Mod(SOCKET s, uint64 data, int event)
	{
		m_stEvent.events = event|EPOLLERR|EPOLLHUP;
		m_stEvent.data.u64 = data;
		int iRet = epoll_ctl(m_kdpfd, EPOLL_CTL_MOD, s, &m_stEvent);
        if (iRet)
        {
            CT_WARNING(("epoll_ctl(mod) failed: %d, epoll = %d, socket=%d", errno, m_kdpfd, s));
        }

        return iRet;
	}

protected:
    void Exit()
    {
        if (m_astEvents)
        {
            delete [] m_astEvents;
            m_astEvents = 0;
        }
        if (m_kdpfd > 0)
        {
            close(m_kdpfd);
            m_kdpfd = 0;
        }
    }

protected:
	int				m_kdpfd;
	int				m_size;
	int				m_iWaitSize;
	TSocketEvent*	m_astEvents;
	TSocketEvent	m_stEvent;
};

/**
* 要使用Epoll的类从这个类继承,
* 用模板的方式来实现类似委托的功能
*/
template<typename Owner>
class CTEpollObject: public CTEpollObjectInf
{
    friend class CTEpoll;
public:
    typedef void (Owner::*PF_EPOLL_EVENT)(CTEpollObject* pstObject, SOCKET iSocket, int iEvent);
    unsigned int GetActID() const {return m_iAct;}

protected:
    Owner*              m_pstOwner;
    PF_EPOLL_EVENT      m_pfEvent;
    unsigned int        m_iRegEvent;
    unsigned int        m_iAct;

public:
    CTEpollObject(): m_pstOwner(NULL), m_pfEvent(NULL), m_iRegEvent(0), m_iAct(0) {}
    virtual ~CTEpollObject() { Unregister(); }

    /**
    * 注册到Epoll中
    */
    int Register(Owner& stOwner, PF_EPOLL_EVENT pfEvent, CTEpoll& stEpoll, SOCKET iSocket, unsigned int iRegEvent, unsigned int iAct = 0)
    {
        CT_ASSERT(iSocket != INVALID_SOCKET && iRegEvent > 0 && pfEvent != NULL && iAct >= 0);

        int iRet = 0;

        iRet = Unregister();
        if (iRet)
        {
            return iRet;
        }

        m_pstEpoll = &stEpoll;
        m_pstOwner = &stOwner;
        m_pfEvent = pfEvent;
        m_iRegEvent = iRegEvent;
        m_iAct = iAct;
        m_iSocket = iSocket;

        uint64 ullData = CreateData(m_iSocket);
        iRet = m_pstEpoll->Add(m_iSocket, ullData, m_iRegEvent);

        return iRet;
    }

    /**
    * 更改关注的事件
    */
    int ModRegEvent(int iRegEvent)
    {
        m_iRegEvent = iRegEvent;
        if (m_pstEpoll)
        {
            uint64 ullData = CreateData(m_iSocket);
            return m_pstEpoll->Mod(m_iSocket, ullData, m_iRegEvent);
        }

        return 0;
    }

protected:
    virtual void OnEpollEvent(int iEvent)
    {
        CT_ASSERT(m_pstOwner != NULL && m_pfEvent != NULL);
        (m_pstOwner->*m_pfEvent)(this, m_iSocket, iEvent);
    }

    int Unregister()
    {
        int iRet = 0;
        if (m_pstEpoll)
        {
            CT_TRACE(("del %d from epoll", m_iSocket));
            iRet = m_pstEpoll->Del(m_iSocket);
            m_pstEpoll = NULL;
        }

        m_pstOwner = NULL;
        m_pfEvent = NULL;
        m_iRegEvent = 0;
        m_iAct = 0;
        m_iSocket = INVALID_SOCKET;

        return iRet;
    }

    int Reregister(CTSocket& stSocket)
    {
        int iRet = 0;
        if (!m_pstEpoll)
        {
            return -1;
        }

        m_pstEpoll->Del(m_iSocket);

        stSocket.Create();
        m_iSocket = stSocket.GetSocket();

        uint64 ullData = CreateData(m_iSocket);
        iRet = m_pstEpoll->Add(m_iSocket, ullData, EPOLLOUT);
        return iRet;
    }

    uint64 CreateData(SOCKET iSocket)
    {
#ifdef CT_BIT64
        return (uint64)this;
#else
        return CT_UINT64_MAKE(iSocket, (unsigned int)this);
#endif
    }

};

} //namespace CTLib

#endif //__CTLIB_EPOLL_H__
