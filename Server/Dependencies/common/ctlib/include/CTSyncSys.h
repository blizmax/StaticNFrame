#ifndef __CTLIB_SYNC_SYS__
#define __CTLIB_SYNC_SYS__

#ifdef WIN32
#include "CTNull.h"
#else
#include <pthread.h>
#endif

#include "CTObjectMgr.h"
#include "CTHashObjectMgr.h"
#include "CTMultiObjectMgr.h"

namespace CTLib
{

/**
* 等待队列类.
* 用于保存等待者和被等待的资源
*/
template<typename RESOURCE_KEY,
        typename WAITER,
        int WAITERCOUNT>
class CTWaitList
{
public:
    size_t CountSize(int iListLen)
    {
        return m_stDualHash.CountSize(iListLen, 2, 2);
    }

    int Init(const char* sBuf, int iListLen)
    {
        int iRet = m_stDualHash.Init(sBuf, iListLen, 2, 2);
        CHECK_THROW(iRet);
        return iRet;
    }

    /**
    * 判断资源是否正被等待.
    * @param[in] stResourceKey 资源标识
    * @return true为正被等待
    */
    bool HasBeenWaited(RESOURCE_KEY& stResourceKey)
    {
        return (m_stDualHash.HashFind(stResourceKey) >= 0);
    }

    /**
    * 判断自身是否正在等待资源.
    * 未实现
    */
    bool IsWaiting(WAITER& stWaiter)
    {
        return false;
    }

    /**
    * 资源等待函数.
    * @param[in] stWaiter 等待者标识
    * @param[in] stResourceKey 资源标识
    * @return 0为正常
    */
    int Wait(WAITER& stWaiter, RESOURCE_KEY& stResourceKey)
    {
        int iRet = 0;
        if (HasBeenWaited(stResourceKey))
        {
            iRet = AddToWaitList(stWaiter, stResourceKey);
        }
        else
        {
            iRet = AllocWaitList(stWaiter, stResourceKey);
        }

        if (iRet)
        {
            CT_WARNING((""));
        }

        return 0;
    }

    /**
    * 获取等待队列的长度.
    * 只有首个等待资源的等待者才能获得该数据
    * @param[in] stWaiter 等待者标识
    * @param[in] stResourceKey 资源标识
    * @return >0表示等待队列的长度
    */
    int WaitMeCount(WAITER& stWaiter, RESOURCE_KEY& stResourceKey)
    {
        int iDualIndex = m_stDualHash.HashFind(stResourceKey);
        if (iDualIndex < 0)
        {
            return -1;
        }
        
        if (m_stDualHash[iDualIndex][0] != stWaiter)
        {
            return -1;
        }
        else
        {
            return m_stDualHash[iDualIndex].m_iUsedCount - 1;
        }
    }

    /**
    * 获取等待队列中的等待者.
    * 主要用于资源调用返回以后唤醒其他等待该资源的等待者
    * @param[in] stResourceKey 资源标识
    * @param[in] iIndex 等待者下标 
    * @param[in] stWaiter 等待者
    * @return 0表示成功
    */
    int GetWaiter(RESOURCE_KEY& stResourceKey, int iIndex, WAITER& stWaiter)
    {
        int iDualIndex = m_stDualHash.HashFind(stResourceKey);
        if (iDualIndex< 0 || iIndex >= m_stDualHash[iDualIndex].m_iUsedCount)
        {
            return -1;
        }

        stWaiter = m_stDualHash[iDualIndex][iIndex];
        return 0;
    }

    /**
    * 释放等待资源的等待队列.
    * @param[in] stResourceKey 资源标识
    * @return 0表示成功
    */
    int FreeList(RESOURCE_KEY& stResourceKey)
    {
        int iDualIndex = m_stDualHash.HashFind(stResourceKey);
        if (iDualIndex<0)
        {
            return -1;
        }

        m_stDualHash[iDualIndex].m_iUsedCount = 0;
        m_stDualHash.HashFreeByIndex(iDualIndex);
        return 0;
    }

private:
    typedef CTArray<WAITER, WAITERCOUNT> DUAL_HASH_VALUE;
    CTHashObjectMgr<RESOURCE_KEY, DUAL_HASH_VALUE> m_stDualHash;     ///< 防止二次重入用hash

