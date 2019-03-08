/*
this template is used to maintain data paires those should be indexed by two keys.
Each primary key has only one corresponding object, secondary key may have multiple objects.
Iteration methods are provided for both primary key and secondary key 
*/
#ifndef __CTDOUBLEKEYEDHASHMGR_H__
#define __CTDOUBLEKEYEDHASHMGR_H__
#include "CTHashObjectMgr.h"
#include "CTArray.h"
#include <string>

using namespace CTLib;

template<typename PRIKEY,
         typename SECKEY,
         typename VALUE,
         int    HEADLISTLEN,
         int    SECLISTLEN>
class CTDoubleKeyedHashMgr
{
public:
    typedef class ValPair
    {
    public:
        SECKEY  stSecKey;
        VALUE   stValue;
        bool    bIsHaveSecKey;
        ValPair()
        {
            OBJ_CHECK_RELOAD();
            bIsHaveSecKey = false;
        }

        ~ValPair()
        {
            
        }

        OBJ_CHECK_BEGIN(ValPair)
        OBJ_CHECK_END()
    } CTValPair;

    typedef class CSecMapList        
    {
    private:
        CTArray<PRIKEY, SECLISTLEN> m_astMappedList;

    public:
        CSecMapList()
        {}
        ~CSecMapList()
        {}
        
        int GetMaxCount() const
        {
            return SECLISTLEN;
        }

        
        int GetUseCount() const
        {
            return m_astMappedList.Size();
        }

        //根据下标获取key
        int GetKeyByIdx(int iIdx, PRIKEY& tKey) const
        {
            if (0 > iIdx || m_astMappedList.Size() <= iIdx)
            {
                CT_ERROR(("Index %d is invalid, used count is %d", iIdx, m_astMappedList.GetCount()));
                return -1;
            }

            tKey = m_astMappedList[iIdx];
            return 0;
        }

        //添加一个新的key
        int AddNewKey(const PRIKEY& tPriKey)
        {
            int iRet = m_astMappedList.AddOneItem(tPriKey);
            if (-1 == iRet)
            {//Adding error
                return -1;
            }
            return 0;
        }

        //从数组中删除一个节点
        int DelKey(const PRIKEY& tPriKey)
        {
            for (int iIdx = 0; iIdx < m_astMappedList.Size(); iIdx ++)
            {
                if (tPriKey == m_astMappedList[iIdx])
                {
                    m_astMappedList.ShiftDelOneItem(iIdx);
                    return 0;
                }
            }
            return -1;
        }

        //获取对象所需的内存大小
        static size_t CountSize()
        {
            return sizeof(CSecMapList);
        }
        
    } CTSecMapList;

    enum 
    { 
        EMAX_OBJ_PERPAGE = 200,
    };
    
protected:
    //主hash，维护prikey和val的映射关系，存储于共享内存中
    CTHashObjectMgr<PRIKEY, CTValPair>  m_stPriHash;

    //申请表逆向索引hash，非共享内存对象
    CTHashObjectMgr<SECKEY, CTSecMapList>  m_stSecHash;

    //用于有序的维护前HEADLISTLEN个申请对象索引的数组，非共享内存对象
    CTArray<PRIKEY, HEADLISTLEN>  m_astHeadList;

    //allocated address for secondary hash.
    char* m_pSecHashAddr;

private:

    int RebuildRelevancy()
    {
        //Clear the reverse hash and heading list first.
        int iIdx = m_stSecHash.GetHeadIndex();
        while (0 <= iIdx)
        {
            int iCurIdx = iIdx;
            iIdx = m_stSecHash.GetNextIndex(iIdx);
            m_stSecHash.HashFreeByIndex(iCurIdx);            
        }

        m_astHeadList.Clear();
        
        //Find each node in apply hash, then insert them into the reverse hash and heading list, 
        iIdx = m_stPriHash.GetHeadIndex();
        while(0 <= iIdx)
        {
            CTValPair stValPair = m_stPriHash[iIdx];
            PRIKEY tPriKey = m_stPriHash.GetKeyByIndex(iIdx);
            if (stValPair.bIsHaveSecKey)
            {
                int iRet = AddToSecHash(tPriKey, stValPair.stSecKey);
                if (0 != iRet)
                {
                    CT_ERROR(("Fail to add key into reverse hash, ret = %d", iRet));
                    return iRet;
                }
            }
            AddToHeadList(tPriKey);
            iIdx = m_stPriHash.GetNextIndex(iIdx);
        }

        int iObjCnt = m_stPriHash.GetObjectCount();
        
        CT_INFO(("Application hash and reverse hash are rebuilt, %d items are found", iObjCnt));
        return 0;
    }

