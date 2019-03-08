/**
* 基于CTObjectMgr的Hash表
*/

#ifndef __CTLIB_HASHOBJECTMGR_H__
#define __CTLIB_HASHOBJECTMGR_H__

#include "CTObjectMgr.h"

#ifndef WIN32
    #include <backward/hash_fun.h>

#else
namespace __gnu_cxx  // 从gcc ext/hash_fun.h抄过来的
{
    template<typename KEY> struct hash {};

    inline size_t __stl_hash_string(const char* s)
    {
        unsigned long ul = 0;
        for ( ; *s; ++s)
        {
            ul = 5 * ul + *s;
        }
        return size_t(ul);
    }

    template<> struct hash<char*>
    {
        size_t operator()(const char* s) const
        {
            return __stl_hash_string(s);
        }
    };

    template<> struct hash<const char*>
    {
        size_t operator()(const char* s) const
        {
            return __stl_hash_string(s);
        }
    };

    template<> struct hash<char>
    {
        size_t operator()(char x) const
        {
            return x;
        }
    };

    template<> struct hash<unsigned char>
    {
        size_t operator()(unsigned char x) const
        {
            return x;
        }
    };

    template<> struct hash<signed char>
    {
        size_t operator()(unsigned char x) const
        {
            return x;
        }
    };

    template<> struct hash<short>
    {
        size_t operator()(short x) const
        {
            return x;
        }
    };

    template<> struct hash<unsigned short>
    {
        size_t operator()(unsigned short x) const
        {
            return x;
        }
    };

    template<> struct hash<int>
    {
        size_t operator()(int x) const
        {
            return x;
        }
    };

    template<> struct hash<unsigned int>
    {
        size_t operator()(unsigned int x) const
        {
            return x;
        }
    };

    template<> struct hash<long>
    {
        size_t operator()(long x) const
        {
            return x;
        }
    };

    template<> struct hash<unsigned long>
    {
        size_t operator()(unsigned long x) const
        {
            return x;
        }
    };

} // namespace __gnu_cxx
#endif // WIN32