    /**
    * 分配等待队列.
    * @param[in] stWaiter 等待者
    * @param[in] stResourceKey 资源标识
    * @return 0表示成功
    */
    int AllocWaitList(WAITER& stWaiter, RESOURCE_KEY& stResourceKey)
    {
        int iDualIndex = m_stDualHash.HashAlloc(stResourceKey);
        if (iDualIndex < 0)
        {
            return -1;
        }

        m_stDualHash[iDualIndex][0] = stWaiter;
        m_stDualHash[iDualIndex].m_iUsedCount = 1;

        //CT_TRACE(("create wait list %d", iDualIndex));
        return 0;
    }

    /**
    * 添加到等待队列.
    * @param[in] stWaiter 等待者
    * @param[in] stResourceKey 资源标识
    * @return 0表示成功
    */
    int AddToWaitList(WAITER& stWaiter, RESOURCE_KEY& stResourceKey)
    {
        int iDualIndex = m_stDualHash.HashFind(stResourceKey);
        if (iDualIndex < 0)
        {
            return -1;
        }

        if (m_stDualHash[iDualIndex].m_iUsedCount >= WAITERCOUNT)
        {
            return -1;
        }

        m_stDualHash[iDualIndex][m_stDualHash[iDualIndex].m_iUsedCount] = stWaiter;
        m_stDualHash[iDualIndex].m_iUsedCount++;
        //CT_TRACE(("add waiter to wait list %d, len = %d", iDualIndex, m_stDualHash[iDualIndex].m_iUsedCount));

        return 0;
    }
};

/**
* 模拟同步系统类.
* 用于在异步server中做操作同步
*/
class CTSyncSys
{
public:
	typedef void* CTLIB_SYNC_RUN_FUNC(void*);
	enum CTLIB_SyncSysThreadStat
	{
		CTLIB_SYNCSYS_THREAD_FREE = 0,
		CTLIB_SYNCSYS_THREAD_IDLE,
		CTLIB_SYNCSYS_THREAD_BUSY,
		CTLIB_SYNCSYS_THREAD_READY,
		CTLIB_SYNCSYS_THREAD_WAIT,
	};

	CTSyncSys()
		:m_iThreadCount(0), m_pfRunFunc(NULL), m_bIsExit(false), m_bThreadCreateEnd(false)
	{

	}

    virtual ~CTSyncSys()
    {
    }

	virtual size_t CountSize(int iThreadCount, int iMaxCount) //为了确保使用的共享内存大小不变，此处根据可能出现的线程数量最大值计算hash size
	{
		if (iThreadCount <= 0)
		{
			return -1;
		}

		m_iThreadCount = iThreadCount;

		//计算需要的内存空间
		m_iThreadCondMgrSize = m_stThreadCondMgr.CountSize(iMaxCount, sizeof(pthread_cond_t), 5, 1);
        m_iThreadHashSize = m_stThreadHash.CountSize(iMaxCount, 2, 2);
        m_iWaitListSize = m_stWaitList.CountSize(iMaxCount);

		return m_iThreadCondMgrSize + m_iThreadHashSize + m_iWaitListSize;
	}

	virtual int Init(CTLIB_SYNC_RUN_FUNC* pfRunFunc, void* pArg, char* sBuff)
	{
		if (m_iThreadCount <= 0)
		{
			return -1;
		}

		m_pfRunFunc = pfRunFunc;
		m_pArg = pArg;
		
        int iBufOffset = 0;

		int iRet = m_stThreadCondMgr.Init(sBuff + iBufOffset, m_iThreadCount, sizeof(pthread_cond_t), 5, 1);
        iBufOffset += m_iThreadCondMgrSize;
		CHECK_THROW(iRet);

		iRet = m_stThreadHash.Init(sBuff + iBufOffset, m_iThreadCount, 2, 2);
        iBufOffset += m_iThreadHashSize;
		CHECK_THROW(iRet);

        iRet = m_stWaitList.Init(sBuff + iBufOffset, m_iThreadCount);
        CHECK_THROW(iRet);

        iRet = pthread_mutex_init(&m_stCondMutex, NULL);
        CHECK_THROW(iRet);

		return 0;
	}

	int Run()
	{
		if (m_iThreadCount <= 0)
		{
			return -1;
		}

		int iRet = 0;
		
		//创建线程
		iRet = CreateThread();
		if (iRet)
		{
			return iRet;
		}

		//启动一个线程
		while (m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_IDLE) != m_iThreadCount)
		{
			usleep(10*1000);
		}
		AwakeIdleWork();
		
