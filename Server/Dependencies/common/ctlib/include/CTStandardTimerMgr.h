#ifndef __CTSTANDARTIMERMGR_H__
#define __CTSTANDARTIMERMGR_H__
#include "CTObjectMgr.h"

/**
*
*/
namespace CTLib
{
    /**
    * 求2个数的最小公倍数
    */
    inline int MinComMultiple(int uiA, int uiB)
    {
        int uiMax = CT_MAX(uiA, uiB);
        int uiMin = CT_MIN(uiA, uiB);

        if (uiMax <= 0 || uiMin <= 0)
        {
            //自然数集合中不应该出现零
            CT_WARNING((""));
            return 0;
        }

        int uiComDivisor = uiMax % uiMin;
        
        while(uiComDivisor)
        {
           uiMax = uiMin;
           uiMin = uiComDivisor;
           uiComDivisor = uiMax % uiMin;
        }
        //现在uiMin 中保存的是最大公约数
        return uiMax * (CT_MIN(uiA, uiB) / uiMin);
      
    }
/**
需要增加其通用性
*/
enum enumItemCountType
{
    EICY_ALL  = 0,
    EICY_FREE = 1,
    EICY_USED = 2,
};

template<typename T>
class CTTimerObjectMgr
{
public:
    typedef struct TStanderdTimerTag
    {
        time_t iDeadLine;	///< 定时器到期时间
        T stData;		    ///< 注册的定时器数据，任务的流水号
    } TSTimerTag;

    typedef struct TStanderTimerRankInfo
    {
        int iTimeTop;  ///< 每一个档位对应的时间上限,尾档对应的时间上限为-1
        int iRankNonius;  ///< 档位游标，当前正在检索的位置
    } TSTimerRankInfo;

    CTObjectMgr<TSTimerTag> m_stMgr;    ///< 先设为public, 外部可能会用到索引链表操作
    TSTimerTag t;
    int iTimerSeq;
//时间档编号
    enum
    {
        ALLOC_LIST = 0,    ///< 未分配队列
        ONTIME_LIST = 1,   ///< 到期任务链
        LIST_ONTIME = 0,   ///< 到期队列档位 
        LIST_RANKREAL = 1,  ///< 准备好队列，该队列每次轮训均完全检索    
        LIST_RANKEND = 8,   ///< 最高档，用于系统限制分档的最多档数，实际最多档数为该值+1
        LIST_RANKTAIL = 100,  ///< 尾档，
    };
    int m_iTimeMode; //计时模 //@@ 改为protected
    int m_itmeCounts;  //@@ 改为protected

//根据项目的Rank值换算其队列编号
//#define LISTINDEX(it) (it == LIST_RANKTAIL ? m_iListRankFlag + 1 : it)
#define LISTINDEX(it) (it == LIST_RANKTAIL ? m_iListRankFlag + 2 : it + 1)
//根据项目的Rank值换算其信息数字下标
#define RANKFINFO(it) (it == LIST_RANKTAIL ? 0 : it)

#define PEVRANK(it) (it == LIST_RANKTAIL ? m_iListRankFlag : it - 1)


    TSTimerRankInfo m_stListRankInfo[LIST_RANKEND + 1]; ///< 队列等级信息列表,m_stListRankInfo[0]为尾档
protected:
//    int m_iTimeConut; ///< 时间计数

    int m_iListRankFlag;    ///< 当前启用的档数

