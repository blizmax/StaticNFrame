/**
* 异步命令工厂
*/

#ifndef __CTLIB_ASYNCCMDFACTORY_H__
#define __CTLIB_ASYNCCMDFACTORY_H__

#include "CTMultiObjectMgr.h"
#include "base/CTSingleton.h"
#include "CTArray.h"

#include <map>
#include <time.h>

namespace CTLib
{

/**
* 异步命令基类的接口.
* 为了避免和Factory相互引用导致编译不过, 
* 这里把AsyncCmd拆分成两部分
*/
class CTAsyncCmdInf
{
    friend class CTAsyncCmdFactory;

public:
    CTAsyncCmdInf() :
        m_iCmdID(0),
        m_iCmdSeq(0),
        m_shRef(0),
        m_iQueueRet(0)
    {
        m_bIsValid = true;
    }

    virtual ~CTAsyncCmdInf()
    {
        m_bIsValid = false;
    }

    bool IsValid() const
    {
        return m_bIsValid;
    }

    int GetCmdID() const
    {
        return m_iCmdID;
    }

    int GetCmdSeq() const
    {
        return m_iCmdSeq;
    }

    const CTMgrIndex& GetMgrIndex() const
    {
        return m_stIndex;
    }

public:
    /**
    * 生成命令后执行Do函数.
    * @param[in] pData 用void*做参数, 以不变应万变
    */
    virtual int Do(void* pData)
    {
        return 0;
    }

    /**
    * 如果有子命令, 子命令执行完毕后调用父命令的CallBack函数
    * @param[in] iRet 子命令执行的返回值
    */
    virtual int CallBack(int iRet)
    {
        return 0;
    }

    /**
    * 如果没有子命令, 外部收到响应后调用命令的OnAnswer函数
    * @param[in] pData 用void*做参数, 以不变应万变
    */
    virtual int OnAnswer(void* pData)
    {
        return 0;
    }

    //命令完成
    virtual int Done(int iRet)
    {
        return 0;
    }

    /**
    * 该命令用于自定义子命令返回处理函数
    * @param[in] iRet 子命令执行的返回值
    */
    virtual int LogicDo(int iRet)
    {
        return 0;
    }

    //设置命令开始时间
    virtual void SetCmdCreateTime(int iNow = -1)
    {
    }

    virtual int GetCmdCreateTime()
    {
        return 0;
    }

    virtual bool HasChild()
    {
        return false;
    }

    virtual bool IsInQueue() const
    {
        return false;
    }

    virtual int AddExecTime(double dExecTime)
    {
        return 0;
    }

    virtual int SetBeginExecTime(timeval stBeginExecTime)
    {

        return 0;
    }

    virtual int AddRunMicroSec(int iCount)
    {
        return 0;
    }

    virtual int GetRunMicroSec()
    {
        return 0;
    }

    /** 返回引用计数 */
    short GetRef() const
    {
        return m_shRef;
    }
protected:

    /** 引用计数加1 */
    short IncRef()
    {
        CT_TRACE(("After IncRef, AsyncCmd(%d, %p)'s ref=%d", GetCmdID(), this, m_shRef + 1));
        return ++m_shRef; 
    }

    /** 引用计数减1 */
    short DecRef()
    {
        CT_TRACE(("After DecRef, AsyncCmd(%d, %p)'s ref=%d", GetCmdID(), this, m_shRef - 1));
        return --m_shRef;
    }

    //从队列中摘出
    virtual void PopQueue()
    {
        ;
    }

private:
    int                 m_iCmdID;
    int		            m_iCmdSeq;
    short				m_shRef;
    CTMgrIndex          m_stIndex;
    bool                m_bIsValid;

protected:
    int                 m_iQueueRet; ///< 只给PushQueue使用
};

/**
* 异步命令的工厂注册基类
*/
class CTAsyncCmdRegisterInf
{
public:
    CTAsyncCmdRegisterInf() : m_iMgrIndex(-1) {}
    virtual ~CTAsyncCmdRegisterInf() {}

