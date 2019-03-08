

#ifndef __CTLIB_MULTIHASHMGR_H__
#define __CTLIB_MULTIHASHMGR_H__

#include "CTHashObjectMgr.h"
#include <string>

namespace CTLib
{

class CVarObjectHead
{
public:
    CVarObjectHead()
    {
        m_iDataInfo = 0;
        m_iDataLen  = 0;
    }
    int m_iDataInfo;     //数据相关元信息
    int m_iDataLen;      //数据长度
};

/*
*可变对象的数据结构
*/
class CVarObject
{
public:
    CVarObject()
    {
        m_stDataHead.m_iDataLen = 0;
    }

    int GetObjectSize()
    {
        return m_stDataHead.m_iDataLen + sizeof(CVarObject);
    }

    int ResetData(const char* pData, const CVarObjectHead& stHead)
    {
        if(pData != NULL)
        {
            m_stDataHead=stHead;
            memcpy(m_chData, pData, stHead.m_iDataLen);
            CT_TRACE(("UPDATE_DATA: memcpy data over"));
        }
        else
        {
            m_stDataHead.m_iDataLen = 0;
        }
        return 0;
    }

    CVarObjectHead m_stDataHead;
    char m_chData[0];
};
};


namespace CTLib
{

//多尺寸对象的hash管理器
template<typename KEY>
class CTMultiHashMgr
{
public:

    enum
    {
        ALLOC_INDEX = 0,    ///< 用来保存节点是否分配
        FREE_LIST = 0,
        USED_LIST = 1,

        HASH_INDEX = 1,     ///< 用来记录冲突链表

    };
    
    CTMultiHashMgr()
    {
    }

    static size_t CountSize(const char* pszMgrConfig, int iListCount = 2, int iIndexCount = 1)
    {
        if (pszMgrConfig == NULL)
        {
            return 0;
        }

        if (iListCount < 2 || iIndexCount < 1)
        {
            return -1;
        }

        int iTotalCount = GetObjectTotalCount(pszMgrConfig);
        if(iTotalCount<=0)
            return 0;
        return CTMultiObjectMgr::CountSize(pszMgrConfig,iListCount,iIndexCount)+
            CTHashObjectMgr<KEY,CTMgrIndex>::CountSize(iTotalCount);
    }

    int Init(const char* pszMgrConfig, const char* pszBuffer,
        int iListCount = 2, int iIndexCount = 1,bool bReset = true)
    {
        if (pszMgrConfig == NULL || pszBuffer == NULL || iListCount < 2 || iIndexCount < 1)
        {
            return -1;
        }

        int iTotalCount = GetObjectTotalCount(pszMgrConfig);
        if(iTotalCount<=0)
        {
            CT_ERROR(("GetObjectTotalCount error count %d",iTotalCount));
            return -1;
        }

        int iRet = m_stObjectIndexHash.Init(pszBuffer,iTotalCount,2,2,bReset);
        if(iRet!=0)
        {
            CT_ERROR(("m_stObjectIndexHash init error"));
            return -1;
        }
        size_t iHashSize = m_stObjectIndexHash.CountSize(iTotalCount);
        size_t iMultiObjectSize = m_stMultiObjectMgr.CountSize(pszMgrConfig,iListCount,iIndexCount);
        iRet = m_stMultiObjectMgr.Init(pszMgrConfig,pszBuffer+iHashSize,
                                      iMultiObjectSize,iListCount,iIndexCount,bReset);
        return iRet;
    }
    

    /*************************************************
    Description:    // 获取元素
    Return:         //
    Others:         // 其它说明
    *************************************************/
    CVarObject*  GetElement(KEY & stKey)
    {
        int iIndex = m_stObjectIndexHash.HashFind(stKey);
        if (iIndex >=0 )
        {

            CVarObject *  pElement =(CVarObject *) m_stMultiObjectMgr.Get(m_stObjectIndexHash[iIndex]);
          
            return pElement;

        }
        return NULL;
        
    }