    /**
    * 插入一个时间档位
    * @param[in] iTimeTop 该档时间上限
    * @return[out] 0 成功
    * @return[out] -1 该档处于被覆盖
    * @return[out] -2 队列已满
    * @return[out] -3 参数不正确
    */
    int InsertListRank(int iTimeTop)
    {
        if (iTimeTop <= 0)
        {
            return -3;
        }

        if (m_iListRankFlag == 0)
        {//首次插入
            //初始化尾档
            m_stListRankInfo[m_iListRankFlag].iTimeTop = -1;
            m_stListRankInfo[m_iListRankFlag].iRankNonius = -1;

            ++m_iListRankFlag;
            m_stListRankInfo[m_iListRankFlag].iTimeTop = iTimeTop;
            m_stListRankInfo[m_iListRankFlag].iRankNonius = -1;
            m_iTimeMode = iTimeTop;
        }
        else if (m_iListRankFlag < LIST_RANKEND) //系统级限制
        {
            if (m_stListRankInfo[m_iListRankFlag].iTimeTop >= iTimeTop)
            {
                //档位处于被覆盖段
                return -1;
            }

            m_iTimeMode = MinComMultiple(m_stListRankInfo[m_iListRankFlag].iTimeTop, iTimeTop);
            ++m_iListRankFlag;
            m_stListRankInfo[m_iListRankFlag].iTimeTop = iTimeTop;
            m_stListRankInfo[m_iListRankFlag].iRankNonius = -1;
           
        }
        else //队列已满
        {
            return -2;
        }

        return 0;
    }

public:
    CTTimerObjectMgr()
    {
        m_iListRankFlag = 0;
        m_iTimeMode = 0;
        m_itmeCounts = 0;
    }

    virtual ~CTTimerObjectMgr()
    {
    }

    /**
    * 计算需要空间
    * @param[in] iObjectCount 对象的总数
    * @param[in] iListCount  分档时间点总数
    */
    static size_t CountSize(int iObjectCount, int iListCount)
    {
        return CTObjectMgr<TSTimerTag>::CountSize(iObjectCount, sizeof(TSTimerTag), iListCount + 3, 1);
    }

    /**
    * 初始化数据结构对象
    * @param[in] pBuffer 预分配内存指针
    * @param[in] iObjectCount 数据对象容量
    *
    */
    int Init(const char* pBuffer, vector<string> &astKeyValue, int iTimeSeq)
    {
        //分档
        string szFlag, szValue;
        int iObjectCount = 0;
        for (size_t i = 0; i < astKeyValue.size(); ++i)
        {
            if (!CTStringUtils::SplitIni(astKeyValue[i], szFlag, szValue, ','))
            {
                return -1;
            }

            switch (CTStringUtils::StrToInt<int>(szFlag.c_str()))
            {
            case 0://总数
                {
                    iObjectCount = CTStringUtils::StrToInt<int>(szValue.c_str());
                    break;
                }           
            case 1:
                {
                    int ret = InsertListRank(CTStringUtils::StrToInt<int>(szValue.c_str()));
                    if (ret < 0)
                    {
                        CT_WARNING(("Init TimerRank Err %d",ret));
                        return -2;
                    }
                    break;
                }

            default:
                break;
            }
        }
        m_itmeCounts = iObjectCount;
        iTimerSeq = iTimeSeq;
        return m_stMgr.Init(pBuffer, iObjectCount, sizeof(T), m_iListRankFlag + 3, 1);
    }

    /**
    * 插入一个节点到队列中，分配一个未分配的节点
    * @param[in] data 为定时器数据
    * @param[in] deadline 为到期时间
    * @return  为插入的队列序号 0表示已经过期
    */
    int Insert(const T& stData, time_t iDeadLine)
    {  

        //插入链表
        //判断新项目所属的阶梯分段
        int iItemRank = LIST_RANKREAL;
        int iWaitTime = (int)(iDeadLine - time(0));

        if (iWaitTime >= 0)
        {
            for (; iItemRank <= m_iListRankFlag; ++iItemRank)
            {
                if (iWaitTime < m_stListRankInfo[iItemRank].iTimeTop)
                {
                    break;
                }
            }
            if (iItemRank > m_iListRankFlag)
            {
                iItemRank = LIST_RANKTAIL;
            }
        }
        else
        {
            iItemRank = LIST_ONTIME;
        }
        
        CT_TRACE(("[time(%d)] Insert New Note Rank=%d", iTimerSeq, iItemRank));
        int iItemFlag = m_stMgr.InfoListAlloc(0, LISTINDEX(iItemRank));
        if (iItemFlag < 0)
        {
            CT_ERROR(("[time(%d)] Insert New Note fails iRet(%d) !!", iTimerSeq, iItemFlag));
            return -1;
        }

        m_stMgr[iItemFlag].stData = stData;
        m_stMgr[iItemFlag].iDeadLine = iDeadLine;
        return iItemFlag;
    }