    int AddToSecHash(const PRIKEY& tPriKey, const SECKEY& tSecKey)
    {
        //Find the corresponding teacher cell, allocate one if necessary
        int iIdx = m_stSecHash.HashFind(tSecKey);
        if (0 > iIdx)
        {//This teacher is not found, let's allocate one cell for it.
            iIdx = m_stSecHash.HashAlloc(tSecKey);
            if (0 > iIdx)
            {
                CT_ERROR(("Fail to allocate a cell from sec hash"));
                return -1;
            }
        }

        //insert this primary key into sec key;s list
        int iRet = m_stSecHash[iIdx].AddNewKey(tPriKey);
        if (0 != iRet)
        {
            CT_WARNING(("Fail to insert primary key into sec hash"));
            return iRet;
        }
        
        return 0;
    }

    //append a new key to the head list
    int AddToHeadList(const PRIKEY& tPriKey)
    {
        return m_astHeadList.AddOneItem(tPriKey);        
    }

    //delete the specified key from head list
    int DelFromHeadList(const PRIKEY& tPriKey)
    {
        //find the prikey and delete it if it's found
        for (int iIdx = 0; iIdx < m_astHeadList.Size(); iIdx ++)
        {
            if (tPriKey == m_astHeadList[iIdx])
            {
                PRIKEY tKey = m_astHeadList[m_astHeadList.Size() - 1];
                m_astHeadList.ShiftDelOneItem(iIdx);
                
                //Find the next key to the last key in this head list                
                PRIKEY tNextKey;
                int iRet = GetNextPriKey(tKey, tNextKey);
                if (0 == iRet)
                {//It's found, let's append it to the head list.
                    m_astHeadList.AddOneItem(tNextKey);
                }
                break;
            }
        }
        return 0;
    }

    //Delete primary key from sec hash
    int DelFromSecHash(const PRIKEY& tPriKey, const SECKEY& tSecKey)
    {
        //Find the second hash first.
        int iIdx = m_stSecHash.HashFind(tSecKey);
        if (0 > iIdx)
        {//This sec hash node is not found.
            CT_ERROR(("Trying to delete a non-existing node"));
            return -1;                
        }

        //del this primary key from sec key's list
        int iRet = m_stSecHash[iIdx].DelKey(tPriKey);
        if (0 != iRet)
        {
            CT_WARNING(("Fail to delete primary key from sec hash"));
            return iRet;
        }

        //free this sec key from secondary hash if there is no prikey associated with it.
        if (m_stSecHash[iIdx].GetUseCount() == 0)
        {
            m_stSecHash.HashFreeByIndex(iIdx);
        }
        
        return 0;
    }

public:
    CTDoubleKeyedHashMgr()
    {
        m_pSecHashAddr = NULL;
    }
    ~CTDoubleKeyedHashMgr()
    {
        if (NULL != m_pSecHashAddr)
        {
            delete[] m_pSecHashAddr;
        }
    }

    //计算需要的共享内存,只有primary hash放在共享内存中，secondaryhash放置于堆中
    static size_t CountSize(int iDataSize)
    {
        if (0 >= iDataSize)
        {
            CT_THROW(("Invalid object count in counting double keyed hash object size"));            
        }
        size_t lShmSize = CTHashObjectMgr<PRIKEY, CTValPair>::CountSize(
            iDataSize, 2, 2);
        CT_TRACE(("Doublekeyedhash needs %ld bytes", lShmSize));
        return lShmSize;
    }

    //初始化数据，如果不是reload数据就重新建立secondary hash与primary hash之间的关系
    int Init(char* pszBuff, int iDataSize, bool bIsRstShm)
    {
        if (NULL == pszBuff || 0 >= iDataSize)
        {
            CT_ERROR(("NULL shm addr %p or invalid item count %d", pszBuff, iDataSize));
            return -1;
        }

        //Init secondary hash.
        size_t lRvsHashSize = CTHashObjectMgr<SECKEY, CTSecMapList>::CountSize(iDataSize);
        m_pSecHashAddr = new char[lRvsHashSize];
        if (NULL == m_pSecHashAddr)
        {
            CT_ERROR(("Fail to allocate %ld bytes for secondary hash", lRvsHashSize));
            return -1;
        }

        int iRet = m_stSecHash.Init(m_pSecHashAddr, iDataSize, 2, 2, true);
        if (0 != iRet)
        {
            CT_ERROR(("Fail to init secondary apply hash, ret = %d", iRet));
            return iRet;
        }

        //Now init the primary hash
        iRet = m_stPriHash.Init(pszBuff, iDataSize, 2, 2, bIsRstShm);
        if (0 != iRet)
        {
            CT_ERROR(("Fail to init primary hash, ret = %d", iRet));
            return iRet;
        }

        //Now rebuild the relevancy of primary hash & secondary hash if its not going to reset shm
        if (false == bIsRstShm)
        {
            iRet = RebuildRelevancy();
            CT_TRACE(("Rebuilt relevancy of primary hash and secondary hash, ret = %d", iRet));
        }        
        return iRet;  
    
    }