		//等待线程结束
		return WaitThreadExit();
	}

    /**
    * 等待函数.
    * @param[in] iTimeOut 等待秒数
    * @return 0为正常
    */
	int Wait(int iTimeOut)
	{
		//检查是否有空闲的线程
		if (m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_READY) == 0 
            && m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_IDLE) == 0)
		{
			CT_WARNING(("no free thread!"));
			return -1;
		}

        int iRet = 0;
        //将空闲或者ready线程切换成工作状态。优先切换ready线程
		if (HasReadyWork())
		{
			AwakeReadyWork();
		}
		else
		{
			iRet = AwakeIdleWork();
            if (iRet)
            {
                return iRet;
            }
		}

		iRet = GotoWait(iTimeOut);
		GotoReady();
		return iRet;
	}

	void DumpInfo(CTLog* pstLog)
	{
		pstLog->Log(EInfo, "Thread: All=%d Idle=%d Busy=%d Ready=%d Wait=%d\n",
			m_iThreadCount,
			m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_IDLE),
			m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_BUSY),
			m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_READY),
			m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_WAIT));
	}

	pthread_t GetMyThreadID()
	{
		return pthread_self();
	}

    /**
    * WaitStart.
    * 在所有线程被创建完毕以后进入空闲状态
    * 如果直接执行GotoIdle()，会因为m_stThreadHash.HashFind失败而出错
    */
	int WaitStart()
	{
		while (!m_bThreadCreateEnd)
		{
			usleep(10 * 1000);
		}
		return GotoIdle();
	}

    /**
    * 切换到idle状态.
    * @return 0为正常
    */
	int GotoIdle()
	{
		int iIndex = m_stThreadHash.HashFind(GetMyThreadID());
		CT_ASSERT(iIndex >= 0);

		pthread_mutex_lock(&m_stCondMutex);
		//CT_TRACE(("put thread(index = %d, stat = %d) to idle", m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex])));
		m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_IDLE);
		int iRet = pthread_cond_wait(&m_stThreadCondMgr[m_stThreadHash[iIndex]], &m_stCondMutex);
		m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_BUSY);
		//CT_TRACE(("idle thread(index = %d, now stat = %d) wait end, ret = %d", 
        //    m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex]), iRet));
		pthread_mutex_unlock(&m_stCondMutex);

		return iRet;
	}

    /**
    * 切换到wait状态.
    * 该函数退出后线程仍处于wait状态，需要在函数退出后将线程置为等待状态
    * @return 0为正常
    */
	int GotoWait(int iTimeOut)
	{
        unsigned long int ulThreadID = GetMyThreadID();
		int iIndex = m_stThreadHash.HashFind(ulThreadID);
		CT_ASSERT(iIndex >= 0);

		pthread_mutex_lock(&m_stCondMutex);
		CT_ERROR(("put thread(index = %d, stat = %d, id = %ul) to wait", 
				m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex]), ulThreadID));
		m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_WAIT);

        struct timeval stStartTime;
        gettimeofday(&stStartTime,NULL);
        struct timespec stTimeTo;
        stTimeTo.tv_sec = stStartTime.tv_sec + iTimeOut;
        stTimeTo.tv_nsec = stStartTime.tv_usec;
		int iRet = pthread_cond_timedwait(&m_stThreadCondMgr[m_stThreadHash[iIndex]], &m_stCondMutex, &stTimeTo);
        //CT_TRACE(("thread(index = %d, stat = %d) wait end ,ret = %d", 
        //    m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex]), iRet));
        //m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_WAIT);
		pthread_mutex_unlock(&m_stCondMutex);
		return iRet;
	}

    /**
    * 切换到ready状态.
    * @return 0为正常
    */
	int GotoReady()
	{
		int iIndex = m_stThreadHash.HashFind(GetMyThreadID());
		CT_ASSERT((iIndex>=0));

		pthread_mutex_lock(&m_stCondMutex);
		//CT_TRACE(("put thread(index = %d, stat = %d) to ready", m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex])));
		m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_READY);
		pthread_cond_wait(&m_stThreadCondMgr[m_stThreadHash[iIndex]], &m_stCondMutex);
		m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_BUSY);
		//CT_TRACE(("ready thread(index = %d, now stat = %d) wait end", m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex])));
		pthread_mutex_unlock(&m_stCondMutex);

		return 0;
	}

    /**
    * WorkDone.
    * 在run中判断是否有ready的线程需要被执行，如果有则将当前线程置为idle状态，并将一个ready线程唤醒
    */
	int WorkDone()
	{
		if (m_bIsExit)
		{
			return 0;
		}

		while (HasReadyWork())
		{
			//CT_TRACE(("has ready work, change stat"));
			AwakeReadyWork();
			GotoIdle();
		}

		return 0;
	}

    /**
    * 唤醒wait线程.
    * @param[in] iTid 线程号
    * @return 0为正常
    */
	int AwakeWaitWork(pthread_t iTid)
	{
		int iIndex  = m_stThreadHash.HashFind(iTid);
		if (iIndex < 0)
		{
            return -1;
		}

		if (m_stThreadCondMgr.Flag(m_stThreadHash[iIndex]) != CTLIB_SYNCSYS_THREAD_WAIT)
		{
			return -2;
		}
		
		pthread_mutex_lock(&m_stCondMutex);
		//CT_TRACE(("awake wait thread(index = %d, now stat = %d)", 
		//		m_stThreadHash[iIndex], m_stThreadCondMgr.Flag(m_stThreadHash[iIndex])));
		pthread_cond_signal(&m_stThreadCondMgr[m_stThreadHash[iIndex]]);
		pthread_mutex_unlock(&m_stCondMutex);
		return 0;
	}

    /**
    * Exit.
    * busy线程收到退出信号以后负责唤醒所有其他线程
    * 如果有线程处于wait状态，需等待超时
    */
	int Exit()
	{
		if (m_bIsExit)
		{
			GotoExit();
			return 0;
		}
		m_bIsExit = true;
		
		while (m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_FREE) != (m_iThreadCount - 1))
		{
			/*
			CT_TRACE(("thread exit, idle = %d, ready = %d, busy = %d, wait = %d, exitcount = %d", 
				m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_IDLE), 
				m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_READY), 
				m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_BUSY), 
				m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_WAIT), 
				iExitCount));
			*/

			if (m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_IDLE))
			{
				AwakeIdleWork();
			}
			else if (m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_READY))
			{
				AwakeReadyWork();
			}
			
            //线程状态切换需要时间，所以此处sleep一段时间
			usleep(10*1000);
		}
		return 0;
	}

	bool IsExit()
	{
		return m_bIsExit;
	}

    /**
    * 唤醒idle线程.
    * @return 0为正常
    */
	int AwakeIdleWork()
	{
		pthread_mutex_lock(&m_stCondMutex);
        if (m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_IDLE) <= 0) //没有空闲的线程
        {
            return -1;
        }
		int iIndex = m_stThreadCondMgr.Head(CTLIB_SYNCSYS_THREAD_IDLE);

        //CT_TRACE(("awake idle thread(index = %d, now stat = %d)", 
        //            iIndex, m_stThreadCondMgr.Flag(m_stThreadHash[iIndex])));
		pthread_cond_signal(&m_stThreadCondMgr[iIndex]);
		//CT_TRACE(("awake idle thread, index = %d", iIndex));
		pthread_mutex_unlock(&m_stCondMutex);
		return 0;
	}

    /**
    * 唤醒ready线程.
    * 最先进入ready队列的线程先被唤醒
    * @return 0为正常
    */
    int AwakeReadyWork()
    {
        pthread_mutex_lock(&m_stCondMutex);
        CT_ASSERT(m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_READY) > 0);
        int iIndex = m_stThreadCondMgr.Head(CTLIB_SYNCSYS_THREAD_READY);

        //CT_TRACE(("awake ready thread(index = %d, now stat = %d)", 
        //    iIndex, m_stThreadCondMgr.Flag(m_stThreadHash[iIndex])));
        pthread_cond_signal(&m_stThreadCondMgr[iIndex]);
        //CT_TRACE(("awake ready thread, index = %d", iIndex));
        pthread_mutex_unlock(&m_stCondMutex);

        return 0;
    }

    /**
    * 判断是否有ready线程.
    * @return true为存在ready线程
    */
    bool HasReadyWork()
    {
        return m_stThreadCondMgr.Size(CTLIB_SYNCSYS_THREAD_READY) > 0;
    }

    /**
    * 判断资源是否有被等待.
    * 防止二次调用
    * @param[in] iKey 资源标识
    * @return true表示资源正被等待
    */
    int HasBeenWaited(unsigned int iKey)
    {
        return m_stWaitList.HasBeenWaited(iKey);
    }

    /**
    * 用于防止二次调用用的等待函数.
    * @param[in] iTimeout 超时秒数
    * @param[in] iKey 资源标识
    * @return true表示资源正被等待
    */
    int Wait(int iTimeout, unsigned int iKey)
    {
        //将等待获取的资源放入等待队列
        pthread_t iMineThreadID = GetMyThreadID();
        int iRet = m_stWaitList.Wait(iMineThreadID, iKey);
        if (iRet != 0)
        {
            CT_WARNING((""));
            return -1;
        }

        //CT_TRACE(("begin wait"));
        iRet = Wait(iTimeout);

        int iWaitMeCount = m_stWaitList.WaitMeCount(iMineThreadID, iKey);
        //CT_TRACE(("iWaitMeCount = %d", iWaitMeCount));

        //唤醒其他正在等待的线程
        if (iWaitMeCount >= 0)
        {
            pthread_t iWaitThreadID = 0;
            for (int i=0; i<iWaitMeCount; i++)
            {
                if (m_stWaitList.GetWaiter(iKey, i+1, iWaitThreadID) != 0)
                {
                    break;
                }
                AwakeWaitWork(iWaitThreadID);
                //CT_TRACE(("awake thread %ul", iWaitThreadID));
            }
            m_stWaitList.FreeList(iKey);
        }
        

        return iRet;
    }

    bool IsValidThreadID(pthread_t iTid)
    {
        return m_stThreadHash.HashFind(iTid) >= 0;
    }

