

#ifndef _CDATA_UPDATEMSGR_H_
#define _CDATA_UPDATEMSGR_H_
#include "CTMultiHashMgr.h"

#define MAX_STRID_LENGTH 33

template<typename KEY>
class CUpdateDataMgr
{
public:
    typedef int (*UPDATE_FUNC)(CVarObject* pData, KEY* stKey);

    enum
    {
        MGR_MAIN =0,          //主hash
        MGR_ASSIST=1,         //测试hash

        EHASH_MGR_NUM = 2,

        MAX_FLUSH_NUM = 8000,   //最大刷新数目
        EACH_INCREACE_NUM = 2, //慢启动的增长数目
    };

    CUpdateDataMgr()
    {
        m_pMainIndex = NULL;
        m_pSwapCount = NULL;

        m_iSendIndex = -1;

        m_iInitFlushNum = 100;
        m_iMaxFlushNum = 300;
        m_iWindowNum = 100;
        int iTime = time(NULL);
        m_iLastFlushTime = iTime;
        m_iLastSwapTime  = iTime;
    }

    size_t CountSize(const char* pszMgrConfig)
    {
        if (pszMgrConfig == NULL)
        {
            return 0;
        }

        return m_UpdateDataMgr[0].CountSize(pszMgrConfig)*2 + sizeof(unsigned char) + sizeof(unsigned int);
    }

    /*
    *bNeedSendSwapSql 是否需要生成swap的sql
    */
    int Init(UPDATE_FUNC pUpdateFunc,const char* pszMgrConfig,const char* pszBuffer,
        int initFlushNum=100,int iMaxFlushNum =300,bool bReset=true, bool bNeedSendSwapFlag = false)
    {
        if (pszBuffer == NULL || pszMgrConfig == NULL || pUpdateFunc == NULL)
        {
            return -1;
        }
        if (iMaxFlushNum > MAX_FLUSH_NUM)
        {
            m_iMaxFlushNum = MAX_FLUSH_NUM;
        }
        else
        {
            m_iMaxFlushNum = iMaxFlushNum;
        }

        if (initFlushNum > m_iMaxFlushNum)
        {
            initFlushNum = m_iMaxFlushNum;
        }
        //系统是否发送swapcount的控制开关
        m_bNeedSendSwapFlag = bNeedSendSwapFlag;
        //单次发送swapcount的控制开关
        m_bNeedSendSwapSql  = true;

        m_iWindowNum = initFlushNum;

        m_pUpdateFunc = pUpdateFunc;

        m_pMainIndex = (unsigned char * )pszBuffer;
        size_t iNowCount = sizeof(unsigned char);

        m_pSwapCount = (unsigned int*)(pszBuffer+iNowCount);
        iNowCount    += sizeof(unsigned int);

        if(bReset)
        {
            *m_pMainIndex = 0;
            *m_pSwapCount = 0;
        }
        else
        {
           
            if (EHASH_MGR_NUM < 2 || *m_pMainIndex >= EHASH_MGR_NUM )
            {
                CT_ERROR(("main index error"));
                return -1;
            }
        }

        int iRet = m_UpdateDataMgr[0].Init(pszMgrConfig,pszBuffer+iNowCount,2,1,bReset);
        if(iRet!=0)
        {
            return iRet;
        }

        iNowCount += m_UpdateDataMgr[0].CountSize(pszMgrConfig);
        iRet = m_UpdateDataMgr[1].Init(pszMgrConfig,pszBuffer+iNowCount,2,1,bReset);
        return iRet;
    }

    //
    /*
    *重新设置刷新的初始值和最大值
    */
    void ReSetUpdateFlushNum(int initFlushNum,int iMaxFlushNum)
    {
        if(initFlushNum <= 0 || iMaxFlushNum <= 0)
        {
            return ;
        }

        if (iMaxFlushNum > MAX_FLUSH_NUM)
        {
            m_iMaxFlushNum = MAX_FLUSH_NUM;
        }
        else
        {
            m_iMaxFlushNum = iMaxFlushNum;
        }
        if (initFlushNum > m_iMaxFlushNum)
        {
            initFlushNum = m_iMaxFlushNum;
        }
        m_iInitFlushNum = initFlushNum;
    }


