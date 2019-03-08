
#ifndef __CTLIB_THREADPOOL_H__
#define __CTLIB_THREADPOOL_H__

#include "CTThread.h"

namespace CTLib
{

class CTThreadPool
{
protected:
	enum { ETP_MAX_THREAD = 1024 };

	typedef enum enThreadState
	{
		ETS_EMPTY = 0,
		ETS_IDLE,
		ETS_ALIVE
	} EThreadState;

	typedef struct tagThreadNode
	{
		pthread_t		stThread;
		EThreadState	enState;
	}TThreadNode;

	int				m_iMaxThread;
	int				m_iMinThread;
	int				m_iMaxIdle;
	int				m_iMinIdle;

	TThreadNode		m_iaThreadState[ETP_MAX_THREAD];
	CHThreadMutex	m_stMutex;

public:
	CHThreadPool() :
		m_iMaxThread(0),
		m_iMinThread(0),
		m_iMaxIdle(0),
		m_iMinIdle(0)
	{
		CHThreadMutexAuto automutex (m_stMutex);
		for (int i = 0; i < CT_COUNTOF(m_iaThreadState); ++i)
		{
			m_iaThreadState[i].stThread = 0;
			m_iaThreadState[i].enState = 0;
		}
	}

	virtual ~CHThreadPool()
	{
		Exit();
	}

	int Init(int iMaxThread, int iMinThread, int iMaxIdle, int iMinIdle)
	{
		if (iMaxThread <= 0 || iMinThread <= 0 || iMaxIdle <= 0 || iMinIdle <= 0 ||
			iMaxThread < iMinIdle || iMaxIdle < iMinIdle)
		{
			CT_ERROR(("Invalid Param"));
			return -1;
		}

		m_iMaxThread = CT_MAX(iMaxThread, iMaxIdle);
		m_iMinThread = CT_MAX(iMinThread, iMinIdle);
		m_iMaxIdle = iMaxIdle;
		m_iMinIdle = iMinIdle;

		// 创建线程池
		int iRet = CreateThread(m_iMinThread);
		return iRet;
	}

	int Exit()
	{
		return 0;
	}

	int Check()
	{
		return 0;
	}

	int Dispatch()
	{

	}

protected:
	int CreateThread(int iCount)
	{
		
	}

	int KillThread()
	{

	}

	virtual void Run() = 0;
	static void ThreadRun()
};

} // namespace

#endif //__CTLIB_THREADPOOL_H__
