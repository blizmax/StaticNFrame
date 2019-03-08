/**
* 共享内存队列
* @author thinkry
*/

#ifndef __CTLIB_SHMQUEUE_H__
#define __CTLIB_SHMQUEUE_H__

#include "CTBase.h"
#include "CTSizeString.h"
#include "net/CTSocket.h"
#include "ipc/CTShm.h"
#include "CTCodeQueue.h"
#include "net/CTEpoll.h"

namespace CTLib
{

/**
* 共享内存队列
*/
template<typename EpollOwner>
class CTShmQueue
{
protected:
    typedef CTEpollObject<EpollOwner> COwnerEpollObject;

    CTShm               m_stShm;               ///< 共享内存
    CTCodeQueue*		m_pstRecvQueue;        ///< 我的接收队列
    CTCodeQueue*		m_pstSendQueue;        ///< 我的发送队列
    CTSocket            m_stNotifySocket;      ///< 向对方发送通知的SOCKET
    CTSocket            m_stListenSocket;      ///< 我接收通知的SOCKET
    COwnerEpollObject   m_stEpollObject;       ///< 用于注册到Epoll
    bool                m_bBackEnd;            ///< 我是共享内存队列的前端 还是后端
    char                m_szBuffer[256];

public:
    CTShmQueue() :  m_pstRecvQueue(NULL),
                    m_pstSendQueue(NULL),
                    m_stNotifySocket(PF_LOCAL, SOCK_DGRAM, false),
                    m_stListenSocket(PF_LOCAL, SOCK_DGRAM, false),
                    m_bBackEnd(false)
    {
    }

    virtual ~CTShmQueue()
    {
    }

    /**
    * 初始化
    * @param[in] bBackEnd 是共享内存队列的前端 还是后端
    * @param[in] pszShmKey 共享内存的Key
    * @param[in] iShmSize 共享内存的大小(这里是共享内存的总大小，单向队列的大小是1/2)
    * @param[in] pszFrontEndSocket 前端的SOCKET
    * @param[in] pszBackEndSocket 后端的SOCKET
    * @return 0=成功
    */
    int Init(bool bBackEnd, const char* pszShmKey, int iShmSize, 
             const char* pszFrontEndSocket, const char* pszBackEndSocket,
             CTEpoll& stEpoll, EpollOwner& stOwner,
             typename COwnerEpollObject::PF_EPOLL_EVENT pfEvent,
             unsigned int iAct = 0)
    {
        CT_ASSERT(!(!pszShmKey || iShmSize <= 0 || !pszFrontEndSocket || !pszBackEndSocket));

        m_bBackEnd = bBackEnd;

        int iRet = 0;

        // 初始化前端共享内存
        iRet = m_stShm.Create(pszShmKey, iShmSize);
        if (iRet)
        {
            CT_ERROR(("m_stShm.Create failed %d. key=%s size=%d", iRet, pszShmKey, iShmSize));
            return iRet;
        }

        CT_INFO(("create shmqueue's shm at %p ok. key=%s size=%d",
                m_stShm.GetBuffer(), pszShmKey, iShmSize));

        // 初始化消息队列
        const int iQueueSize = m_stShm.GetSize() / 2;
        m_pstRecvQueue = (CTCodeQueue*) 
            (m_stShm.GetBuffer() + (m_bBackEnd ? 0 : iQueueSize));
        if (m_pstRecvQueue->CheckSum())
        {
            CT_INFO(("m_pstRecvQueue need create"));
            iRet = m_pstRecvQueue->Init(iQueueSize);
            if (iRet)
            {
                CT_ERROR(("m_pstRecvQueue init failed %d", iRet));
                return iRet;
            }
        }

        m_pstSendQueue = (CTCodeQueue*) 
            (m_stShm.GetBuffer() + (m_bBackEnd ? iQueueSize : 0));
        if (m_pstSendQueue->CheckSum())
        {
            CT_INFO(("m_pstSendQueue need create"));
            iRet = m_pstSendQueue->Init(iQueueSize);
            if (iRet)
            {
                CT_ERROR(("m_pstSendQueue init failed %d", iRet));
                return iRet;
            }
        }

        // 初始化和前端通讯的Socket
        const char* pszNotifySocket = m_bBackEnd ? pszFrontEndSocket : pszBackEndSocket;
        const char* pszListenSocket = m_bBackEnd ? pszBackEndSocket : pszFrontEndSocket;

        iRet = m_stNotifySocket.Connect(pszNotifySocket);
        if (iRet)
        {
            CT_ERROR(("m_stNotifySocket.Connect failed %d, sock=%s", iRet, pszNotifySocket));
            return iRet;
        }

        iRet = m_stListenSocket.Listen(pszListenSocket);
        if (iRet)
        {
            CT_ERROR(("m_stListenSocket.Listen failed %d, sock=%s", iRet, pszListenSocket));
            return iRet;
        }

        // 注册到Epoll
        iRet = m_stEpollObject.Register(stOwner, pfEvent, stEpoll, m_stListenSocket.GetSocket(), EPOLLIN, iAct);
        return iRet;
    }

    /**
    * 向发送队列中放入一个Code
    */
    int PutOneCode(const char* p, int len)
    {
        int iRet = 0;
        iRet = m_pstSendQueue->Put(p, len);
        
        // 发送Notify消息
        int iSendLen = 0;
        m_stNotifySocket.SendEx("1", 1, iSendLen);
        return iRet;
    }

    /**
    * 把两块缓冲区合并成一个Code放入队列中.
    * 这个函数的作用是为了减少1次内存拷贝
    */
    int PutOneCode(const char* p1, int len1, const char* p2, int len2)
    {
        int iRet = 0;
        iRet = m_pstSendQueue->Put(p1, len1, p2, len2);

        // 发送Notify消息
        int iSendLen = 0;
        m_stNotifySocket.SendEx("1", 1, iSendLen);
        return iRet;
    }

    /**
    * 从接收队列中获取一个Code
    */
    int GetOneCode(char* p, int buflen, int& len)
    {
        return m_pstRecvQueue->Get(p, buflen, len);
    }

    /**
    * 删除接收队列中的下一个Code
    */
    int Remove()
    {
        return m_pstRecvQueue->Remove();
    }

    /**
    * 删除recv中的内容, 只有接收方可以调用这个函数!!!
    */
    int RemoveAllRecv()
    {
        return m_pstRecvQueue->RemoveAll();
    }

    /**
    * 判断接收队列中有没有Code
    */
    bool HasCode() const
    {
        return m_pstRecvQueue->GetCodeLen() > 0;
    }

    /**
    * 清除socket事件, 避免重复被调用
    */
    void ClearNotify()
    {
        int iRet = 0;
        int iRecvLen = 0;
        while (true)
        {
            iRet = m_stListenSocket.Recv(m_szBuffer, sizeof(m_szBuffer), iRecvLen);
            if (iRet)
            {
                if (iRet != CT_EWOULDBLOCK && iRet != CT_EINPROGRESS)
                {
                    CT_WARNING(("m_stListenSocket.Recv failed %d, errno=%d", iRet, CT_WSA_ERRNO));
                }

                return;
            }

            // 直到把所有数据收完才出来
            if (iRecvLen == 0)
            {
                break;
            }
        }
    }
};

} //namespace CTLib

#endif //__CTLIB_SHMQUEUE_H__