    virtual int GetObjectSize() const = 0;
    virtual CTAsyncCmdInf* CreateObject(char* pszBuffer) const = 0;

public:
    int m_iMgrIndex;
};

/**
* 异步命令工厂
*/
class CTAsyncCmdFactory
{
protected:
    typedef map<int, CTAsyncCmdRegisterInf*> TRegisterMap;

    enum
    {
        LIST_COUNT  = 2,
        INDEX_COUNT = 3,

		ALLOC_INDEX = 0,
		FREE_LIST = 0,
		USED_LIST = 1,

        QUEUE_INDEX = 1,
        NO_QUEUE_LIST = 0,
        QUEUE_LIST = 1,
    
        MAX_DOQUEUE = 1024, ///< 默认一次最多执行的队列命令
        MAX_CMD_TIMEOUT = 10, ///< 命令超时时间（秒）
    };

    int                 m_iCmdSeq;
    TRegisterMap        m_stMap;
    CTMultiObjectMgr    m_stMgr;

public:
    CTAsyncCmdFactory() : m_iCmdSeq(0)
    {
    }

    virtual ~CTAsyncCmdFactory()
    {
    }

    int Init(const char* pszMgrConfig, const char* pszBuffer, int iBufferSize)
    {
        int iRet = 0;
        iRet = m_stMgr.Init(pszMgrConfig, pszBuffer, iBufferSize, LIST_COUNT, INDEX_COUNT);
        if (iRet)
        {
            return iRet;
        }

        TRegisterMap::iterator it;
        int iMgrIndex = 0;
        for (it = m_stMap.begin(); it != m_stMap.end(); ++it)
        {
            iMgrIndex = m_stMgr.CalcMgrIndex(it->second->GetObjectSize());
            if (iMgrIndex < 0)
            {
                CT_ERROR(("RegisterCmd fail, cmdid(%d)'s size(%d) is too large", it->first,
                    it->second->GetObjectSize()));
                return -1;
            }

            it->second->m_iMgrIndex = iMgrIndex;
        }

        return 0;
    }

    static size_t CountSize(const char* pszMgrConfig)
    {
        return CTMultiObjectMgr::CountSize(pszMgrConfig, LIST_COUNT, INDEX_COUNT);
    }

    //=================================
    // 异步命令的注册/创建/释放

    /**
    * 注册异步命令
    */
    void RegisterCmd(int iCmdID, CTAsyncCmdRegisterInf* p)
    {
        TRegisterMap::iterator it = m_stMap.find(iCmdID);
        if (it != m_stMap.end())
        {
            CT_THROW(("cmdid(%d) already exists", iCmdID));
        }
        
        CT_INFO(("CmdID = %d, cmd size = %d",
            iCmdID, p->GetObjectSize()));

        m_stMap[iCmdID] = p;
    }

    /**
    * 根据iCmdID创建异步命令
    */
    CTAsyncCmdInf* CreateCmd(int iCmdID)
    {
        TRegisterMap::iterator it = m_stMap.find(iCmdID);
        if (it != m_stMap.end())
        {
            CTAsyncCmdRegisterInf* p = it->second;
            CT_ASSERT((p && p->m_iMgrIndex >= 0 && p->m_iMgrIndex < (int)m_stMgr.GetMgrCount()));

            CTMgrIndex stIndex;
            char* pszBuffer = m_stMgr.Alloc(p->m_iMgrIndex, &stIndex);
            if (!pszBuffer)
            {
                CT_WARNING(("alloc buffer fail, mgrindex=%d", p->m_iMgrIndex));
                return NULL;
            }

            CTAsyncCmdInf* pstCmd = p->CreateObject(pszBuffer);
            pstCmd->m_iCmdID = iCmdID;
            pstCmd->m_iCmdSeq = GetCmdSeq();
            pstCmd->m_stIndex = stIndex;
            pstCmd->SetCmdCreateTime(time(NULL));   //执行time(NULL)可能有性能问题，以后优化
			CT_TRACE(("Create AsyncCmd %d(%p)", iCmdID, pstCmd));
            return pstCmd;
        }

        return NULL;
    }