    /**
    * 从定时器中删除一个节点
    * 待删除的节点索引号
    * note:这里专门定义这么一个函数是为了
    * 防止调度队列的游标刚好落到待删除项目上的情况发生
    */
    int RemoveOut(int iItemIndex)
    {
        //检查各个档位有无游标位置是目标节点的
        //for (int i = 0; i < m_iListRankFlag)
        //这里我并不关心它到底在哪个队里,只关心它是否是当前的一个游标
        for (int iItemRank = 0; iItemRank <= m_iListRankFlag; ++iItemRank)
        {
            if (m_stListRankInfo[iItemRank].iRankNonius == iItemIndex)
            {
                //一个元素只可能在一个列表中如果发现那么操作完就可以跳出了
                //把游标移动到他的后继节点,不需要做-1的判断，每次轮训的时候会自动纠正
                m_stListRankInfo[iItemRank].iRankNonius = m_stMgr.List(iItemIndex).iNext;
                break;
            }
        }
        
        //把结点转移到未分配列表中去
        return m_stMgr.InfoListMove(iItemIndex, 0, ALLOC_LIST);
    }
    /**
    * 删除一个节点
    * @param[in] iItemIndex 待删除的项目索引
    * @param[in] iListIndex 待删除项目所在的链表 
    *
    */
    void Remove(int iItemIndex, int iRankIndex)
    { 
        m_stMgr.InfoListMove(iItemIndex, 0, 0, m_stMgr.ListInfo(LISTINDEX(iRankIndex)), m_stMgr.ListInfo(0));
    }

    /**
    * 转移一个节点到另一条链表中
    * @param[in] iSourListIndex 原链表索引号
    * @param[in] iNewListIndex  新链表索引号
    * @param[in] iItemIndex     待转移的项目ID
    */
    int MoveTo(int iSourListIndex, int iNewListIndex, int iItemIndex)
    {
        if (m_stMgr.Flag(iItemIndex) != iSourListIndex)
        {
            CT_ERROR(("[time(%d)]  Move Note(%d) but not in list(%d)", iTimerSeq, iItemIndex, iSourListIndex));
            return -1;
        }

        return m_stMgr.InfoListMove(iItemIndex, 0, iNewListIndex, m_stMgr.ListInfo(iSourListIndex), m_stMgr.ListInfo(iNewListIndex));
    }

    /**
    * 获取指定定时器档位的定时器数目
    * @param[in] iListIndex 定时器档位序列号
    */
    int GetListCount(int iRankIndex)
    {
        return m_stMgr.Size(LISTINDEX(iRankIndex));
    }

    int GetAllocListCount()
    {
        return m_stMgr.ListInfo(ALLOC_LIST).iSize;
    }

    int GetListHead(int iRankIndex)
    {
        return m_stMgr.ListInfo(LISTINDEX(iRankIndex)).iHead;
    }

    int GetListTail(int iRankIndex)
    {
        return m_stMgr.ListInfo(LISTINDEX(iRankIndex)).iTail; 
    }

    TSTimerTag& GetNextItem(int &iItemIndex)
    {
        int ItemFlag = iItemIndex;
        iItemIndex = m_stMgr.List(iItemIndex).iNext;
        return m_stMgr[ItemFlag];
      
    }

    int GetRealLen()
    {
        return m_stMgr.ListInfo(ONTIME_LIST).iSize;
    }
    
    int DoRealHead(T& stHeadData)
    {
        int iTmpFalg = m_stMgr.ListInfo(ONTIME_LIST).iHead;
        if (iTmpFalg < 0)
        {
            return 1;
        }

        stHeadData = m_stMgr[iTmpFalg].stData;
        return m_stMgr.InfoListMove(iTmpFalg, 0, ALLOC_LIST);
        //return 0;
    }

    TSTimerTag& GetItem(int iItemIndex)
    {
        return m_stMgr[iItemIndex];
    }