protected:
    enum MAX_THERAD_COUNT_tag
    {
        MAX_THERAD_COUNT = 100
    };
	int							m_iThreadCount;
private:
	pthread_mutex_t				m_stCondMutex;          ///< 用于状态变更

	CTObjectMgr<pthread_cond_t>	m_stThreadCondMgr;      ///< 保存线程等待时的条件变量
	int							m_iThreadCondMgrSize;   ///< 保存m_stThreadCondMgr所需要的内存
	CTHashObjectMgr<pthread_t, int> m_stThreadHash;     ///< 与m_stThreadCondMgr一起使用，用于从thread id到pthread_cond_t的映射
    int                         m_iThreadHashSize;      ///< 保存m_stThreadHash所需要的内存

    CTWaitList<unsigned int, pthread_t, MAX_THERAD_COUNT> m_stWaitList; ///< 防止二次重入用对象
    int                         m_iWaitListSize;        ///< 保存WaitList所需要的内存

	CTLIB_SYNC_RUN_FUNC*		m_pfRunFunc;
	void*						m_pArg;

	bool						m_bIsExit;
	bool						m_bThreadCreateEnd;

    /**
    * 线程创建函数.
    * @return 0为正常
    */
	int CreateThread()
	{
		int iRet = 0;
		pthread_t iTid;
		int iIndex;

		for (int i = 0; i < m_iThreadCount; i++)
		{
            //设置线程栈大小
            pthread_attr_t stThreadAttr;
            pthread_attr_init(&stThreadAttr);
            pthread_attr_setstacksize(&stThreadAttr, 2 * 1024 * 1024); //设置成2M

			iRet = pthread_create(&iTid, &stThreadAttr, m_pfRunFunc, m_pArg);
            pthread_attr_destroy(&stThreadAttr);
			CHECK_RETURN(iRet);

			iIndex = m_stThreadHash.HashAlloc(iTid);
			if (iIndex < 0)
			{
				return -2;
			}
			m_stThreadHash[iIndex] = i;
            iRet = pthread_cond_init(&m_stThreadCondMgr[i], NULL);
            CHECK_RETURN(iRet);
			//CT_TRACE(("create thread, id = %ul, index = %d, mgr index = %d", iTid, iIndex, i));
		}
		m_bThreadCreateEnd = true;
		return 0;
	}

    /**
    * 等待线程退出.
    * 用于主线程
    * @return 0为正常
    */
	int WaitThreadExit()
	{
		for (int i = 0; i < m_iThreadCount; i++)
		{
			//CT_TRACE(("wait for %ul exit", m_stThreadHash.m_stMgr[i].stKey));
			pthread_join(m_stThreadHash.m_stMgr[i].stKey, NULL);
			//CT_TRACE(("thread %ul exit", m_stThreadHash.m_stMgr[i].stKey));
		}
		return 0;
	}

    /**
    * 线程退出函数.
    * @return 0为正常
    */
	int GotoExit()
	{
		int iIndex = m_stThreadHash.HashFind(GetMyThreadID());
		CT_ASSERT(iIndex>=0);

		pthread_mutex_lock(&m_stCondMutex);
		m_stThreadCondMgr.InfoListMove(m_stThreadHash[iIndex], 0, CTLIB_SYNCSYS_THREAD_FREE);
		//CT_TRACE(("thread goto exit, index = %d", m_stThreadHash[iIndex]));
		pthread_mutex_unlock(&m_stCondMutex);

		return 0;
	}
};