    /*************************************************
    Description:    检查更新，负责往ｄｂ发送更新命令
    Input:          @iTimeStep 检查步长
    *************************************************/
    void CheckUpdate(int iTimeStep,time_t iNowTime)
    {
        if (iTimeStep <= 0)
        {
            iTimeStep = 1;
        }

        //时间到，启动下一次刷新
        if (iNowTime-m_iLastFlushTime >= iTimeStep)
        {
            int iIndex = GetDataMgrIndex(MGR_MAIN);
            //CT_TRACE(("UPDATE_DATA: GetDataMgrIndex(MGR_MAIN), index = %d", iIndex));
            if(iIndex < 0)
            {
                CT_ERROR(("get main data mgr error"));
                return;
            }

            CVarObject * astObj[MAX_FLUSH_NUM]={0};
            KEY stKeys[MAX_FLUSH_NUM];
            //判断index是否合法，不合法进行调整
            if(CheckSendIndex()!=0)
            {
                m_iLastFlushTime = iNowTime;
                return;
            }
            //可发送数目为窗口大小加上一个慢启动的增量,并且保证发送数目不为0
            int iCanSendCount = m_iWindowNum*EACH_INCREACE_NUM+1;
            if(iCanSendCount > m_iMaxFlushNum)
            {
                iCanSendCount = m_iMaxFlushNum;
                if (m_iWindowNum > m_iMaxFlushNum)
                {
                    m_iWindowNum = m_iMaxFlushNum;
                }
            }
            int iNum = m_UpdateDataMgr[iIndex].GetNextElements(m_iSendIndex,astObj,stKeys,iCanSendCount);
            CT_INFO(("UPDATE_DATA: GetHeadElements iNum = %d iWindow %d iCanSend %d",iNum,m_iWindowNum,iCanSendCount));
            if(iNum >= 0)
            {
                m_iLastFlushTime = iNowTime;
                int iCount = 0;
                for(int i=0;i<iNum;i++)
                {
                    //发送数据更新命令
                    int iRet = m_pUpdateFunc(astObj[i],&stKeys[i]);
                    if(iRet!=0)
                    {
                        CT_WARNING(("update data error  ret=%d ",
							iRet));
                    }
                    else
                    {
                        m_iWindowNum--;
                        iCount++;
                    }
                    CT_TRACE(("UPDATE_DATA: update data over, send count = %d", iCount));

                }
                //保证窗口大于0
                if (m_iWindowNum < 0)
                    m_iWindowNum = 0;
            }
            
            else
            {
                //保证窗口大于0
                if (m_iWindowNum < 0)
                    m_iWindowNum = 0;
                CT_ERROR(("GetHeadElements error"));
            }
        }
    }

    /*************************************************
    Description:    更新命令的响应
    Input:          @stKey 更新数据的ｋｅｙ
    *************************************************/
    void UpdateResponse(KEY & stKey)
    {
        CT_TRACE(("UPDATE_DATA: UpdateResponse , key1 = %s, key2 = %llu, dt = %d", 
            stKey.m_sStrID(), stKey.m_u64ID, stKey.m_chDataType));

        int iRet = DeleteData(stKey);
        if (iRet != 0)
        {
            CT_ERROR(("delete from update mgr error, ret %d, key1 = %s, key2 = %llu, dt = %d", 
                            iRet, stKey.m_sStrID(), stKey.m_u64ID, stKey.m_chDataType));
            return;
        }
        m_iWindowNum++;
    }