    /**
    * 获取当前分档数
    */
    int GetRankCount()
    {
        return m_iListRankFlag;
    }


#define CHECK_NONUIUS(nus) \
    if (nus < 0 || nus >= m_itmeCounts) \
    { \
        CT_ERROR(("[time(%d)]TimerMgr Erro R(%d) nus(%d) fails!,reset to head", iTimerSeq, \
                                                        iRankIndex, nus)); \
        nus = -1; \
        break; \
    }
    /**
    * 检查指定档的元素是否到期
    * @param[in] iRankIndex 被检查的时间档
    * @param[in] iTimeCount 调用记数，用来判断是否需要对剩下项目作完全遍历
    * @param[in] iNowTime 当前时间，用来判断当前项目是否需要递进到下档队列
    */
    int CheckUpWaitList(int iRankIndex, int iTimeCount, time_t iNowTime)
    {
        int ret = 0;
        int iListIndex = LISTINDEX(iRankIndex);
    
        int iRankInfo = RANKFINFO(iRankIndex);
        int iPevRankIndex = PEVRANK(iRankIndex);

        int iHeadItem = m_stMgr.ListInfo(iListIndex).iHead;
        if (iHeadItem == -1)
        {
            return ret;
        }

        int& iRankNus = m_stListRankInfo[iRankInfo].iRankNonius;
        int iTimeTop = 1;

        int iLoopCount =  m_stMgr.Size(iListIndex);
        
        if (iRankIndex > LIST_RANKREAL)
        {//非准备队列不完全调度
            iTimeTop = m_stListRankInfo[iPevRankIndex].iTimeTop;
            iLoopCount = iLoopCount / iTimeTop;

            if (iRankNus == -1)
            {   //初始化游标
                iRankNus = iHeadItem;
                CT_TRACE(("[time(%d)] reset the R(%d) Ns(%d)", iTimerSeq, iRankIndex, iRankNus));
            }

            //每次循环至少调度一个元素
            iLoopCount = (iLoopCount == 0) ? 1 : iLoopCount;
        }
        else
        {
            iRankNus = iHeadItem;
        }

        CT_TRACE(("[time(%d)] check Rank(%d) hd(%d) tl(%d) ct(%d)", iTimerSeq, iRankIndex,
                            m_stMgr.ListInfo(iListIndex).iHead,
                            m_stMgr.ListInfo(iListIndex).iTail,
                            m_stMgr.ListInfo(iListIndex).iSize));

        while (iRankNus != -1)
        {
            ++ret;
            CT_TRACE(("[time(%d)] checking Ns(%d)", iTimerSeq, iRankNus));   
            CHECK_NONUIUS(iRankNus);
         
            TSTimerTag t = m_stMgr[iRankNus];            
            int iWaitTime = (int)(t.iDeadLine - iNowTime);
            if (iWaitTime <= iTimeTop)
            {//移动到前驱节点去
                CT_TRACE(("[time(%d)] R(%d) Node(%d) move to nR(%d) ", iTimerSeq, iRankIndex,
                                                iRankNus, iPevRankIndex));

                int iTmpNode = iRankNus;
                iRankNus = m_stMgr.Next(iRankNus);
                m_stMgr.InfoListMove(iTmpNode, 0, LISTINDEX(iPevRankIndex));
            }
            else
            {
                iRankNus = m_stMgr.Next(iRankNus);
            }
            
            
            --iLoopCount;

            if (!iLoopCount)
            {
                if ((iTimeCount % iTimeTop) == 0 ||
                    (iRankIndex == CTTimerObjectMgr<T>::LIST_RANKREAL))
                { //如果本次便利刚好到达循环模的话则必须把剩下的节点遍历玩
                    continue;
                }

                CT_TRACE(("[time(%d)] R(%d) LoopCount Out", iTimerSeq, iRankIndex));
                break;
            }          
        }

        CT_TRACE(("[time(%d)] Check Out R(%d) hd(%d), tl(%d), ct(%d), ns(%d)", iTimerSeq,
                        iRankIndex, m_stMgr.ListInfo(iListIndex).iHead,
                        m_stMgr.ListInfo(iListIndex).iTail, 
                        m_stMgr.ListInfo(iListIndex).iSize, iRankNus));  
      
        return ret;
    }
};

template<typename T, unsigned int LOOP_GAP = 100>
class CStandardTimerMgr
{

protected:
    time_t m_uiNow;
	unsigned int	m_iLoop;
    typedef void(*pCallBack)(T &stData);
    pCallBack m_pCallBackFunc;  