class CTThreadStackBufInf
{
public:
    CTThreadStackBufInf()
    {

    }

    virtual ~CTThreadStackBufInf(){}

    template<typename T>
    T* New()
    {
        char* sBufPtr = GetBuff(sizeof(T));
        if (!sBufPtr)
        {
            CT_WARNING(("new error"));
            return NULL;
        }
        else
        {
            return new (sBufPtr) T;
        }
    }

    template<typename T>
    int Delete(T* p)
    {   
        p->~T();
        return FreeBuff((char*)p);
    }

    virtual char* GetBuff(int iSize) = 0;
    virtual int FreeBuff(char* p) = 0;
};

class CTThreadStackBuf : public CTThreadStackBufInf
{
public:
    CTThreadStackBuf(){}
    virtual ~CTThreadStackBuf(){}

    static size_t CountSize(const char* pszMgrConfig)
    {
        return CTMultiObjectMgr::CountSize(pszMgrConfig);
    }

    int Init(const char* pszMgrConfig, const char* pszBuffer, int iBufferSize)
    {
        return m_stObjMgr.Init(pszMgrConfig, pszBuffer, iBufferSize);
    }

    virtual char* GetBuff(int iSize)
    {
        int iIndex = m_stObjMgr.CalcMgrIndex(iSize);
        if (iIndex < 0)
        {
            return NULL;
        }

        return m_stObjMgr.Alloc(iIndex, NULL);
    }