    int DeleteData(KEY & stKey)
    {
        int iIndex = GetDataMgrIndex(MGR_MAIN);
        CT_TRACE(("UPDATE_DATA: GetDataMgrIndex, iIndex = %d", iIndex));
        if (iIndex < 0)
        {
            CT_ERROR(("get main data mgr error"));
            return -1;
        }

        if (m_UpdateDataMgr[iIndex].DelElement(stKey) == 0)
        {
            CT_TRACE(("UPDATE_DATA: update success , key1 = %s, key2 = %llu, dt = %d", 
                stKey.m_sStrID(), stKey.m_u64ID, stKey.m_chDataType));
        }
        else
        {
            return -2;
        }

        return 0;
    }

    /*************************************************
    Description:    添加数据更新项
    Input:          @stKey 更新数据的ｋｅｙ
    @szData 更新数据指针
    @iDataSize 尺寸
    return          0 success,else failed
    *************************************************/
    int UpdateData(KEY & stKey, const char * szData, const CVarObjectHead & stHead)
    {
        int iIndex = GetDataMgrIndex(MGR_ASSIST);
        if (iIndex < 0)
        {
            CT_ERROR(("get main data mgr error"));
            return -1;
        }

        CT_TRACE(("UPDATE_DATA: begin add element key = %llu, key2 = %s datatype=%d optype=%d datalen %d",
            stKey.m_u64ID, stKey.m_sStrID(),stKey.m_chDataType, stHead.m_iDataInfo,stHead.m_iDataLen));
        int iRet = m_UpdateDataMgr[iIndex].AddElement(stKey,szData,stHead);
        if (iRet != 0)
        {
            CT_ERROR(("UPDATE_DATA: add element to assist mgr key = %llu, key2 = %s datatype=%d optype=%d datalen %d error iRet %d",
                stKey.m_u64ID, stKey.m_sStrID(),stKey.m_chDataType, stHead.m_iDataInfo,stHead.m_iDataLen,iRet));
        }
        return iRet;
    }

    /*************************************************
    Description:    获取数据，用于在从dbsvr/mysql获取数据前检查数据已经在数据区中
    Input:          @stKey 更新数据的ｋｅｙ
    @szData 用于存放数据的内存空间指针
    @iMaxDataLen 数据空间最大长度
    return          0 表示找到数据，且可以放入szData
    *************************************************/
    int GetData(KEY& stKey, char* szData, int iMaxDataLen, CVarObjectHead & stHead)
    {
        if (szData == NULL || iMaxDataLen == 0)
        {
            return -1;
        }

        //优先查找辅链
        int iIndex = GetDataMgrIndex(MGR_ASSIST);
        if(iIndex < 0)
        {
            CT_ERROR(("get main data mgr error index = %d",iIndex));
            return -1;
        }

        CVarObject *pszVarObject = m_UpdateDataMgr[iIndex].GetElement(stKey);
        if (NULL == pszVarObject)  //辅链中没有找到，则查询主链
        {
            iIndex = GetDataMgrIndex(MGR_MAIN);
            if (iIndex < 0)
            {
                CT_ERROR(("get main data mgr error index=%d",iIndex));
                return -2;
            }

            pszVarObject = m_UpdateDataMgr[iIndex].GetElement(stKey);
            if (NULL == pszVarObject)  //没有找到
            {
                return -3;
            }
        }

        //已找到数据
        if (iMaxDataLen < pszVarObject->m_stDataHead.m_iDataLen) //数据空间不够大
        {
            CT_ERROR(("data space is not enough"));
            return -4;
        }
        if (pszVarObject->m_stDataHead.m_iDataLen != 0)
        {
            memcpy(szData, pszVarObject->m_chData, pszVarObject->m_stDataHead.m_iDataLen);
        }
      
        stHead = pszVarObject->m_stDataHead;      
        return 0;
    }