    CTTimerObjectMgr<T> m_stTimerList;
    char* m_pszBuf;

    int m_iObjectCount;
    int m_iTimeConut;   ///< 时间计数
    int iTimerSeq;      ///< 定时器编号

public:
    CStandardTimerMgr():
      m_uiNow(time(0)),
      m_pCallBackFunc(NULL),
      m_pszBuf(NULL),
      m_iTimeConut(0),
      iTimerSeq(0)
    {
    }
    virtual ~CStandardTimerMgr()
    {
        if (m_pszBuf)
        {
            delete [] m_pszBuf;
        }
    }
    //初始化定时器设置
    /**                      
    *szConfig配置的格式为(0,conuts)(1,real)(1,small)(1,long) 最多只能有八个门槛
    */
    int BuildTimeMgrConfigVec(const char* pszMgrConfig, vector<string> &astKeyValue)
    {
        string s(pszMgrConfig);
        CTStringUtils::RemoveNot(s, "01234567890,)"); 
        CTStringUtils::Split(s, ")", astKeyValue);
        int ret = 0;
        m_iObjectCount = 0;

        string szFlag, szValue;

        for (size_t i = 0; i < astKeyValue.size(); ++i)
        {
            if (!CTStringUtils::SplitIni(astKeyValue[i], szFlag, szValue, ','))
            {
                return -1;
            }

            switch (CTStringUtils::StrToInt<int>(szFlag.c_str()))
            {
            case 0:
                {
                    if (m_iObjectCount)
                    {
                        return -2;
                    }
                
                    m_iObjectCount = CTStringUtils::StrToInt<int>(szValue.c_str());
                    break;
                }
            case 1:
                ++ret;
                break;
            default:
                return -2;
            }
        }
        return ret;
    }

    int Init(const char* pszMgrConfig, pCallBack psFunc, int iSeq = 0)
    {
        vector<string> astKeyValue;
        int iListCount = BuildTimeMgrConfigVec(pszMgrConfig, astKeyValue);
        if (iListCount <= 0)
        {
            return -1;
        }
        iTimerSeq = iSeq;
        size_t iBufSize = CTTimerObjectMgr<T>::CountSize(m_iObjectCount, iListCount);
        CT_ASSERT(m_pszBuf == NULL);
        m_pszBuf = new char[iBufSize];
        CT_ASSERT(m_pszBuf != NULL);
        CT_TRACE(("[Timer(%d)] Init Timer Buff(%p) Size(%d), itemCount(%d)", iTimerSeq, m_pszBuf, iBufSize, m_iObjectCount));
        m_stTimerList.Init(m_pszBuf, astKeyValue, iSeq);
        CT_ASSERT(psFunc);

        m_pCallBackFunc = psFunc;
        return 0;
    }

    /**
    *定时器注册函数
    *data为定时器注册的数据
    *deadline为定时器到期时间
    */
    int Register(const T& stData, time_t iDeadLine)
    {
        //计算需要等待的时间
        /*
        int iWaitTime = (int)(iDeadLine - time(0));
        if (iWaitTime < 0)
        {
            //m_pCallBackFunc(stData);
            return -1;
        }
        */
        
        int iItemFlag = m_stTimerList.Insert(stData, iDeadLine);
        CT_TRACE(("[Timer(%d)] Register Note(%d) Into timer", iTimerSeq, iItemFlag));
        return iItemFlag;
    }
    /**
    * 删除指定的定时器项目
    * stData 定时数据
    * iItemIndex 定时项索引
    * return  0,删除成功,非零删除失败
    *         1,项目已更新
    *         2,项目索引不正确
    */
    //删除指定项目，如果删除失败
    //
    int DelItem(T stData, int iItemIndex)
    {
        if (iItemIndex < 0 || iItemIndex > m_stTimerList.m_stMgr.GetObjectCount())
        {
            CT_ERROR(("[Timer(%d)] delete Timer Note but Index(%d) fails", iTimerSeq, iItemIndex));
            return 2;
        }

        if (stData == m_stTimerList.m_stMgr[iItemIndex].stData)
        {
            //内容相符，删除掉这一项
            CT_TRACE(("[Timer(%d)] Remove Note(%d) Out", iTimerSeq, iItemIndex));
            int iRet = m_stTimerList.RemoveOut(iItemIndex);
            if (iRet != 0)
            {
                CT_ERROR(("[Timer(%d)] Remove Out Note(%d) fails(%d)", iTimerSeq, iItemIndex, iRet));
            }
            return 0;
        }
        else
        {
            //数据已发生变化，不能删除
            CT_TRACE(("[Timer(%d)] Remove fails Note(%d) Out !", iTimerSeq, iItemIndex));
            return 1;
        }
    }
    /**
    *刷新当前时间,每一秒刷新一次
    */
    int DoLoop()
    {
        //有待改进
        ++ m_iLoop;
        if (m_iLoop >= LOOP_GAP)
        {
            m_iLoop = 0;
            m_uiNow = time(0);
            m_iTimeConut = (++ m_iTimeConut) % m_stTimerList.m_iTimeMode;
            //CheckTimer();
            return 1;
        }

        return 0;
    }