    /*************************************************
      Description:    // 添加元素
      Return:         // ret >= 0 the hash index,ret<0 failed
      Others:         // 其它说明
    *************************************************/
    int AddElement(KEY& stKey,const char * pData,const CVarObjectHead & stHead,bool bReplace=true)
    {
        int iIndex = m_stObjectIndexHash.HashFind(stKey);
        if (iIndex >= 0)
        {
            CT_TRACE(("UPDATE_DATA: find data in hash del data, key1 = %s, key2 = %llu, dt = %d, op = %d,dataLen %d", 
                stKey.m_sStrID(), stKey.m_u64ID, stKey.m_chDataType,
                stHead.m_iDataInfo,stHead.m_iDataLen));
            m_stMultiObjectMgr.Free(m_stObjectIndexHash[iIndex]);
            m_stObjectIndexHash.HashFreeByIndex(iIndex);
        }

        CT_TRACE(("UPDATE_DATA:  insert in hash , key1 = %s, key2 = %llu, dt = %d, op = %d dataLen %d", 
            stKey.m_sStrID(), stKey.m_u64ID, stKey.m_chDataType, stHead.m_iDataInfo,stHead.m_iDataLen));

        iIndex = m_stObjectIndexHash.HashAlloc(stKey);
        if(iIndex < 0)
        {
            CT_WARNING((""));
            return -1;
        }
        else
        {
            int iMgrIndex = m_stMultiObjectMgr.CalcMgrIndex(stHead.m_iDataLen+sizeof(CVarObject));
            if (iMgrIndex < 0)
            {
                m_stObjectIndexHash.HashFreeByIndex(iIndex);
                CT_ERROR(("CalcMgrIndex error size %u",stHead.m_iDataLen+sizeof(CVarObject)));
                return -1;
            }

            CVarObject* pElement = (CVarObject *)m_stMultiObjectMgr.Alloc(iMgrIndex,&m_stObjectIndexHash[iIndex]);

            CT_TRACE(("UPDATE_DATA: alloc cell from m_stMultiObjectMgr , iMgrIndex = %d, index = %d, objmgr_index = (%d,%d), element = %p", 
                iMgrIndex, iIndex, m_stObjectIndexHash[iIndex].iMgrIndex, m_stObjectIndexHash[iIndex].iObjIndex, pElement));

            if(pElement == NULL)
            {
                m_stObjectIndexHash.HashFreeByIndex(iIndex);
                CT_ERROR(("alloc object error size %u",stHead.m_iDataLen+sizeof(CVarObject)));
                return -1;
            }
            pElement->ResetData(pData,stHead);
        }

        return 0;
    }

    /*************************************************
    Description:    // 删除元素
    Return:         // ret = 0 success,ret<0 failed
    Others:         // 其它说明
    *************************************************/
    int DelElement(KEY & stKey)
    {
        int iIndex = m_stObjectIndexHash.HashFind(stKey);
        if (iIndex >= 0)
        {
            CT_TRACE(("UPDATE_DATA: delete element , key1 = %s, key2 = %llu, dt = %d,  index = %d, obj_mgr = (%d,%d)", 
                stKey.m_sStrID(), stKey.m_u64ID, stKey.m_chDataType,
                iIndex, m_stObjectIndexHash[iIndex].iMgrIndex, m_stObjectIndexHash[iIndex].iObjIndex));

            m_stMultiObjectMgr.Free(m_stObjectIndexHash[iIndex]);
            m_stObjectIndexHash.HashFreeByIndex(iIndex);
			return 0;
        }
        
        return -1;
    }
    /*************************************************
    Description:    // 批量从头部获取元素
    Return:         // ret 元素个数,ret<0 failed
    Others:         // 其它说明
    *************************************************/
    int GetHeadElements(CVarObject ** ppstElement,KEY* pstKeys,int iNeedNum)
    {
        if (ppstElement == NULL || pstKeys == NULL || iNeedNum <= 0)
        {
            return -1;
        }

        int iGetNum = 0;

        int iIndex = m_stObjectIndexHash.GetHeadIndex();
             
        while (iIndex >= 0 && iGetNum < iNeedNum)
        {
            
  
            ppstElement[iGetNum] = (CVarObject *) m_stMultiObjectMgr.Get(m_stObjectIndexHash[iIndex]);
            if(ppstElement[iGetNum] == NULL)
            {
                return iGetNum;
            }
            pstKeys[iGetNum] = m_stObjectIndexHash.GetKeyByIndex(iIndex);

            CT_TRACE(("UPDATE_DATA: get data in hash , key1 = %s, key2 = %llu, dt = %d, op = %d,dataLen %d", 
                pstKeys[iGetNum].m_sStrID(), pstKeys[iGetNum].m_u64ID, pstKeys[iGetNum].m_chDataType,
                ppstElement[iGetNum]->m_stDataHead.m_iDataInfo,
                ppstElement[iGetNum]->m_stDataHead.m_iDataLen));

            iGetNum++;
            iIndex = m_stObjectIndexHash.GetNextIndex(iIndex);
            
        }
        return iGetNum;
    }

    /*************************************************
    Description:    // 或取头结点的下标
    Return:         // 结点下标，<0失败
    Others:         // 其它说明
    *************************************************/
    int GetHeadIndex()
    {
        return m_stObjectIndexHash.GetHeadIndex();
    }