namespace CTLib
{
/**
 * Hash元素初始化函数.
*/
template<typename TYPE>
struct CTHashObjInitor
{
    void operator()(TYPE* pstHashObj)
    {
        pstHashObj->~TYPE();
        new (pstHashObj) TYPE;
    }
};

template<>
struct CTHashObjInitor<int>
{
    void operator()(int* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<unsigned int>
{
    void operator()(unsigned int* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<char>
{
    void operator()(char* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<unsigned char>
{
    void operator()(unsigned char* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<short>
{
    void operator()(short* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<unsigned short>
{
    void operator()(unsigned short* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<int64>
{
    void operator()(int64* pstHashObj)
    {
        return;
    }
};

template<>
struct CTHashObjInitor<uint64>
{
    void operator()(uint64* pstHashObj)
    {
        return;
    }
};


/**
* Hash对象管理器.
* 模板参数类似gcc hashmap
*/
template<typename KEY,
         typename VALUE,
         typename HASH_FUNC = __gnu_cxx::hash<KEY>,
         typename EQUAL_KEY = std::equal_to<KEY> >
class CTHashObjectMgr;

template<typename KEY,
         typename VALUE,
         typename HASH_FUNC,
         typename EQUAL_KEY>
class CTHashObjectMgr
{
public:
    typedef class tagPair
    {
    public:
        KEY     stKey;
        VALUE   stValue;

        tagPair()
        {

        }

        ~tagPair()
        {

        }

        OBJ_CHECK_BEGIN(tagPair)
        OBJ_CHECK_END()
    }
    TPair;

    enum
    {
        ALLOC_INDEX = 0,    ///< 用来保存节点是否分配
        FREE_LIST = 0,
        USED_LIST = 1,

        HASH_INDEX = 1,     ///< 用来记录冲突链表
    };
    CTObjectMgr<TPair>  m_stMgr;    ///< 先设为public, 外部可能会用到索引链表操作
    typedef void(*pDumpFunc)(KEY& stKey, VALUE &stData);  ///< 外部用来dump Hash表中内容的函数
    bool m_bNeedConstructValue;

public:

    CTHashObjectMgr()
        :m_bNeedConstructValue(true)
    {

    }

    virtual ~CTHashObjectMgr()
    {

    }
 /*   int Size(int iListID, int iIndexID)
    {
        return m_stMgr.Size(iListID, iIndexID);
    }
*/
    /**
    * 计算需要多少空间.
    */
    static size_t CountSize(int iObjectCount, int iListCount = 2, int iIndexCount = 2)
    {
        return CTObjectMgr<TPair>::CountSize(iObjectCount, sizeof(TPair), iListCount, iIndexCount);
    }

    /**
    * 初始化.
    * @note
    * iIndexID=0 用来保存节点是否分配
    * iIndexID=1 用来记录冲突链表
    */
    virtual int Init(const char* pBuffer, int iObjectCount, int iListCount = 2, int iIndexCount = 2, bool bResetShm = true)
    {
        if (iIndexCount < 2 && iListCount < 2) // 至少要2个索引
        {
            return -1;
        }

        int iRet = m_stMgr.Init(pBuffer, iObjectCount, sizeof(TPair),
            iListCount, iIndexCount, bResetShm);
        if (iRet)
        {
            return iRet;
        }

        if (bResetShm)
        {
            m_stMgr.BuildEachList(HASH_INDEX);
            for (int i = 0; i < iObjectCount; ++i)
            {
                m_stMgr.Flag(i, HASH_INDEX) = -1;
            }
        }

        return 0;
    }

    /**
    * 基于Hash的分配.
    * @param[in] stKey Key
    * @param[in] bFindExist 分配之前检查是否已存在
    * @return 返回的是分配的节点的下标
    */
    virtual int HashAlloc(const KEY& stKey, bool bFindExist = true)
    {
        if (bFindExist)
        {
            int i = HashFind(stKey);
            if (i >= 0) // 已存在就返回以前分配的下标
            {
                return i;
            }
        }

        // 分配的节点的Prev(iNew, HASH_INDEX) 必然是-1
        // Prev(iNew, HASH_INDEX)如果不是-1 表示这个节点以前是某个冲突链的头节点
        int iKey = HASH_FUNC()(stKey);
        iKey &= 0x7FFFFFFF;
        int iIndex = iKey % m_stMgr.GetObjectCount();
        int iRetIndex = iIndex;
        if (m_stMgr.Flag(iIndex, ALLOC_INDEX) == FREE_LIST) // 未分配
        {
            m_stMgr.InfoListMove(iIndex, ALLOC_INDEX, USED_LIST);
        }
        else
        {
            // 已经分配了
            // ALLOC_INDEX链可能有其它LIST，如果已经分配是否要放到USED_LIST中需要考虑 by will
            iRetIndex = m_stMgr.InfoListAlloc(ALLOC_INDEX, USED_LIST);
            if (iRetIndex < 0)  // 分配失败
            {
                return -1;
            }
        }

        if (m_stMgr.Flag(iIndex, HASH_INDEX) == -1)
        {
            m_stMgr.Flag(iIndex, HASH_INDEX) = iRetIndex;
        }
        else  // 后续冲突节点
        {
            int iRet = m_stMgr.EachListAppend(iRetIndex, HASH_INDEX,
                m_stMgr.Flag(iIndex, HASH_INDEX));
            if (iRet) // 调整冲突链条失败
            {
                return -1;
            }
        }

        m_stMgr[iRetIndex].stKey = stKey;
        return iRetIndex;
    }

    /**
    * 释放节点
    * @param[in] i 节点的下标
    * @return 释放成功返回0
    */
    virtual int HashFreeByIndex(int i)
    {
        if (i < 0 || i >= m_stMgr.GetObjectCount())
        {
            return -1;
        }

        if (m_bNeedConstructValue)
        {
            //初始化内存 add by will
            CTHashObjInitor<VALUE>()(&(this->m_stMgr[i].stValue));
        }

        if (m_stMgr.Flag(i, ALLOC_INDEX) == FREE_LIST)  // 状态不对
        {
            return -1;
        }

        int iRet = m_stMgr.InfoListMove(i, ALLOC_INDEX, FREE_LIST);
        if (iRet)
        {
            return iRet;
        }

        if (m_stMgr.Prev(i, HASH_INDEX) == -1) // 是冲突链表的头
        {
            int iKey = HASH_FUNC()(m_stMgr[i].stKey);			
            iKey &= 0x7FFFFFFF;
            int iTempIndex = iKey % m_stMgr.GetObjectCount();
            m_stMgr.Flag(iTempIndex, HASH_INDEX) = m_stMgr.Next(i, HASH_INDEX);
        }

        return m_stMgr.EachListRemove(i, HASH_INDEX);
    }

    /**
    * 释放节点
    * @param[in] stKey 要释放的Key
    * @return 释放成功返回0
    */
    virtual int HashFreeByKey(const KEY& stKey)
    {
        int i = HashFind(stKey);
        return HashFreeByIndex(i);
    }

    /**
    * 按Key进行查找
    * @param[in] piFindCount 返回查找次数
    * @return 找到了就返回对应的下标, 否则就返回-1
    */
    virtual int HashFind(const KEY& stKey, int* piFindCount = NULL)
    {
        int iKey = HASH_FUNC()(stKey);
        iKey &= 0x7FFFFFFF;
        int i = iKey % m_stMgr.GetObjectCount();
        if (m_stMgr.Flag(i, HASH_INDEX) == -1) // 未分配  Flag(i, ALLOC_INDEX) == FREE_LIST &&
        {
            return -1;
        }

        int iFindCount = 0;
        int* piCount = (piFindCount ? piFindCount : &iFindCount);
        *piCount = 0;

        i = m_stMgr.Flag(i, HASH_INDEX);
        while (i >= 0)
        {
            ++(*piCount);
            if (m_stMgr.Flag(i, ALLOC_INDEX) != FREE_LIST &&  EQUAL_KEY()(stKey, m_stMgr[i].stKey))
            {
                //调整一下位置，将最近访问的放在最后
                m_stMgr.InfoListMove(i, ALLOC_INDEX, m_stMgr.Flag(i, ALLOC_INDEX));

                return i;
            }

            i = m_stMgr.Next(i, HASH_INDEX); // 移动下一个冲突节点
        }

        return -1;
    }

public:
    virtual VALUE& operator[] (int i)
    {
        CT_ASSERT(i >= 0 && i < m_stMgr.GetObjectCount());
        return m_stMgr[i].stValue;
    }

    virtual const VALUE& operator[] (int i) const
    {
        CT_ASSERT(i >= 0 && i < m_stMgr.GetObjectCount());
        return m_stMgr[i].stValue;
    }


public:
    int GetObjectCount() const
    {
        return m_stMgr.GetObjectCount();
    }

    int GetListCount() const
    {
        return m_stMgr.GetListCount();
    }

    int GetIndexCount() const
    {
        return m_stMgr.GetIndexCount();
    }

    int Size(int iListID) const
    {
       return m_stMgr.ListInfo(iListID).iSize;
    }

    virtual int GetHeadIndex()
    {
        return m_stMgr.Head(USED_LIST, ALLOC_INDEX);
    }

    virtual int GetNextIndex(int iIndex)
    {
        if (m_stMgr.Flag(iIndex, ALLOC_INDEX) == FREE_LIST)
        {
            return -1;
        }
        return m_stMgr.Next(iIndex, ALLOC_INDEX);
    }

    virtual int GetTailIndex()
    {
        return m_stMgr.Tail(USED_LIST, ALLOC_INDEX);
    }

    virtual int GetPrevIndex(int iIndex)
    {
        if (m_stMgr.Flag(iIndex, ALLOC_INDEX) == FREE_LIST)
        {
            return -1;
        }
        return m_stMgr.Prev(iIndex, ALLOC_INDEX);
    }

    virtual bool CheckIndexUsed(int iIndex)
    {
        if (m_stMgr.Flag(iIndex, ALLOC_INDEX) == FREE_LIST)
        {
            return false;
        }
        return true;
    }

    KEY GetKeyByIndex(int iIndex) const
    {
        return m_stMgr[iIndex].stKey;
    }

    void SwapHashByMaxCount(int iMaxCount)
    {
        if (Size(USED_LIST) < iMaxCount)
        {
            return;
        }

        while (Size(USED_LIST) > iMaxCount)
        {
            HashFreeByIndex(GetHeadIndex());
        }
    }

/**导出hash表所有已分配的节点到指定函数里*/
    int DumpData(pDumpFunc pDf)
    {
        int iRetCounts = 0;
        int iIndex = GetHeadIndex();
        while(iIndex != -1)
        {
            pDf(GetKeyByIndex(iIndex), m_stMgr[iIndex].stValue);
            iIndex = GetNextIndex(iIndex);
            ++iRetCounts;
        }
        return iRetCounts;
    }
};

template<typename KEY,
typename VALUE,
typename HASH_FUNC = __gnu_cxx::hash<KEY>,
typename EQUAL_KEY = std::equal_to<KEY> >
class CTAutoHashObjectMgr;

template<typename KEY,
    typename VALUE,
    typename HASH_FUNC,
    typename EQUAL_KEY>
class CTHashPtr
{
public:
    typedef CTAutoHashObjectMgr<KEY, VALUE, HASH_FUNC, EQUAL_KEY> HASH_TYPE;

    CTHashPtr()
        :m_pstHash(NULL), m_iIndex(-1)
    {

    }
    CTHashPtr(HASH_TYPE* pstHash, int iIndex)
        :m_pstHash(pstHash), m_iIndex(iIndex)
    {
        if (!IsValid())
        {
            return;
        }
        AddPtr();
    }

    CTHashPtr(const CTHashPtr& ptr)
        :m_pstHash(ptr.m_pstHash), m_iIndex(ptr.m_iIndex)
    {
        if (!IsValid())
        {
            return;
        }
        AddPtr();
    }

    CTHashPtr& operator= (const CTHashPtr& ptr)
    {
        if (ptr.m_pstHash == m_pstHash && m_iIndex == ptr.m_iIndex)
        {
            return *this;
        }

        DeletePtr();
        m_pstHash = ptr.m_pstHash;
        m_iIndex = ptr.m_iIndex;
        AddPtr();
        return *this;
    }

    ~CTHashPtr()
    {
        if (IsValid())
        {
            DeletePtr();
        }
    }

    VALUE* operator() ()
    {
        if (!IsValid())
        {
            return NULL;
        }
        return &(*m_pstHash)[m_iIndex];
    }

    bool IsValid() const
    {
        return (m_pstHash && m_iIndex >= 0 && m_iIndex <= m_pstHash->GetObjectCount() && m_pstHash->HasUsed(m_iIndex));
    }

    //由于HashPtr保存了hash对象的指针，如果hash对象先销毁，需要执行该函数
    void UnInit()
    {
        m_pstHash = NULL;
    }

    int SetUpdate()
    {
        if (!IsValid())
        {
            return -1;
        }

        return m_pstHash->SetUpdate(m_iIndex);
    }

    int SetNormal()
    {
        if (!IsValid())
        {
            return -1;
        }

        return m_pstHash->SetNormal(m_iIndex);
    }

    int SetSwaping()
    {
        if (!IsValid())
        {
            return -1;
        }

        return m_pstHash->SetSwaping(m_iIndex);
    }

    int Free()
    {
        if (IsValid())
        {
            //m_pstHash->SetNormal(m_iIndex); //注意：只要不存在其他智能指针，不管其状态如何都会被free掉
            return m_pstHash->HashFreeByPtr(*this);
        }

        return 0;
    }

    int GetIndex() const { return m_iIndex;}

private:
    HASH_TYPE* m_pstHash;
    int m_iIndex;

    void DeletePtr()
    {
        if (IsValid())
        {
            m_pstHash->AccessCount(m_iIndex)--;
            if (m_pstHash->AccessCount(m_iIndex) == -1 && m_pstHash->HasUsed(m_iIndex))
            {
                Free();
            }
        }
    }

    void AddPtr()
    {
        if (IsValid())
        {
            m_pstHash->AccessCount(m_iIndex) += 1;
        }
    }
};


template<typename KEY,
        typename VALUE,
        typename HASH_FUNC,
        typename EQUAL_KEY>
class CTAutoHashObjectMgr : public CTHashObjectMgr<KEY, VALUE, HASH_FUNC, EQUAL_KEY>
{
public:
    CTAutoHashObjectMgr()
    {

    }

    virtual ~CTAutoHashObjectMgr()
    {

    }

    enum
    {
           COUNT_INDEX = 2,    ///< 用来记录引用计数链表
           STAT_INDEX = 3,     ///< 用来保存状态的链表

           NORTMAL_LIST = 0,
           UPDATE_LIST = 1,   ///< 需更新的
           SWAPING_LIST = 2,  ///< 正在换出的
    };
    friend class CTHashPtr<KEY, VALUE, HASH_FUNC, EQUAL_KEY>;
    typedef CTHashPtr<KEY, VALUE, HASH_FUNC, EQUAL_KEY> HASH_PTR;
    typedef CTHashObjectMgr<KEY, VALUE, HASH_FUNC, EQUAL_KEY> HASH_TYPE;

    /**
    * 计算需要多少空间.
    */
    static size_t CountSize(int iObjectCount, int iListCount = 3, int iIndexCount = 4)
    {
        return HASH_TYPE::CountSize(iObjectCount, iListCount, iIndexCount) + sizeof(bool) * iObjectCount;
    }

    virtual int Init(const char* pBuffer, int iObjectCount, int iListCount = 3, int iIndexCount = 4, bool bResetShm = true)
    {
        if (iListCount < 3 || iIndexCount < 4)
        {
            return -1;
        }

        int iRet = HASH_TYPE::Init(pBuffer, iObjectCount, iListCount, iIndexCount, bResetShm);
        if (iRet)
        {
            return iRet;
        }

        m_arbNeedReUpdate = (bool*)(pBuffer + HASH_TYPE::CountSize(iObjectCount, iListCount, iIndexCount));

        if (bResetShm)
        {
            this->m_stMgr.BuildEachList(COUNT_INDEX);
            for (int i = 0; i < iObjectCount; ++i)
            {
                AccessCount(i) = -1;
            }
            memset(m_arbNeedReUpdate, 0x0, sizeof(bool) * iObjectCount);
        }

        return iRet;
    }

    virtual HASH_PTR HashNewAlloc(const KEY& stKey, bool bFindExist = true)
    {
        HASH_PTR stPtr = GetPtr(stKey);
        if (stPtr.IsValid())
        {
            return stPtr;
        }

        int iRetIndex = HASH_TYPE::HashAlloc(stKey, bFindExist);
        if (iRetIndex>=0)
        {
            if (IsSwaping(iRetIndex) || IsUpdate(iRetIndex))  //为了防止覆盖
            {
                return HASH_PTR();
            }

            //修改引用计数(对于重复调用HashNewAlloc的情况来说此处会有风险)
            AccessCount(iRetIndex) = 0;
            m_arbNeedReUpdate[iRetIndex] = false;
            return HASH_PTR(this, iRetIndex);
        }
        return HASH_PTR();
    }

    virtual int HashFreeByPtr(HASH_PTR& stHashPtr)
    {
        if (!stHashPtr.IsValid())
        {
            return -1;
        }

        int i = stHashPtr.GetIndex();
        if (this->m_stMgr.Flag(i, HASH_TYPE::ALLOC_INDEX) == HASH_TYPE::FREE_LIST) //未被分配
        {
            return -2;
        }

        stHashPtr = HASH_PTR();
        int &iCount = AccessCount(i);
        if (iCount > 0)
        {
            iCount--;
            return -3;
        }

        m_arbNeedReUpdate[i] = false;
        SetNormal(i);
        AccessCount(i) = 0;
        return HASH_TYPE::HashFreeByIndex(i);
    }

    virtual int HashFreeByIndex(int i)
    {
        HASH_PTR stPtr = HASH_PTR(this, i);
        return HashFreeByPtr(stPtr);
    }

    virtual int HashFreeByKey(const KEY& stKey)
    {
        HASH_PTR stPtr = GetPtr(stKey);
        if (!stPtr.IsValid())
        {
            return -1;
        }
        return HashFreeByPtr(stPtr);
    }

    HASH_PTR GetPtr(const KEY& stKey)
    {
        int iIndex = this->HashFind(stKey);
        if (iIndex < 0 )
        {
            return HASH_PTR();
        }

        SetAccess(iIndex);
        return HASH_PTR(this, iIndex);
    }

    HASH_PTR GetHeadPtr(int iListID = HASH_TYPE::USED_LIST, int iIndex = 0)
    {
        if (iIndex < 0 || iIndex >= this->m_stMgr.GetIndexCount() || iListID < 0 || iListID >= this->m_stMgr.GetListCount())
        {
            return HASH_PTR();
        }

        return HASH_PTR(this, this->m_stMgr.Head(iListID, iIndex));
    }

    HASH_PTR GetNextPtr(HASH_PTR& stPtr, int iIndex = HASH_TYPE::ALLOC_INDEX)
    {
        if (iIndex < 0 || iIndex >= this->m_stMgr.GetIndexCount() || !stPtr.IsValid())
        {
            return HASH_PTR();
        }

        return HASH_PTR(this, this->m_stMgr.Next(stPtr.GetIndex(), iIndex));
    }

    HASH_PTR GetTailPtr(int iListID = HASH_TYPE::USED_LIST, int iIndex = 0)
    {
        if (iIndex < 0 || iIndex >= this->m_stMgr.GetIndexCount() || iListID < 0 || iListID >= this->m_stMgr.GetListCount())
        {
            return HASH_PTR();
        }

        return HASH_PTR(this, this->m_stMgr.Tail(iListID, iIndex));
    }

    HASH_PTR GetPrevPtr(HASH_PTR& stPtr, int iIndex = HASH_TYPE::ALLOC_INDEX)
    {
        if (iIndex < 0 || iIndex >= this->m_stMgr.GetIndexCount() || !stPtr.IsValid())
        {
            return HASH_PTR();
        }

        return HASH_PTR(this, this->m_stMgr.Prev(stPtr.GetIndex(), iIndex));
    }


    HASH_PTR GetUpdateHeadPtr()
    {
        return GetHeadPtr(UPDATE_LIST, STAT_INDEX);
    }

    HASH_PTR GetUpdateNextPtr(HASH_PTR& stPtr)
    {
        return GetNextPtr(stPtr, STAT_INDEX);
    }

    HASH_PTR GetSwapingHeadPtr()
    {
        return GetHeadPtr(SWAPING_LIST, STAT_INDEX);
    }

    HASH_PTR GetSwapingNextPtr(HASH_PTR& stPtr)
    {
        return GetNextPtr(stPtr, STAT_INDEX);
    }

    bool HasUsed(int iIndex)
    {
        return this->m_stMgr.Flag(iIndex, HASH_TYPE::ALLOC_INDEX) != HASH_TYPE::FREE_LIST;
    }

    int& AccessCount(int iIndex)
    {
        return this->m_stMgr.Flag(iIndex, COUNT_INDEX);
    }

    virtual int SetAccess(int iIndex)
    {
        if (!HasUsed(iIndex))
        {
            return -1;
        }

        //队列的靠前的元素会先被唤出
        this->m_stMgr.InfoListMove(iIndex, HASH_TYPE::ALLOC_INDEX, this->m_stMgr.Flag(iIndex, HASH_TYPE::ALLOC_INDEX));
        //更新列表，不做顺序改变
        //this->m_stMgr.InfoListMove(iIndex, STAT_INDEX, this->m_stMgr.Flag(iIndex, STAT_INDEX));
        return 0;
    }

    int SetSwaping(int iIndex) { return this->m_stMgr.InfoListMove(iIndex, STAT_INDEX, SWAPING_LIST); }
    int SetNormal(int iIndex)
    {
        if (IsUpdate(iIndex) && m_arbNeedReUpdate[iIndex] == true)  //需再次更新，此时不修改状态
        {
            m_arbNeedReUpdate[iIndex] = false;
            return 0;
        }
        return this->m_stMgr.InfoListMove(iIndex, STAT_INDEX, NORTMAL_LIST);
    }

    int SetUpdate(int iIndex)
    {
        if (IsUpdate(iIndex))
        {
            m_arbNeedReUpdate[iIndex] = true;
            return 0;
        }
        return this->m_stMgr.InfoListMove(iIndex, STAT_INDEX, UPDATE_LIST);
    }

    int SetSwaping(HASH_PTR& stHashPtr) { return SetSwaping(stHashPtr.GetIndex()); }
    virtual int SetNormal(HASH_PTR& stHashPtr) { return SetNormal(stHashPtr.GetIndex()); }
    virtual int SetUpdate(HASH_PTR& stHashPtr) { return SetUpdate(stHashPtr.GetIndex()); }

    bool IsNormal(int iIndex) { return this->m_stMgr.Flag(iIndex, STAT_INDEX) == NORTMAL_LIST; }
    bool IsSwaping(int iIndex) { return this->m_stMgr.Flag(iIndex, STAT_INDEX) == SWAPING_LIST; }
    bool IsUpdate(int iIndex) { return this->m_stMgr.Flag(iIndex, STAT_INDEX) == UPDATE_LIST; }

    int Size(int iListID, int iIndexID = HASH_TYPE::ALLOC_INDEX)
    {
        return this->m_stMgr.Size(iListID, iIndexID);
    }

    int UsedSize()
    {
        return Size(HASH_TYPE::USED_LIST, HASH_TYPE::ALLOC_INDEX);
    }

    int UpdateSize()
    {
        return Size(UPDATE_LIST, STAT_INDEX);
    }

    int SwapingSize()
    {
        return Size(SWAPING_LIST, STAT_INDEX);
    }

private:
    //屏蔽接口
    virtual VALUE& operator[] (int i)
    {
        return this->m_stMgr[i].stValue;
    }

    virtual const VALUE& operator[] (int i) const
    {
        CT_ASSERT(i >= 0 && i < this->m_stMgr.GetObjectCount());
        return this->m_stMgr[i].stValue;
    }

    virtual int HashFind(const KEY& stKey, int* piFindCount = NULL) const
    {
        return CTHashObjectMgr<KEY, VALUE, HASH_FUNC, EQUAL_KEY>::HashFind(stKey, piFindCount);
    }

    virtual int HashAlloc(const KEY& stKey, bool bFindExist = true)
    {
        return CTHashObjectMgr<KEY, VALUE, HASH_FUNC, EQUAL_KEY>::HashAlloc(stKey, bFindExist);
    }
private:
    bool* m_arbNeedReUpdate;  //需重新更新的状态位，为了避免重复更新引起数据不同步的问题
};

//hash换出控制器
template<typename KEY,
    typename VALUE,
    typename HASH_FUNC = __gnu_cxx::hash<KEY>,
    typename EQUAL_KEY = std::equal_to<KEY> >
class CTAutoHashSwapMgr
{
public:
    typedef CTHashPtr<KEY, VALUE, HASH_FUNC, EQUAL_KEY> HASH_PTR;
    typedef CTAutoHashObjectMgr<KEY, VALUE, HASH_FUNC, EQUAL_KEY> HASH_TYPE;
    typedef int (*UPDATE_FUNC)(HASH_PTR& stPtr);

    CTAutoHashSwapMgr()
        :m_pstHash(NULL), m_fLimitPercent(0), MAX_SWAP_OUT_PER_TIMER(300), MAX_UPDATE_PER_TIMER(300)
    {

    }

    virtual ~CTAutoHashSwapMgr()
    {

    }

    virtual int Init(HASH_TYPE* pstHash, float fLimitPercent, UPDATE_FUNC pUpdateFunc)
    {
        if (pstHash == NULL)
        {
            return -1;
        }

        if (pUpdateFunc == NULL)
        {
            return -2;
        }

        m_pstHash = pstHash;
        m_fLimitPercent = fLimitPercent;
        m_pUpdateFunc = pUpdateFunc;
        return 0;
    }

    //换出检查
    virtual int CheckSwap()
    {
        if (!m_pstHash)
        {
            return -1;
        }

        if (!IsNeedSwap())
        {
            return 0;
        }

        CT_TRACE(("begin swap out"));
        int iRet = 0;
        HASH_PTR stNowPtr, stNextPtr;
        stNowPtr = m_pstHash->GetHeadPtr();
        stNextPtr = m_pstHash->GetNextPtr(stNowPtr);
        for (int i = 0; stNowPtr.IsValid() && IsNeedSwap() && i < MAX_SWAP_OUT_PER_TIMER;i++)
        {
            if (m_pstHash->IsNormal(stNowPtr.GetIndex()) || m_pstHash->IsSwaping(stNowPtr.GetIndex()))  //swaping状态的元素会被重复更新，外层需做好访问频率限制
            {
                iRet = (*m_pUpdateFunc)(stNowPtr);
                if (iRet == 0)
                {
                    m_pstHash->SetSwaping(stNowPtr);
                }
            }

            stNowPtr = stNextPtr;
            stNextPtr = m_pstHash->GetNextPtr(stNowPtr);
        }

        return 0;
    }

    //全部换出
    virtual int SwapAllOut()
    {
        if (!m_pstHash)
        {
            return -1;
        }
        CT_TRACE(("begin swap all out"));

        HASH_PTR stNowPtr, stNextPtr;
        int iRet = 0;
        stNowPtr = m_pstHash->GetHeadPtr();
        stNextPtr = m_pstHash->GetNextPtr(stNowPtr);
        for (int i = 0; stNowPtr.IsValid() && i < MAX_SWAP_OUT_PER_TIMER;i++)
        {
            if (m_pstHash->IsNormal(stNowPtr.GetIndex()) || m_pstHash->IsUpdate(stNowPtr.GetIndex()))
            {
                iRet = (*m_pUpdateFunc)(stNowPtr);
                if (iRet == 0)
                {
                    m_pstHash->SetSwaping(stNowPtr);
                }
            }

            stNowPtr = stNextPtr;
            stNextPtr = m_pstHash->GetNextPtr(stNowPtr);
        }
        return 0;
    }

    //释放
    virtual int SwapOut(HASH_PTR& stPtr)
    {
        if (!m_pstHash)
        {
            return -1;
        }

        if (!stPtr.IsValid())
        {
            return -2;
        }

        if (!m_pstHash->IsSwaping(stPtr.GetIndex()))
        {
            return -3;
        }

        return m_pstHash->HashFreeByPtr(stPtr);
    }

    //检查更新
    virtual int CheckUpdate(int iUpdateInterval)
    {
        static int iLastUpdateTime;
        int iNow = time(NULL);
        if (iNow - iLastUpdateTime < iUpdateInterval)
        {
            return 0;
        }
        iLastUpdateTime = iNow;

        int iRet = 0;
        int iUpdateCount = 0;
        HASH_PTR stNowPtr, stNextPtr;
        stNowPtr = this->m_pstHash->GetUpdateHeadPtr();
        stNextPtr = this->m_pstHash->GetUpdateNextPtr(stNowPtr);
        for (; stNowPtr.IsValid() && iUpdateCount < MAX_UPDATE_PER_TIMER;)
        {
            if (this->m_pstHash->IsUpdate(stNowPtr.GetIndex()))
            {
                CT_TRACE(("update to db ,index = %d", stNowPtr.GetIndex()));
                iRet = (*this->m_pUpdateFunc)(stNowPtr);
                if (iRet == 0)
                {
                    iUpdateCount++;
                }
            }

            stNowPtr = stNextPtr;
            stNextPtr = this->m_pstHash->GetUpdateNextPtr(stNowPtr);
        }

        CT_TRACE(("update %d cell to db", iUpdateCount));
        return 0;
    }

    virtual int SwapingSize()
    {
        return m_pstHash->SwapingSize();
    }

    virtual int UsedSize()
    {
        return m_pstHash->UsedSize();
    }

protected:
    virtual bool IsNeedSwap()
    {
        return (((float)UsedSize() - SwapingSize()) / m_pstHash->GetObjectCount() >= m_fLimitPercent);
    }

    HASH_TYPE* m_pstHash;
    float m_fLimitPercent;      //换出上限比例
    UPDATE_FUNC m_pUpdateFunc;

    const int MAX_SWAP_OUT_PER_TIMER; //一次换出的元素数量
    const int MAX_UPDATE_PER_TIMER;

    CTAutoHashSwapMgr& operator= (const CTAutoHashSwapMgr&)
    {
        return *this;
    }
};

} // namespace CTLib

#endif //__CTLIB_HASHOBJECTMGR_H__
