/**
* 多级ObjectMgr
*/

#ifndef __CTLIB_MULTIOBJECTMGR_H__
#define __CTLIB_MULTIOBJECTMGR_H__

#include "CTBase.h"
#include "CTObjectMgr.h"
#include "CTObjReloadCheck.h"

namespace CTLib
{

/**
* 多级ObjectMgr的下标信息
*/
class CTMgrIndex
{
public:
    int iMgrIndex;  ///< 在CTMultiObjectMgr中的哪个Mgr
    int iObjIndex;  ///< 在Mgr中的下标

    CTMgrIndex()
    {
        OBJ_CHECK_RELOAD();
        iMgrIndex = -1;
        iObjIndex = -1;
    }

    CTMgrIndex(const CTMgrIndex& obj)
    {
        iMgrIndex = obj.iMgrIndex;
        iObjIndex = obj.iObjIndex;
    }

    CTMgrIndex& operator = (const CTMgrIndex& obj)
    {
        iMgrIndex = obj.iMgrIndex;
        iObjIndex = obj.iObjIndex;
        return *this;
    }

    bool operator == (const CTMgrIndex& obj)
    {
        return iMgrIndex == obj.iMgrIndex &&
               iObjIndex == obj.iObjIndex;
    }

    bool operator != (const CTMgrIndex& obj)
    {
        return ! operator == (obj);
    }

    void Clear()
    {
        iMgrIndex = -1;
        iObjIndex = -1;
    }

    bool IsNull() const
    {
        return (iMgrIndex == -1 || iObjIndex == -1);
    }
};

class CTMgrConfig
{
public:
    int iObjectCount;
    int iObjectSize;

    bool operator < (const CTMgrConfig& obj) const
    {
        return iObjectSize < obj.iObjectSize;
    }
};

/**
* 多级ObjectMgr
*/
class CTMultiObjectMgr
{
protected:
    typedef CTObjectMgr<char*>  TObjectMgr;
    typedef vector<TObjectMgr>  TObjectMgrVec;

    typedef vector<CTMgrConfig>  TMgrConfigVec;
    TObjectMgrVec    m_astObjectMgr;

public:
    CTMultiObjectMgr()
    {
    }

    virtual ~CTMultiObjectMgr()
    {
    }

    /**
    * 初始化
    * @param[in] pszMgrConfig 不同大小的内存块和数量\n
    *   字符串格式是 (内存块大小1,内存块数量1)...(内存块大小n,内存块数量n)
    * @param[in] pszBuffer 内存首地址(内存是外部分配的)
    * @param[in] iBufferSize 内存的大小，必须要等于CountSize()计算出来的大小
    * @return 0=成功
    */
    int Init(const char* pszMgrConfig, const char* pszBuffer, int iBufferSize,
        int iListCount = 2, int iIndexCount = 1,bool bReload = true)
    {
        int iRet = 0;

        TMgrConfigVec ast;
        iRet = BuildMgrConfigVec(pszMgrConfig, ast);
        if (iRet)
        {
            return iRet;
        }

        return Init(ast, pszBuffer, iBufferSize, iListCount, iIndexCount, bReload);
    }

    /**
    * 计算astMgrConfig需要的内存块大小，方便申请内存/共享内存
    * @param[in] pszMgrConfig 不同大小的内存块和数量
    * @return 返回所需内存的总大小，单位字节
    */
    static size_t CountSize(const char* pszMgrConfig, int iListCount = 2, int iIndexCount = 1)
    {
        int iRet = 0;

        TMgrConfigVec ast;
        iRet = BuildMgrConfigVec(pszMgrConfig, ast);
        if (iRet)
        {
            return iRet;
        }

        return CountSize(ast, iListCount, iIndexCount);
    }

    /**
    * 如果要分配缓冲区, 应该从哪个Mgr中分配.
    * 计算方法是从开始遍历，找到最先满足大小的Mgr, 后续可以优化
    * @return 返回Mgr的下标, 如果都不满足就返回-1
    */
    int CalcMgrIndex(int iSize) const
    {
        for (size_t i = 0; i < m_astObjectMgr.size(); ++i)
        {
            if (m_astObjectMgr[i].GetObjectSize() >= iSize)
            {
                return i;
            }
        }

        return -1;
    }