    /**
    * 判断是否有定时器被触发
    * @param[in] pCountData 处理量统计，如果不需要统计值请使用默认值
    */
    int CheckTimer(int* pCountData = NULL)
    {
        //轮询规则
        if (pCountData)
        {
            *pCountData = 0;
        }
         
        //2 其他队列如果不是在触发时间则只调度前n个元素
        int iRankCount = m_stTimerList.GetRankCount();
        for (int iRankID = CTTimerObjectMgr<T>::LIST_RANKREAL; iRankID <= iRankCount + 1; ++iRankID)
        {
     
            iRankID = (iRankID == iRankCount + 1) ? CTTimerObjectMgr<T>::LIST_RANKTAIL : iRankID;
            //CT_TRACE(("[Timer(%d)] Check Up Rank(%d)", iTimerSeq, iRankID));           
            int itmpCount = m_stTimerList.CheckUpWaitList(iRankID, m_iTimeConut, m_uiNow);
            if (pCountData)
            {
                (*pCountData) += itmpCount;
            }                            
        }
        
        //DoReal();
        return 0;
    }

    int GetRealListLen()
    {
        return m_stTimerList.GetRealLen();
    }

    int DoReal(int* pCountData = NULL)
    {
        T stData;
        int iRet = 0;
        //一次执行最多从完成队列中取出的元素个数
        int iLoop = 0;
        while(!iRet)
        {
            iRet = m_stTimerList.DoRealHead(stData);
            if (iRet == 0)
            {
                m_pCallBackFunc(stData);
            }
            else if (iRet < 0)
            {
                CT_ERROR(("[Timer(%d)]Do Real List Head fails(%d)", iTimerSeq, iRet));
                CT_ASSERT(iRet >= 0);
                break;
            }
            ++iLoop;

            //一次最多取出10条命令
            if (iLoop >= 2)
            {
                break;
            }
        }
        return iRet;
    }

    int GetItemCount(int iItemType)
    {
        switch(iItemType)
        {
        case EICY_ALL:
            {
                int OjbCount = 0;
                OjbCount += m_stTimerList.GetAllocListCount();
                for(int i = 0; i < m_stTimerList.GetRankCount() + 1; ++i)
                {
                    OjbCount += m_stTimerList.GetListCount(i);
                }

                OjbCount += m_stTimerList.GetListCount(CTTimerObjectMgr<T>::LIST_RANKTAIL);
                return OjbCount;
            }
        case EICY_FREE:
            {
                return m_stTimerList.GetAllocListCount();
            }
        case EICY_USED:
            {
                int iUseCount = 0;
                for(int i = 0; i < m_stTimerList.GetRankCount() + 1; ++i)
                {
                    iUseCount += m_stTimerList.GetListCount(i);
                }
                iUseCount += m_stTimerList.GetListCount(CTTimerObjectMgr<T>::LIST_RANKTAIL);
                return iUseCount;
            }
        default:
            CT_ERROR(("Check Type err!!"));
            return -1;
        }
        
    }

    T& operator[] (int i)
    {
        return m_stTimerList.GetItem(i).stData;
    }
};

} //CTLib
 
#endif //__CTSTANDARTIMERMGR_H__
