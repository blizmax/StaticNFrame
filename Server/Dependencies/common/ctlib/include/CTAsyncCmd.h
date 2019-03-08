/**
* 异步命令框架
*/

#ifndef __CTLIB_ASYNCCMD_H__
#define __CTLIB_ASYNCCMD_H__

#include "CTBase.h"
#include "CTMsgBase.h"
#include "CTAsyncCmdFactory.h"

namespace CTLib
{


/**
* 异步命令基类
*/
class CTAsyncCmd: public CTAsyncCmdInf
{
public:
    typedef int(CTAsyncCmd::*ASYNC_CALL_BACK_FUNC)(int);

    CTAsyncCmd() :
        m_pstParent(NULL),
        m_pstChild(NULL),
        m_iState(0),
        m_iCmdCreateTime(0),
        m_bParentEnableLogicDo(false),
        m_bFreeSelf(true),
        m_iReverse1(0),
        m_iReverse2(0)
       
    {
        m_iHasRunMicroSec = 0;
    }

    virtual ~CTAsyncCmd()
    {
        CT_TRACE(("AsyncCmd(%p) free. parent=%p, child=%p, seq=%d", this, m_pstParent, m_pstChild, GetCmdSeq()));

        // 如果自己比子命令先释放(超时会出现这种情况), 那么把子命令的pParent设为0
        if (m_pstChild != NULL && m_pstChild->m_pstParent == this)
        {
            m_pstChild->m_pstParent = NULL;
        }

        if (m_pstParent != NULL && m_pstParent->m_pstChild == this)
        {
            m_pstParent->m_pstChild = NULL;
        }
        m_bParentEnableLogicDo = false;
    }

    /**
    * 在调用子命令Do之前先RegChild
    */
    void RegisterChild(CTAsyncCmd* pChildCmd, bool bParentEnableLogicDo = false)
    {
        CT_ASSERT(pChildCmd);
        CT_TRACE(("AsyncCmd(%d, %p) register child(%d, %p)",
            GetCmdID(), this, pChildCmd->GetCmdID(), pChildCmd));

        m_iState = pChildCmd->GetCmdID();
        pChildCmd->m_pstParent = this;
        this->m_pstChild = pChildCmd;
        pChildCmd->m_bParentEnableLogicDo = bParentEnableLogicDo;
        IncRef();
    }

    /**
    * 命令执行完毕后调用Done函数, 以便自动释放或回调父命令
    */
    virtual int Done(int iRet)
    {
        CT_TRACE(("AsyncCmd(%d, %p) done, ret=%d", GetCmdID(), this, iRet));

        // 回调父命令
        if (m_pstParent)
        {
            CT_TRACE(("AsyncCmd(%d, %p) done, it's parent is (%d, %p)",
                GetCmdID(), this, m_pstParent->GetCmdID(), m_pstParent));
            m_pstParent->DecRef();

            if (m_bParentEnableLogicDo)
            {
                m_pstParent->LogicDo(iRet);
            }
            else
            {
                m_pstParent->CallBack(iRet);
            }
        }

        // 释放自己
        if (m_bFreeSelf)
        {
            m_pstParent = NULL;
            m_bParentEnableLogicDo = false;

            int i  = CT_CMDFACTORY->FreeCmd(this);
            if (i)
            {
                CT_WARNING(("freecmd(%d, %p) failed, ret=%d", GetCmdID(), this, i));
            }
        }
        else
        {
            m_bFreeSelf = true; //下次释放
        }
        
        return iRet;
    }

    virtual bool HasChild()
    {
        return (m_pstChild != NULL && GetRef() > 0);
    }

    virtual bool IsInQueue() const
    {
        return m_bIsInQueue;
    }

    //从队列中摘出
    virtual void PopQueue()
    {
        m_bIsInQueue = false;
    }

    /**
    * 把自己放到CTAsyncCmdQueue中
    */
    int PushQueue(int iRet)
    {
        m_iQueueRet = iRet;
        m_iState = GetCmdID();
        m_bIsInQueue = true;
        IncRef();
        return CT_CMDFACTORY->PushQueue(this);
    }

    virtual void SetCmdCreateTime(int iNow = -1)
    {
        m_iCmdCreateTime = iNow;
    }

    virtual int GetCmdCreateTime()
    {
        return m_iCmdCreateTime;
    }

    virtual CTAsyncCmd* GetChild()
    {
        return m_pstChild;
    }

    virtual int AddRunMicroSec(int iCount)
    {
        m_iHasRunMicroSec += iCount;
        return m_iHasRunMicroSec;
    }

    virtual int GetRunMicroSec()
    {
        return m_iHasRunMicroSec;
    }

public:
    /**
    * 默认回调函数是执行Done
    */
    virtual int CallBack(int iRet)
    {
        return Done(iRet);
    }

    /**
    * 默认调用callback
    */
    virtual int LogicDo(int iRet)
    {
        return CallBack(iRet);
    }

protected:
    CTAsyncCmd*			m_pstParent;
    CTAsyncCmd*			m_pstChild;
    int 				m_iState;
    int                 m_iCmdCreateTime;          // 命令生成时间。
    bool                m_bParentEnableLogicDo;

    int                 m_iHasRunMicroSec;          //已经执行的微秒数
    bool                m_bIsInQueue;           //是否在队列中
public:
    bool                m_bFreeSelf;                //是否释放自己
    int					m_iReverse1;			// 保留字段
    int					m_iReverse2;			// 保留字段

};

} // namespace CTLib

#endif //__CTLIB_ASYNCCMD_H__