    //插入一条申请
    int AddObj(
        const PRIKEY& tPriKey, 
        const SECKEY& tSecKey, 
        const VALUE& tVal,
        bool bIsMapToSecKey)
    {
        //Check if this user has already inserted an application item
        int iIdx = m_stPriHash.HashFind(tPriKey);
        if (0 <= iIdx)
        {
            CT_WARNING(("Try to insert an application item for existing key at index %d?", 
                iIdx));
            return -1;
        }        

        //If the it needs to be mapped to its sec key and the sec key list is full, quit
        if (bIsMapToSecKey)
        {
            int iSecKeyListLen = GetPriKeyCntBySecKey(tSecKey);
            if (SECLISTLEN == iSecKeyListLen)
            {
                CT_WARNING(("the sec key has %d pri key mapped already!", SECLISTLEN));
                return -2;
            }
        }
        
        //Allocate a new socket in hash for this applicant
        iIdx = m_stPriHash.HashAlloc(tPriKey);    
        if (0 > iIdx)
        {
            CT_WARNING(("fail to allocate object for prikey, total cnt is %d", Size()));
            return -3;
        }
        
        CTValPair stValPair;
        stValPair.stSecKey = tSecKey;
        stValPair.stValue = tVal;
        stValPair.bIsHaveSecKey = bIsMapToSecKey;
        m_stPriHash[iIdx] = stValPair;

        //Now insert it into the reverse mapping hash too. please define a enum for each type of application
        if (bIsMapToSecKey)
        {
            int iRet = AddToSecHash(tPriKey, tSecKey);
            if (0 != iRet)
            {
                CT_ERROR(("Fail to add idx %d's object into sec hash, ret = %d", 
                    iIdx, iRet));
                return iRet;
            }
        }
        
        //Now append it into the apply list (this item is surely the latest one)
        AddToHeadList(tPriKey);    
        
        return 0;

    }
    
    //删除一条申请表
    int DelObj(const PRIKEY& tPriKey)
    {
        //find its value pair first.
        int iIdx = m_stPriHash.HashFind(tPriKey);
        if (0 > iIdx)
        {
            CT_ERROR(("fail to find value"));
            return -1;
        }

        const CTValPair& stValPair = m_stPriHash[iIdx];
        SECKEY tSecKey = stValPair.stSecKey;

        //delete it from secondary hash and head list
        if (stValPair.bIsHaveSecKey)
        {
            DelFromSecHash(tPriKey, tSecKey);
        }

        DelFromHeadList(tPriKey);
        
        //Delete it from primary hash
        int iRet = m_stPriHash.HashFreeByIndex(iIdx);
        if (0 != iRet)
        {
            CT_ERROR(("Fail to free object, ret = %d", iRet));
            return iRet;
        }
            
        return 0;
    }

    //获取对象表,根据输入的起始位置和目标对象数获取指定的对象
    //In: iStartPos -- the start position to get objs
    //    iTargetObjCnt --- how many objects to be read
    //In&Out:
    //      astObjArray --- returning objects
    //      iTotalObjCnt --- total object count in primary hash
    int GetObjByRange(
        int iStartPos,
        int iTargetObjCnt,
        CTArray<VALUE, EMAX_OBJ_PERPAGE>& astObjArray,
        int& iTotalObjCnt) const
    {
        if (0 > iStartPos || 0 >= iTargetObjCnt || EMAX_OBJ_PERPAGE < iTargetObjCnt)
        {
            CT_WARNING(("Invalid start position %d or target obj cnts %d", 
                iStartPos, iTargetObjCnt));
            return -1;
        }

        //Get primary keys in this range first (iTotalObjCnt is set in GetKeyByRange())
        CTArray<PRIKEY, EMAX_OBJ_PERPAGE> astKeyArray;
        int iRet = GetKeyByRange(iStartPos, iTargetObjCnt, astKeyArray, iTotalObjCnt);
        if (0 != iRet)
        {
            CT_WARNING(("Fail to get pri keys starts at %d, target cnt %d, ret = %d",
                iStartPos, iTargetObjCnt, iRet));
            return iRet;
        }
        
        astObjArray.Clear();           

        //Now copy the objects to the astObjArray one by one
        for (int iIdx = 0; iIdx < astKeyArray.Size(); iIdx ++)
        {
            const PRIKEY& tPriKey = astKeyArray[iIdx];            
            int iObjIdx = m_stPriHash.HashFind(tPriKey);
            if (0 > iObjIdx)
            {
                CT_ERROR(("Object is not found!"));
                return -1;
            }
            astObjArray.AddOneItem(m_stPriHash[iObjIdx].stValue);
        }       

        return 0;
    }