    /*************************************************
    Description:    获取数据头信息
    Input:          @stKey 更新数据的ｋｅｙ
    return          0 表示找到数据
    *************************************************/
    int GetDataHead(KEY& stKey, CVarObjectHead & stHead)
    {
        //优先查找辅链
        int iIndex = GetDataMgrIndex(MGR_ASSIST);
        if(iIndex < 0)
        {
            CT_ERROR(("get main data mgr error index = %d",iIndex));
            return -1;
        }

        CVarObject *pszVarObject = m_UpdateDataMgr[iIndex].GetElement(stKey);
        if (NULL == pszVarObject)  //辅链中没有找到，则查询主链
        {
            iIndex = GetDataMgrIndex(MGR_MAIN);
            if (iIndex < 0)
            {
                CT_ERROR(("get main data mgr error index=%d",iIndex));
                return -2;
            }

            pszVarObject = m_UpdateDataMgr[iIndex].GetElement(stKey);
            if (NULL == pszVarObject)  //没有找到
            {
                return -3;
            }
        }

        stHead = pszVarObject->m_stDataHead;   
        return 0;
    }
    

    /*************************************************
    Description:    检查数据区是否被清空，主要用于停机过程中的检查
    Input:          
    *************************************************/
    bool CheckEmpty()
    {
        for (int i=0; i<EHASH_MGR_NUM; i++)
        {
            if(!m_UpdateDataMgr[i].CheckEmpty())
                return false;
        }
        return true;
    }

    /*************************************************
    Description:    获取切换次数，主要用于将数据更新到dbsvr时标识数据的批号，在mainsvr是一批的数据更新到dbsvr时存放在不同的数据链中
    @iPententage 
    Others:        
    *************************************************/
    int GetSwapCount()
    {
		if (m_pSwapCount == NULL)
		{
			CT_ERROR(("SwapCount point error"));
			return 0;
		}
        return *m_pSwapCount;
    }

    /*************************************************
    Description:    // 检查使用率是否已经超过所给的百分比
    @iPententage 检查的百分比
    Others:         // 其它说明
    *************************************************/
    bool CheckPassUsedPentage(int iPententage)
    {
        if (iPententage <= 0)
        {
            return true;
        }

        int iIndex = GetDataMgrIndex(MGR_ASSIST);
        if(iIndex < 0)
        {
            CT_ERROR(("get main data mgr error index=%d",iIndex));
            return -1;
        }
        return m_UpdateDataMgr[iIndex].CheckPassUsedPentage(iPententage);
    }

    const char * GetStatInfo()
    {
        m_szStatInfo.Set("DATAUPDATEMGR MAIN INDEX %d \n",*m_pMainIndex);
        for (int i=0; i<2; i++)
        {
            m_szStatInfo.Cat("%s\n",m_UpdateDataMgr[i].GetStatInfo());
        }
        return m_szStatInfo();
    }

    void CheckSwap(int iTimeStep,time_t iNowTime,int iSwapPerCentage, bool* bSwapMgr = NULL)
    {
        if (iTimeStep <= 0)
        {
            iTimeStep = 1;
        }

        if (iSwapPerCentage <= 0)
        {
            iSwapPerCentage = 10;
        }

        int iAssistIndex = GetDataMgrIndex(MGR_ASSIST);

        if (bSwapMgr)
        {
            *bSwapMgr = false;
        }

        //判断是否需要交换,两种情况：1.时间到 2辅链占用率过高
        if (iNowTime - m_iLastSwapTime >= iTimeStep || 
            m_UpdateDataMgr[iAssistIndex].CheckPassUsedPentage(iSwapPerCentage) )
        {
            //CheckAddSwapSqlData();
            int iMainIndex = GetDataMgrIndex(MGR_MAIN);
            if (m_UpdateDataMgr[iMainIndex].CheckEmpty())
            {
				m_iLastSwapTime = iNowTime;
                //CT_TRACE(("UPDATE_DATA: iAssistIndex = %d", iAssistIndex));
                if(!m_UpdateDataMgr[iAssistIndex].CheckEmpty())
                {
                    CT_TRACE(("UPDATE_DATA: begin swap mgr"));
                    //开始交换
                    SwapMgr();
                    CT_INFO(("main data flush over ,swap data updater time %d swapCount %d",iNowTime,*m_pSwapCount));
   
                    if (bSwapMgr)
                    {
                        *bSwapMgr = true;
                    }
                }
                else
                {
                    //CT_INFO(("main data flush over ,two hash is empty,no need swap time %d swapCount %d",iNowTime,*m_pSwapCount));
                }
            }
        }
    }

