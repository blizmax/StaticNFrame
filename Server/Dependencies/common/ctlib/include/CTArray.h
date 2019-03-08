/**
* 静态数组
*/

#ifndef __CTLIB_ARRAY_H__
#define __CTLIB_ARRAY_H__

#include "CTObjReloadCheck.h"
#include "base/CTAssert.h"

#ifndef WIN32
    #include <execinfo.h>
#endif


namespace CTLib
{

/**
* 静态数组
*/
template<typename T, int COUNT>
class CTArray
{
public:
    int m_iUsedCount;
    T   m_astData[COUNT];

public:
    CTArray()
    {
        OBJ_CHECK_RELOAD();
        m_iUsedCount = 0;
    }

	/*检查下标的合法性，返回合法的下标值,用于[]函数中*/
	int CheckIndex(int i) const
	{
		int iIndex = 0;
		if (i < 0)
		{
			iIndex = 0;
		}
		else
		{/*不能假设m_iUsedCount不会超过COUNT*/
			/*以前的逻辑下，i会超过m_iUsedCount*/
			/*int iMaxIndex = m_iUsedCount > COUNT ? (COUNT - 1) : (m_iUsedCount - 1);
			if (iMaxIndex < 0)
			{
				iMaxIndex = 0;
			}
			iIndex = i > iMaxIndex ? iMaxIndex : i;
			*/

			int iMaxIndex = COUNT - 1;
			iIndex = i > iMaxIndex ? iMaxIndex : i;
		}
		return iIndex;
	}

    const T& operator[] (int i) const
    {
        CT_ASSERT(i < COUNT && i >= 0);

		int iIndex = CheckIndex(i); //绝对信任CheckIndex的返回值;
		
		assert(iIndex>=0 && iIndex < COUNT);

        return m_astData[iIndex];
    }

    T& operator[] (int i)
    {
        CT_ASSERT(i < COUNT && i >= 0); 

		int iIndex = CheckIndex(i);//绝对信任CheckIndex的返回值;

        return m_astData[iIndex];
    }
   
    int GetCount() const
    {
        return COUNT;
    }

    CTArray<T, COUNT>& operator = (const CTArray<T, COUNT>& obj)
    {
        m_iUsedCount = obj.m_iUsedCount;
        if (obj.m_iUsedCount > COUNT)
        {
            CT_ERROR(("UseCount(%d)!", obj.m_iUsedCount));
            m_iUsedCount = COUNT;
#ifndef WIN32
        void* array[20];
        int iCount = backtrace(array, CT_COUNTOF(array));
        char** symbols = backtrace_symbols(array, iCount);
        CTAutoFree autop(symbols);
        string sBackTrace = "StackTrace:\n";
        for (int i = 1; i < iCount; ++i)  // 第1个是DumpBackTrace, 忽略
        {
            sBackTrace += "    ";
            sBackTrace += symbols[i];
            sBackTrace += "\n";
        }

        //CT_ERROR(("Exception: ErrorCode=%d Msg=%s", m_iErrorCode, m_strErrorMsg.c_str()));
        CT_ERROR(("%s", sBackTrace.c_str()));
#endif


        }

        for (int i = 0; i < m_iUsedCount; ++i)
        {
            m_astData[i] = obj.m_astData[i];
        }
        return *this;
    }

    CTArray<T, COUNT>& operator -= (CTArray<T, COUNT>& obj)
    {
        int iCounts = m_iUsedCount > obj.m_iUsedCount ? m_iUsedCount : obj.m_iUsedCount; 

		iCounts = iCounts > COUNT ? COUNT : iCounts;

        for (int i = 0; i < iCounts; ++i)
        {
            if (i >= m_iUsedCount)
            {
                m_astData[i] = 0;
            }

            if (i >= obj.m_iUsedCount)
            {
                obj[i] = 0;
            }

            m_astData[i] = m_astData[i] - obj[i];
        }

        m_iUsedCount = iCounts; 
        return *this;
    }

    CTArray<T, COUNT>& operator += (CTArray<T, COUNT>& obj)
    {
        int iCounts = m_iUsedCount > obj.m_iUsedCount ? m_iUsedCount : obj.m_iUsedCount; 

		iCounts = iCounts > COUNT ? COUNT : iCounts;

        for (int i = 0; i < iCounts; ++i)
        {
            if (i >= m_iUsedCount)
            {
                m_astData[i] = 0;
            }

            if (i >= obj.m_iUsedCount)
            {
                obj[i] = 0;
            }

            m_astData[i] = m_astData[i] + obj[i];
        }

        m_iUsedCount = iCounts; 
        return *this;
    }

	int Clear()
	{
		m_iUsedCount = 0;
		return 0;
	}

	int Size() const
	{
		return m_iUsedCount;
	}

	/*
	在数组最后一位增加一个元素
	返回值：-1 表示增加失败
	否则返回新增加元素的下标
	*/
	int AddOneItem(const T &tItem)
	{
		if ((COUNT) == m_iUsedCount)
		{//最后一个了，加不了了
			return -1;
		}

		if (m_iUsedCount < 0 || m_iUsedCount > COUNT)
		{
			return -1;
		}
		
		int iIndex = m_iUsedCount;
		m_astData[m_iUsedCount] = tItem;
		++m_iUsedCount;
		return iIndex;
	}




	/*
	删除数组中指定下标的元素，如果删除的非最后一个元素，则用最后一个元素来填补空位
	返回值：-1表示删除失败
	否则返回被删除的元素下标索引
	*/
	int DelOneItem(int iIndex)
	{
		if (iIndex < 0 || iIndex >= m_iUsedCount)
		{
			return -1;
		}

		if (m_iUsedCount <= 0 || m_iUsedCount > COUNT)
		{
			return -1;
		}

		--m_iUsedCount;
		m_astData[iIndex] = m_astData[m_iUsedCount]; //这里不做检查了。。。
		return iIndex;
	}

	/*
	删除数组中指定下标的元素，如果删除的非最后一个元素，则将后面的元素逐个位移过来
	返回值：-1表示删除失败
	否则返回被删除的元素下标索引
	*/
	int ShiftDelOneItem(int iIndex)
	{
		if (iIndex < 0 || iIndex >= m_iUsedCount)
		{
			return -1;
		}
		//if (m_iUsedCount <= 0 || m_iUsedCount > COUNT)//现在外网有很多m_iUsedCount>COUNT的情况
		if(m_iUsedCount <= 0)
		{
			return -1;
		}

		--m_iUsedCount;
		int i = 0;
		for (i = iIndex; i < m_iUsedCount; ++i)
		{
			m_astData[i] = m_astData[i+1];
		}

		return iIndex;
	}

    typedef CTArray<T, COUNT> THIS_ARRAY_TYPE;
    OBJ_CHECK_BEGIN(THIS_ARRAY_TYPE)
    OBJ_CHECK(T)
    OBJ_CHECK_END()
};


} // namespace CTLib

#endif //__CTLIB_ARRAY_H__