    //获取对象表,根据输入的起始位置和目标对象数获取指定的prikey
    //In: iStartPos -- the start position to get objs
    //    iTargetObjCnt --- how many objects to be read
    //In&Out:
    //      astObjArray --- returning pri keys
    //      iTotalObjCnt --- total object count in primary hash
    int GetKeyByRange(
        int iStartPos,
        int iTargetObjCnt,
        CTArray<PRIKEY, EMAX_OBJ_PERPAGE>& astObjArray,
        int& iTotalObjCnt) const
    {
        if (0 > iStartPos || 0 >= iTargetObjCnt || EMAX_OBJ_PERPAGE < iTargetObjCnt)
        {
            CT_WARNING(("Invalid start position %d or target obj cnts %d", 
                iStartPos, iTargetObjCnt));
            return -1;
        }

        int iHeadListCnt = m_astHeadList.Size();
        if (iHeadListCnt <= iStartPos)
        {
            CT_WARNING(("invalid startPos %d", iStartPos));
            return -1;
        }

        if (iHeadListCnt > m_stPriHash.GetObjectCount())
        {
            CT_ERROR(("algo error: heading list has %d obj but pri hash has %d only",
                iHeadListCnt, m_stPriHash.GetObjectCount()));
            return -1;
        }

        //truncate the target obj cnt if it's beyond the available list
        if (iHeadListCnt < iTargetObjCnt + iStartPos)
        {
            iTargetObjCnt = iHeadListCnt - iStartPos;
        }
        
        astObjArray.Clear();           

        //Now copy the objects to the astObjArray one by one
        for (int iIdx = 0; iIdx < iTargetObjCnt; iIdx ++)
        {             
            astObjArray.AddOneItem(m_astHeadList[iIdx + iStartPos]);
        }

        //retrieve its total cnt from primary hash
        iTotalObjCnt = Size();

        return 0;
    }

    int UpdateObjByPriKey(const PRIKEY& tPriKey, const VALUE& tVal)
    {
        int iIdx = m_stPriHash.HashFind(tPriKey);
        if (0 > iIdx)
        {
            CT_TRACE(("No object for this primary key is found"));
            return -1;
        }
        m_stPriHash[iIdx].stValue = tVal;
        return 0;
    }

    const VALUE* GetObjByPriKey(const PRIKEY& tPriKey)
    {
        int iIdx = m_stPriHash.HashFind(tPriKey);
        if (0 > iIdx)
        {
            CT_TRACE(("No object for this primary key is found"));
            return NULL;
        }

        return &(m_stPriHash[iIdx].stValue);
    }

    //获取与prikey相关联的对象
    int GetObjByPriKey(const PRIKEY& tPriKey, VALUE& tVal) const
    {
        int iIdx = m_stPriHash.HashFind(tPriKey);
        if (0 > iIdx)
        {
            CT_TRACE(("No object for this primary key is found"));
            return -1;
        }
        CTValPair stValPair = m_stPriHash[iIdx];
        tVal = stValPair.stValue;
        return 0;
    }

    //获取与指定seckey相关联的所有对象
    int GetObjsBySecKey(const SECKEY& tSecKey, CTArray<VALUE, SECLISTLEN>& astObjArray) const
    {
        //Clear the return array first
        astObjArray.Clear();

        //get all its mapped primary keys        
        CTArray<PRIKEY, SECLISTLEN> astKeyArray;
        int iRet = GetKeysBySecKey(tSecKey,astKeyArray);
        if (0 != iRet)
        {
            CT_TRACE(("Get prikeys mapped to seckey returns %d", iRet));
            return iRet;
        }

        //copy the value of these primary keys one bye one
        int iPriKeyCnt = astKeyArray.Size();
        for(int iPriKeyIdx = 0; iPriKeyIdx < iPriKeyCnt; iPriKeyIdx++)
        {
            //Get pri key mapped to this sec key
            const PRIKEY& tPriKey = astKeyArray[iPriKeyIdx];            

            //Get val corresponding to this key
            VALUE tVal;
            iRet = GetObjByPriKey(tPriKey, tVal);
            if (0 != iRet)
            {
                CT_ERROR(("fail to get key at index %d, total cnt is %d",
                    iPriKeyIdx, iPriKeyCnt));
                return -1;
            }

            //append it to out array
            astObjArray.AddOneItem(tVal);
        }        

        return 0;
    }