    int GetLastSwapTime()
    {
        return m_iLastSwapTime;
    }


protected:
    int GetDataMgrIndex(int iType)
    {
		CT_ASSERT(m_pMainIndex!=NULL);
		CT_ASSERT(*m_pMainIndex < EHASH_MGR_NUM);
		
        if(iType == MGR_MAIN)
        {
            return *m_pMainIndex;
        }
        else
        {
            return EHASH_MGR_NUM-1-*m_pMainIndex;
        }
    }

    int CheckSendIndex()
    {
        int iIndex = GetDataMgrIndex(MGR_MAIN);
        //游标非法就调整到头部
        if (m_iSendIndex < 0)
        {
            m_iSendIndex = m_UpdateDataMgr[iIndex].GetHeadIndex(); 
        }
        if (m_iSendIndex < 0)
        {
            //CT_TRACE(("hash is empty index %d",iIndex));
            return -1;
        }
        return 0;
    }

    void SwapMgr()
    {
		CT_ASSERT(m_pMainIndex != NULL);
		CT_ASSERT(m_pSwapCount != NULL);
		CT_ASSERT(*m_pMainIndex < EHASH_MGR_NUM);

        *m_pMainIndex = EHASH_MGR_NUM-1-*m_pMainIndex;
        *m_pSwapCount = *m_pSwapCount+1;
        //调整游标从开始位置开始
        m_iSendIndex = -1;
        //切换完成，恢复标志
        m_bNeedSendSwapSql = true;
    }

    void CheckAddSwapSqlData()
    {
        if (!m_bNeedSendSwapFlag)
        {
            return;
        }
        int iMainIndex = GetDataMgrIndex(MGR_MAIN);
		int iAssistIndex = GetDataMgrIndex(MGR_ASSIST);
        if (m_UpdateDataMgr[iMainIndex].CheckEmpty())
        {
			//两桶都空不换桶
            if (m_bNeedSendSwapSql && (!m_UpdateDataMgr[iAssistIndex].CheckEmpty()))
            {
                KEY stKey;
                stKey.m_u64ID=*m_pSwapCount;
                stKey.m_chDataType = DT_SWAPCOUNT;
                CVarObjectHead stHead;
                stHead.m_iDataInfo = 0;
                stHead.m_iDataLen = 0;
                if (m_UpdateDataMgr[iMainIndex].AddElement(stKey,NULL,stHead) == 0)
                {
                    m_bNeedSendSwapSql = false;
                    CT_INFO(("add swap count to updatemgr %d",*m_pSwapCount));
                }
                else
                {
                    CT_ERROR(("add swap sql error"));
                }
            }
        }
    }

    UPDATE_FUNC m_pUpdateFunc;

private:

    //每次刷新数目
    int m_iInitFlushNum;

    //当次时间发送请求的数目
    int m_iMaxFlushNum;

    //当前发送窗口大小
    int m_iWindowNum;

    //上次刷新时间
    int m_iLastFlushTime;

    //上次交换时间
    int m_iLastSwapTime;

    //数据发送的位置游标
    int m_iSendIndex;


    //主hash的索引
    unsigned char * m_pMainIndex;

    //交换次数
    unsigned int *m_pSwapCount;

    //是否需要发送swapcount更新
    bool m_bNeedSendSwapSql;

    //是否往发送swapcount 开关，初始控制
    bool m_bNeedSendSwapFlag;

    CTMultiHashMgr<KEY> m_UpdateDataMgr[EHASH_MGR_NUM];
    CTSizeString<8192> m_szStatInfo;
};


#endif