    /**
    * 释放异步命令
    */
    int FreeCmd(CTAsyncCmdInf* pstCmd)
    {
        CTMgrIndex stIndex;
        if (!pstCmd || !m_stMgr.IsValidObject((const char*) pstCmd, &stIndex))
        {
            return -1;
        }

        pstCmd->~CTAsyncCmdInf();
        m_stMgr.Free(stIndex);
        return 0;
    }

    bool IsValidCmd(CTAsyncCmdInf* pstCmd, CTMgrIndex* pstIndex = NULL)
    {
        return m_stMgr.IsValidObject((const char*)pstCmd, pstIndex);
    }
    
    //-1表示按照全局测试判断，其他ID为单种命令块使用量判断
    bool IsBusy(int iMaxPercent)
    {
        if (iMaxPercent <= 0)
        {
            return true;
        }

        for(unsigned int i = 0; i < m_stMgr.GetMgrCount(); i++)
        {
	    int iTotalCount = m_stMgr.GetMgrObjectCount(i);
            if(iTotalCount <= 0)
            {
                return true;
            }
            int64 i64Temp = m_stMgr.Size(i, ALLOC_INDEX, USED_LIST) * 100;
            if (i64Temp / iTotalCount >= iMaxPercent)
            {
                return true;
            }
        }

        return false;
    }

	void DumpStatInfo(CTLog* pstLog)
	{
		for(unsigned int i = 0; i<m_stMgr.GetMgrCount(); i++)
		{
			pstLog->Log(EInfo, "Cmds_Lv%d: All=%d Used=%d Free=%d Queue=%d",i+1,
				m_stMgr.GetMgrObjectCount(i),
				m_stMgr.Size(i, ALLOC_INDEX, USED_LIST),
				m_stMgr.Size(i, ALLOC_INDEX, FREE_LIST),
				m_stMgr.Size(i, QUEUE_INDEX, QUEUE_LIST));
		}
	}

	void DumpStatDetail(CTLog* pstLog)
	{
		pstLog->Log(EInfo, "===================== Async Cmds Detail =====================");
		CTMgrIndex stFirst, stIndex, stNext;
		m_stMgr.GetHead(stFirst, USED_LIST, ALLOC_INDEX);

		if(stFirst.IsNull())
		{
			pstLog->Log(EInfo, "No Used Cmd");
			return ;
		}

		stIndex = stFirst;
		for (; !stIndex.IsNull(); stIndex = stNext)
		{
			m_stMgr.GetNext(stIndex, stNext, ALLOC_INDEX);
			pstLog->Log(EInfo, "Cmd MsgId %d", ((CTAsyncCmdInf*)(m_stMgr.Get(stIndex)))->GetCmdID());
		}
	}

    //=================================
    // 命令队列的操作

    /**
    * 把命令放到队列中
    */
    int PushQueue(CTAsyncCmdInf* pstCmd)
    {
        CTMgrIndex stIndex;
        if (!m_stMgr.IsValidObject((const char*)pstCmd, &stIndex))
        {
            return -1;
        }

        return m_stMgr.Move(stIndex, QUEUE_INDEX, QUEUE_LIST);
    }

