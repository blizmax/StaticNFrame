
#ifndef __CTLIB_THREAD_H__
#define __CTLIB_THREAD_H__

#include "base/CTException.h"
#include <pthread.h>
#ifdef WIN32
	#ifdef _DEBUG
		#pragma comment(lib, "pthreadVC2d.lib")
	#else
		#pragma comment(lib, "pthreadVC2.lib")
	#endif
#endif

#define CT_THROW_THREAD(x)  ((void)(0)) //@@

namespace CTLib
{

//=====================================
class CTThreadCond;
class CTThreadMutex
{
	friend class CTThreadCond;

protected:
	pthread_mutex_t m_stMutex;

public:
	CTThreadMutex()
	{
		int iRet = pthread_mutex_init(&m_stMutex, NULL);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_mutex_init:%s:%d", strerror(iRet), iRet));
		}
	}

	virtual ~CTThreadMutex()
	{
		int iRet = pthread_mutex_destroy(&m_stMutex);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_mutex_destroy:%s:%d", strerror(iRet), iRet));
		}
	}

	void Lock()
	{
		int iRet = pthread_mutex_lock(&m_stMutex);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_mutex_lock:%s:%d", strerror(iRet), iRet));
		}
	}

	bool TryLock()
	{
		int iRet = pthread_mutex_trylock(&m_stMutex);
		if (iRet)
		{
			if (iRet == EBUSY)
			{
				return false;
			}

			CT_THROW_THREAD(("pthread_mutex_trylock:%s:%d", strerror(iRet), iRet));
		}
		return true;
	}

	void Unlock()
	{
		int iRet = pthread_mutex_unlock(&m_stMutex);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_mutex_unlock:%s:%d", strerror(iRet), iRet));
		}
	}
};

//=====================================
class CTThreadMutexAuto
{
public:
	CTThreadMutexAuto(CTThreadMutex& stMutex) :
		m_stMutex (stMutex)
	{
		m_stMutex.Lock();
	}

	~CTThreadMutexAuto()
	{
		m_stMutex.Unlock();
	}

protected:
	CTThreadMutex&	m_stMutex;
};

//=====================================
class CTThreadCond
{
protected:
	pthread_cond_t m_stCond;
	
public:
	CTThreadCond()
	{
		int iRet = pthread_cond_init(&m_stCond, NULL);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_cond_init:%s:%d", strerror(iRet), iRet));
		}
	}

	virtual ~CTThreadCond()
	{
		int iRet = pthread_cond_destroy(&m_stCond);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_cond_destroy:%s:%d", strerror(iRet), iRet));
		}
	}
	
	void Signal()
	{
		int iRet = pthread_cond_signal(&m_stCond);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_cond_signal:%s:%d", strerror(iRet), iRet));
		}
	}

	void Broadcast()
	{
		int iRet = pthread_cond_broadcast(&m_stCond);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_cond_broadcast:%s:%d", strerror(iRet), iRet));
		}
	}

	void Wait(CTThreadMutex& stThMutex)
	{
		int iRet = pthread_cond_wait(&m_stCond, &stThMutex.m_stMutex);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_cond_wait:%s:%d", strerror(iRet), iRet));
		}
	}

	bool TimedWait(CTThreadMutex& stThMutex, const struct timespec* pstAbsTime)
	{
		int iRet = pthread_cond_timedwait(&m_stCond, &stThMutex.m_stMutex, pstAbsTime);
		if (iRet)
		{
			if (iRet == ETIMEDOUT || iRet == CT_EINTR)
			{
				return false;
			}

			CT_THROW_THREAD(("pthread_cond_timedwait:%s:%d", strerror(iRet), iRet));
		}

		return true;
	}
};

//=====================================
class CTThread
{
protected:
	pthread_attr_t	m_stThreadAttr;
	pthread_t		m_stThread;
	bool			m_bDetach;
	bool			m_bStart;

public:
	CTThread(bool bDetach = true)
	{
		m_bDetach = bDetach;
		m_bStart = false;
		
		pthread_attr_init(&m_stThreadAttr);
		if (m_bDetach)
		{
			pthread_attr_setdetachstate(&m_stThreadAttr, PTHREAD_CREATE_DETACHED);
		}
		else
		{
			pthread_attr_setdetachstate(&m_stThreadAttr, PTHREAD_CREATE_JOINABLE);
		}
	}

	virtual ~CTThread()
	{
		pthread_attr_destroy(&m_stThreadAttr);
		OnDelete();
	}

	void Start()
	{
		if (m_bStart)
		{
			return;
		}

		int iRet = pthread_create(&m_stThread, &m_stThreadAttr, 
								(void* (*) (void*))ThreadRun, this);
		if (iRet)
		{
			CT_THROW_THREAD(("pthread_create:%s:%d", strerror(iRet), iRet));
		}
        
		m_bStart = true;
	}

    bool IsEnd()
    {
        return (m_bStart != true);
    }

protected:
	virtual void Run() = 0;

	virtual void OnEnd() {}

	virtual void OnException(CTException& e) {}

	virtual void OnDelete()
	{
#ifdef WIN32
		if (m_bStart && pthread_self().x != m_stThread.x)	//end by another thread
#else
		if (m_bStart && pthread_self() != m_stThread)	//end by another thread
#endif
		{
			int iRet = pthread_cancel(m_stThread);
			if (iRet)
			{
				CT_THROW_THREAD(("pthread_cancel:%s:%d", strerror(iRet), iRet));
			}

			if (!m_bDetach)
			{
				int iRet = pthread_join(m_stThread, NULL);
				if (iRet)
				{
					CT_THROW_THREAD(("pthread_join:%s:%d", strerror(iRet), iRet));
				}
			}
		}
	}

protected:
	static void* ThreadRun(CTThread* pstThread)
	{
		try
		{
			pstThread->Run();
		}
		catch (CTException& e)
		{
			pstThread->OnException(e);
		}

		pstThread->m_bStart = false;
		pstThread->OnEnd();
		return NULL;
	}
};

} // namespace CTLib

#endif //__CTLIB_THREAD_H__