    //获取与指定seckey相关联的所有priKey
    int GetKeysBySecKey(const SECKEY& tSecKey, CTArray<PRIKEY, SECLISTLEN>& astObjArray) const
    {
        //Clear the return array first
        astObjArray.Clear();

        //get all its mapped primary keys       
        int iIdx = m_stSecHash.HashFind(tSecKey);
        if (0 > iIdx)
        {
            CT_TRACE(("No object for this secondary key is found"));
            return -1;
        }
        const CTSecMapList& stSecKeyMapList = m_stSecHash[iIdx];

        //copy these primary keys one bye one
        int iPriKeyCnt = stSecKeyMapList.GetUseCount();
        if (0 < iPriKeyCnt)
        {
            for(int iPriKeyIdx = 0; iPriKeyIdx < iPriKeyCnt; iPriKeyIdx++)
            {
                //Get pri key mapped to this sec key
                PRIKEY tPriKey;
                int iRet = stSecKeyMapList.GetKeyByIdx(iPriKeyIdx, tPriKey);
                if (0 != iRet)
                {
                    CT_ERROR(("fail to get key at index %d, total cnt is %d",
                        iPriKeyIdx, iPriKeyCnt));
                    return -1;
                }                

                //append it to out array
                astObjArray.AddOneItem(tPriKey);
            }
        }

        return 0;
    }

    //获取第一个primary key
    int GetFirstPriKey(PRIKEY& tPriKey) 
    {
        int iIdx = m_stPriHash.GetHeadIndex();
        if (0 > iIdx)
        {
            CT_TRACE(("No object is in this hash yet"));
            return -1;
        }
        
        tPriKey = m_stPriHash.GetKeyByIndex(iIdx);
        return 0;
    }

    //获取下一个primary key
    int GetNextPriKey(const PRIKEY& tPriKey, PRIKEY& tNextKey) 
    {
        int iIdx = m_stPriHash.HashFind(tPriKey);
        if (0 > iIdx)
        {
            CT_TRACE(("the requested key is not in this hash yet"));
            return -1;
        }

        //Find next index of this index, then get key from its index
        iIdx = m_stPriHash.GetNextIndex(iIdx);
        if (0 > iIdx)
        {
            CT_TRACE(("reached the last key!"));
            return -1;
        }
        tNextKey = m_stPriHash.GetKeyByIndex(iIdx);
        
        return 0;
    }

    //获取第一个secondary key
    int GetFirstSecKey(SECKEY& tSecKey)
    {
        int iIdx = m_stSecHash.GetHeadIndex();
        if (0 > iIdx)
        {
            CT_TRACE(("No object is in this hash yet"));
            return -1;
        }
        
        tSecKey = m_stSecHash.GetKeyByIndex(iIdx);
        return 0;
    }

    //获取下一个secondary key
    int GetNextSecKey(const SECKEY& tSecKey, SECKEY& tNextKey)
    {
        int iIdx = m_stSecHash.HashFind(tSecKey);
        if (0 > iIdx)
        {
            CT_TRACE(("the requested key is not in this hash yet"));
            return -1;
        }

        //Find next index of this index, then get key from its index
        iIdx = m_stSecHash.GetNextIndex(iIdx);
        if (0 > iIdx)
        {
            CT_TRACE(("Reached the last key"));
            return -1;
        }
        
        tNextKey = m_stSecHash.GetKeyByIndex(iIdx);
        
        return 0;
    }

    //后去目前跟指定secondary key相关联的primary key 个数
    int GetPriKeyCntBySecKey(const SECKEY& tSecKey)
    {
        int iIdx = m_stSecHash.HashFind(tSecKey);
        if (0 > iIdx)
        {//the sec key has no pri key yet
            return 0;
        }

        //Get used count of primary key array for this sec key
        return m_stSecHash[iIdx].GetUseCount();
    }

    //获取现有的obj数量
    int Size() const
    {
        return m_stPriHash.Size(CTHashObjectMgr<PRIKEY, CTValPair>::USED_LIST);
    }

    //获取总共的obj数量
    int GetCount()
    {
        return m_stPriHash.GetObjectCount();
    }
    
};

#endif