    /**
    * 把队列中的命令拉出来执行
    * @param[in] iMaxDo 最多执行多少个队列命令
    * @return 返回执行了多少个队列命令
    */
    int DoQueue(int iMaxDo = MAX_DOQUEUE)
    {
        int iCount = 0;

        CTMgrIndex stFirst, stIndex, stNext;
        m_stMgr.GetHead(stFirst, QUEUE_LIST, QUEUE_INDEX);
        
        // 如果队列中的某个命令执行后还会继续放到队列中, 需要
        // 用 !(iCount > 0 && stIndex != stFirst) 来避免这种情况造成的无效循环
        for (iCount = 0, stIndex = stFirst;
             iCount < iMaxDo && !stIndex.IsNull() && !(iCount > 0 && stIndex != stFirst);
             ++iCount, stIndex = stNext)
        {
            m_stMgr.GetNext(stIndex, stNext, QUEUE_INDEX);

            CTAsyncCmdInf* pstCmd = (CTAsyncCmdInf*) m_stMgr.Get(stIndex);

            CT_TRACE(("exec queue AsyncCmd(%d, %p)(%d)", pstCmd->GetCmdID(),
                pstCmd, pstCmd->m_iQueueRet));

            //从队列摘除
            m_stMgr.Move(stIndex, QUEUE_INDEX, NO_QUEUE_LIST);

            pstCmd->DecRef();
            pstCmd->PopQueue();
            pstCmd->CallBack(pstCmd->m_iQueueRet);
            
            /*
            if (pstCmd->GetRef() <= 0)
            {
                FreeCmd(pstCmd);
            }
            */
        }

        return iCount;
    }

    //检查超时的命令
    int CheckTimeoutCmd(int iNow)
    {
        iNow = time(NULL);  //传进来的时间可能带时区
        int iCount = 0;
        const int FREE_CMD_PER_CHECK = 100;
        CTArray<CTAsyncCmdInf*, FREE_CMD_PER_CHECK> arpCmdNeedFree;
        
        CTMgrIndex stFirst, stIndex, stNext;
        m_stMgr.GetHead(stFirst, USED_LIST, ALLOC_INDEX);

        // 如果队列中的某个命令执行后还会继续放到队列中, 需要
        for (iCount = 0, stIndex = stFirst;!stIndex.IsNull() && iCount < FREE_CMD_PER_CHECK; stIndex = stNext)
        {
            m_stMgr.GetNext(stIndex, stNext, ALLOC_INDEX);

            CTAsyncCmdInf* pstCmd = (CTAsyncCmdInf*) m_stMgr.Get(stIndex);
            if (!pstCmd->IsInQueue() && !pstCmd->HasChild() && iNow - pstCmd->GetCmdCreateTime() >= MAX_CMD_TIMEOUT)
            {
                arpCmdNeedFree[iCount] = pstCmd;            //由于命令有关联性，所以将待释放的命令先存起来，之后统一释放
                arpCmdNeedFree.m_iUsedCount = iCount+1;
                iCount++;
            }
        }

        //释放命令
        for (int i = 0; i < arpCmdNeedFree.m_iUsedCount; i++)
        {
            CT_WARNING(("find cmd(%d, %p) timeout, do Done()", arpCmdNeedFree[i]->GetCmdID(), arpCmdNeedFree[i]));
            arpCmdNeedFree[i]->Done(10); //10定义在commerror.h中，RET_SYS_CMD_TIMEOUT
        }

        arpCmdNeedFree.m_iUsedCount = 0;

        return iCount;
    }

protected:
    int GetCmdSeq()
    {
        if (m_iCmdSeq >= CT_INT_MAX || m_iCmdSeq < 0)
        {
            m_iCmdSeq = 0;
        }

        return ++m_iCmdSeq;
    }
};

#define CT_CMDFACTORY  (CTSingleton<CTAsyncCmdFactory>::Instance())

/**
* 异步命令的工厂注册类
*/
template<typename T>
class CTAsyncCmdRegister: public CTAsyncCmdRegisterInf
{
public:
    CTAsyncCmdRegister(int iCmdID)
    {
        CT_TRACE(("register cmd id(%d)!", iCmdID));
        CT_CMDFACTORY->RegisterCmd(iCmdID, this);
    }

    int GetObjectSize() const
    {
        return sizeof(T);
    }

    CTAsyncCmdInf* CreateObject(char* pszBuffer) const
    {
        return new (pszBuffer) T;
    }
};

// 简化注册的宏
#define CT_REGISTER_ASYNCCMD(CmdID, Class) \
static const CTAsyncCmdRegister<Class> CTAsyncCmdRegister##Class(CmdID)

} // namespace CTLib

#endif //__CTLIB_ASYNCCMDFACTORY_H__