    /**
    * 分配缓冲区.
    * 需要在外面placement new
    * @param[in] iSize 要分配的缓冲区大小
    * @param[out] pstIndex 分配缓冲区的下标信息
    */
    char* Alloc(int iMgrIndex, CTMgrIndex* pstIndex)
    {
        if (pstIndex)
        {
            pstIndex->Clear();
        }

        if (iMgrIndex < 0)
        {
            return NULL;
        }

        // 分配
        TObjectMgr& mgr = m_astObjectMgr[iMgrIndex];
        int iIndex = mgr.InfoListAlloc(0, 1);
        if (iIndex < 0)
        {
            CT_WARNING(("Mgr[%d].Alloc failed %d", iMgrIndex, iIndex));
            return NULL;
        }

        if (pstIndex)
        {
            pstIndex->iMgrIndex = iMgrIndex;
            pstIndex->iObjIndex = iIndex;
        }

        return m_astObjectMgr[iMgrIndex][iIndex];
    }

    /**
    * 判断缓冲区是不是从这里分配的
    * @param[in] pObject 要检测的缓冲区
    * @param[out] pstIndex 保存缓冲区的下标信息
    */
    bool IsValidObject(const char* pObject, CTMgrIndex* pstIndex) const
    {
        if (!pObject)
        {
            return false;
        }

        for (size_t i = 0; i < m_astObjectMgr.size(); ++i)
        {
            const TObjectMgr& stMgr = m_astObjectMgr[i];
            if (pObject >= stMgr[0] && pObject <= stMgr[stMgr.GetObjectCount() - 1])
            {
                if ((pObject - stMgr[0]) % stMgr.GetObjectSize() == 0)
                {
                    if (pstIndex)
                    {
                        pstIndex->iMgrIndex = i;
                        pstIndex->iObjIndex = (pObject - stMgr[0]) / stMgr.GetObjectSize();
                    }

                    return true;
                }
                else
                {
                    return false;
                }
            }
        }

        return false;
    }

    /**
    * 判断下标信息是不是从这里分配的
    * @param[in] stIndex 要检测的下标信息
    */
    bool IsValidIndex(const CTMgrIndex& stIndex) const
    {
        if (stIndex.iMgrIndex < 0 || stIndex.iMgrIndex >= (int)m_astObjectMgr.size() ||
            stIndex.iObjIndex < 0 || stIndex.iObjIndex >= m_astObjectMgr[stIndex.iMgrIndex].GetObjectCount())
        {
            return false;
        }

        return true;
    }

    /**
    * 释放缓冲区.
    * 需要在外面调用placement new出来的对象的析构函数
    */
    int Free(const char* pObject)
    {
        CTMgrIndex stIndex;
        if (!IsValidObject(pObject, &stIndex))
        {
            return -1;
        }

        return Free(stIndex);
    }

    /**
    * 释放缓冲区.
    * 以分配缓冲区时的下标信息做参数效率更高
    * 需要在外面调用placement new出来的对象的析构函数
    */
    int Free(const CTMgrIndex& stIndex)
    {
        if (!IsValidIndex(stIndex))
        {
            return -1;
        }

        m_astObjectMgr[stIndex.iMgrIndex].InfoListMove(stIndex.iObjIndex, 0, 0);
        return 0;
    }

    /**
    * 返回下标信息对应的缓冲区
    */
    char* Get(const CTMgrIndex& stIndex)
    {
        return m_astObjectMgr[stIndex.iMgrIndex][stIndex.iObjIndex];
    }

    size_t GetMgrCount() const
    {
        return m_astObjectMgr.size();
    }

    int GetMgrObjectSize(int iMgrIndex) const
    {
        return m_astObjectMgr[iMgrIndex].GetObjectSize();
    }

    int GetMgrObjectCount(int iMgrIndex) const
    {
        return m_astObjectMgr[iMgrIndex].GetObjectCount();
    }

	int Size(int iMrgIndex, int iIndex, int iList)
	{
		return m_astObjectMgr[iMrgIndex].Size(iList, iIndex);
	}

    /**
    * 获取多级ObjectMgr某条链条的头部
    * @param[out] stIndex 头部的下标信息, 如果链表为空, 则stIndex.IsNull
    */
    void GetHead(CTMgrIndex& stIndex, int iListID, int iIndexID) const
    {
        stIndex.Clear();
        for (size_t i = 0; i < m_astObjectMgr.size(); ++i)
        {
            stIndex.iMgrIndex = i;
            stIndex.iObjIndex = m_astObjectMgr[i].Head(iListID, iIndexID);
            if (!stIndex.IsNull())
            {
                return;
            }
        }
    }