    /*************************************************
    Description:    // 批量从指定点获取元素
    Return:         // ret 元素个数,ret<0 failed
    Others:         // 其它说明
    *************************************************/
    int GetNextElements(int & iNextIndex, CVarObject ** ppstElement, KEY* pstKeys, int iNeedNum)
    {
        if (ppstElement == NULL || pstKeys == NULL || iNeedNum <= 0)
        {
            return -1;
        }

        if(iNextIndex < 0 )
        {
            return 0;
        }

        int iGetNum = 0;

        if(!m_stObjectIndexHash.CheckIndexUsed(iNextIndex))
        {
            iNextIndex = -1;
            return 0;
        }

        while(iNextIndex>=0 && iGetNum < iNeedNum)
        {
            
            ppstElement[iGetNum] = (CVarObject *) m_stMultiObjectMgr.Get(m_stObjectIndexHash[iNextIndex]);
            if(ppstElement[iGetNum] == NULL)
            {
                return iGetNum;
            }
            pstKeys[iGetNum] = m_stObjectIndexHash.GetKeyByIndex(iNextIndex);

            CT_TRACE(("UPDATE_DATA: get data in hash , key1 = %s, key2 = %llu, dt = %d, op = %d,dataLen %d", 
                pstKeys[iGetNum].m_sStrID(), pstKeys[iGetNum].m_u64ID, pstKeys[iGetNum].m_chDataType,
                ppstElement[iGetNum]->m_stDataHead.m_iDataInfo,
                ppstElement[iGetNum]->m_stDataHead.m_iDataLen));

            iGetNum++;
            iNextIndex = m_stObjectIndexHash.GetNextIndex(iNextIndex);

        }
        return iGetNum;
    }


    /*************************************************
    Description:    // 检查是否为空
    Others:         // 其它说明
    *************************************************/
    bool CheckEmpty()
    {
        int iIndex = m_stObjectIndexHash.GetHeadIndex();
        if(iIndex < 0)
        {
            return true;
        }
        return false;
    }

    /*************************************************
    Description:    // 检查使用率是否已经超过所给的百分比
    @iPententage 检查的百分比
    Others:         // 其它说明
    *************************************************/
    bool CheckPassUsedPentage(int iPententage)
    {
        size_t iSize = m_stMultiObjectMgr.GetMgrCount();
        for (size_t i = 0; i < iSize; ++i)
        {
            int iTotalCount = m_stMultiObjectMgr.GetMgrObjectCount(i);
            if(iTotalCount <= 0)
            {
                CT_ERROR(("magr index %d count %d error",i,iTotalCount));
                return true;
            }

            //先检查满的情况
            int64 i64TimeSize =  m_stMultiObjectMgr.Size(i, ALLOC_INDEX, USED_LIST)*100; 

            if(m_stMultiObjectMgr.Size(i, ALLOC_INDEX, USED_LIST) == iTotalCount)
            {
                return true;
            }        
            else if(i64TimeSize/iTotalCount > (int64)iPententage)
            {
                CT_TRACE(("mgr %d is pass %d ",i,iPententage));
                return true;
            }
        }
        return false;
    }
    const char * GetStatInfo()
    {
        size_t iSize = m_stMultiObjectMgr.GetMgrCount();

        m_szStatInfo.Set("UPDATEMGR HASHLIST :");

        for (size_t i = 0; i < iSize; ++i)
        {

            int iTotalCount = m_stMultiObjectMgr.GetMgrObjectCount(i);

            m_szStatInfo.Cat("(%d,%d) ",iTotalCount,m_stMultiObjectMgr.Size(i, ALLOC_INDEX, USED_LIST));
            
        }
        return m_szStatInfo();
    }
protected:
    static int GetObjectTotalCount(const char*pszMgrConfig)
    {
        if (pszMgrConfig == NULL)
        {
            return -1;
        }

        vector<CTMgrConfig> ast;
        int iTotalCount = 0;
        int iRet = CTMultiObjectMgr::BuildMgrConfigVec(pszMgrConfig, ast);
        if (iRet)
        {
            return -1;
        }
        for (unsigned int i=0; i < ast.size(); i++)
        {
            iTotalCount +=ast[i].iObjectCount;
        }
        return iTotalCount;
    }


private:
    CTSizeString<4096> m_szStatInfo;
    //索引ｈａｓｈ
    CTHashObjectMgr<KEY,CTMgrIndex> m_stObjectIndexHash;
    //多尺寸对象列表管理器
    CTMultiObjectMgr m_stMultiObjectMgr;


};

};

#endif