    virtual int FreeBuff(char* p)
    {
        return m_stObjMgr.Free(p);
    }

private:
    CTMultiObjectMgr m_stObjMgr;
};

class CTSyncSysWithStackBuf : public CTSyncSys
{
public:
    virtual size_t CountSize(int iThreadCount, int iMaxCount) //为了确保使用的共享内存大小不变，此处根据可能出现的线程数量最大值计算hash size
    {
        size_t iSyncSysSize = CTSyncSys::CountSize(iThreadCount, iMaxCount); 
        if (iSyncSysSize <= 0)
        {
            return iSyncSysSize;
        }

        m_iBufPtrSize = m_stBufPtrHash.CountSize(iMaxCount);
        return iSyncSysSize + m_iBufPtrSize;
    }

    virtual int Init(CTLIB_SYNC_RUN_FUNC* pfRunFunc, void* pArg, char* sBuff)
    {
        int iRet = m_stBufPtrHash.Init(sBuff, m_iThreadCount);  
        CHECK_RETURN(iRet);
        return CTSyncSys::Init(pfRunFunc, pArg, sBuff+m_iBufPtrSize);
    }

    int InitBuf(CTThreadStackBufInf* pstStackBuf)
    {
        int iIndex = m_stBufPtrHash.HashFind(GetMyThreadID());
        if (iIndex < 0)
        {
            iIndex = m_stBufPtrHash.HashAlloc(GetMyThreadID());
            if (iIndex < 0)
            {
                return -1;
            }
        }

        m_stBufPtrHash[iIndex] = pstStackBuf;
        return 0;
    }

    template<typename T>
    T* New()
    {
        int iIndex = m_stBufPtrHash.HashFind(GetMyThreadID());
        if (iIndex < 0)
        {
            CT_WARNING((""));
            return NULL;
        }
        
        return m_stBufPtrHash[iIndex]->New<T>();
    }

    template<typename T>
    int Delete(T* p)
    {   
        int iIndex = m_stBufPtrHash.HashFind(GetMyThreadID());
        if (iIndex < 0)
        {
            CT_WARNING((""));
            return -1;
        }

        return m_stBufPtrHash[iIndex]->Delete<T>(p);
    }

private:
    int m_iBufPtrSize;
    CTHashObjectMgr<pthread_t, CTThreadStackBufInf*> m_stBufPtrHash;
};

} // namespace CTLib end 

#endif