    /**
    * 获取多级ObjectMgr某条链条的下一个节点
    */
    void GetNext(const CTMgrIndex& stIndex, CTMgrIndex& stNext, int iIndexID) const
    {
        stNext.iMgrIndex = stIndex.iMgrIndex;
        stNext.iObjIndex = m_astObjectMgr[stIndex.iMgrIndex].Next(stIndex.iObjIndex, iIndexID);
        if (!stNext.IsNull())
        {
            return;
        }
        
        // 在这个级别的ObjectMgr找不到下一个节点了, 就在下个级别的ObjectMgr找
        int iListID = m_astObjectMgr[stIndex.iMgrIndex].Flag(stIndex.iObjIndex, iIndexID);
        for (size_t i = stIndex.iMgrIndex + 1; i < m_astObjectMgr.size(); ++i)
        {
            stNext.iMgrIndex = i;
            stNext.iObjIndex = m_astObjectMgr[i].Head(iListID, iIndexID);
            if (!stNext.IsNull())
            {
                return;
            }
        }
    }

    /**
    * 把节点移到另一个链表
    */
    int Move(const CTMgrIndex& stIndex, int iIndexID, int iNewListID)
    {
        TObjectMgr& stMgr = m_astObjectMgr[stIndex.iMgrIndex];
        return stMgr.InfoListMove(stIndex.iObjIndex, iIndexID, iNewListID);
    }

protected:
    /**
    * 初始化
    * @param[in] astMgrConfig 不同大小的内存块和数量
    * @param[in] pszBuffer 内存首地址(内存是外部分配的)
    * @param[in] iBufferSize 内存的大小，必须要等于CountSize()计算出来的大小
    * @return 0=成功
    */
    int Init(const TMgrConfigVec& astMgrConfig, const char* pszBuffer,
        size_t iBufferSize, int iListCount = 2, int iIndexCount = 1,bool bReset = true)
    {
        const size_t iAllSize = CountSize(astMgrConfig, iListCount, iIndexCount);
        if (iAllSize != iBufferSize || iAllSize <= 0)
        {
            return -1;
        }

        size_t iPos = 0;
        int iRet = 0;
        for (size_t i = 0; i < astMgrConfig.size(); ++i)
        {
            m_astObjectMgr.push_back(TObjectMgr());
            iRet = m_astObjectMgr[i].Init(pszBuffer + iPos, astMgrConfig[i].iObjectCount,
                astMgrConfig[i].iObjectSize, iListCount, iIndexCount,bReset);
            if (iRet)
            {
                return iRet;
            }

            iPos += TObjectMgr::CountSize(astMgrConfig[i].iObjectCount,
                astMgrConfig[i].iObjectSize, iListCount, iIndexCount);
        }

        return 0;
    }

    /**
    * 计算astMgrConfig需要的内存块大小，方便申请内存/共享内存
    * @param[in] astMgrConfig 不同大小的内存块和数量
    * @return 返回所需内存的总大小，单位字节
    */
    static size_t CountSize(const TMgrConfigVec& astMgrConfig, 
        int iListCount = 2, int iIndexCount = 1)
    {
        int iAllSize = 0;
        for (size_t i = 0; i < astMgrConfig.size(); ++i)
        {
            iAllSize += TObjectMgr::CountSize(astMgrConfig[i].iObjectCount,
                astMgrConfig[i].iObjectSize, iListCount, iIndexCount);
        }

        return iAllSize;
    }
public:
    /**
    * 从字符串解析出astMgrConfig.
    * 字符串格式是 (内存块大小1,内存块数量1)...(内存块大小n,内存块数量n)\n
    * 内存块大小的单位是字节
    */
    static int BuildMgrConfigVec(const char* pszMgrConfig, TMgrConfigVec& astMgrConfig)
    {
        astMgrConfig.clear();

        string s(pszMgrConfig);
        CTStringUtils::RemoveNot(s, "01234567890,)");

        vector<string> astKeyValue;
        CTStringUtils::Split(s, ")", astKeyValue);

        string sSize, sCount;
        for (size_t i = 0; i < astKeyValue.size(); ++i)
        {
            if (!CTStringUtils::SplitIni(astKeyValue[i], sSize, sCount, ','))
            {
                astMgrConfig.clear();
                return -1;
            }

            CTMgrConfig stConfig;
            stConfig.iObjectSize = CTStringUtils::StrToInt<int>(sSize.c_str());
            stConfig.iObjectCount = CTStringUtils::StrToInt<int>(sCount.c_str());
            if (!(stConfig.iObjectSize > 0 && stConfig.iObjectCount > 0))
            {
                astMgrConfig.clear();
                return -2;
            }

            astMgrConfig.push_back(stConfig);
        }

        // 排序
        sort(astMgrConfig.begin(), astMgrConfig.end());
        return 0;
    }
};

} // namespace CTLib

#endif //__CTLIB_MULTIOBJECTMGR_H__


