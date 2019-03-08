/**
* 统计类
*/

#ifndef __CTLIB_STAT_H__
#define __CTLIB_STAT_H__

#include "CTBase.h"

namespace CTLib
{

class CTStatItem
{
public:
	enum
	{
		CTLIB_DESC_MAX = 64,
	};

	char		m_szDesc[CTLIB_DESC_MAX];
	int64		m_iCount;
	int64		m_iZeroCount;
	int64		m_iMax;
	int64		m_iMin;
	int64		m_llAll;

public:
	CTStatItem()
	{
		m_szDesc[0] = '\0';
		Reset();
	}

	~CTStatItem()
	{
	}

	void Put(int i = 1)
	{
		m_llAll += i;
		m_iMax = CT_MAX(i, m_iMax);
		m_iMin = CT_MIN(i, m_iMin);

		if(m_iCount == 0)
		{
			m_iMax = m_llAll;
			m_iMin = m_llAll;
		}
		++m_iCount;
		if (i == 0)
		{
			++ m_iZeroCount;
		}
	}

	void Dump(CTLog* plog, ELogFilter filter)
	{
#ifndef WIN32
		plog->Log(filter, "%s: Count=%lld Max=%lld Min=%lld Avg=%lld All=%lld Zero=%lld",
#else
		plog->Log(filter, "%s: Count=%I64d Max=%I64d Min=%I64d Avg=%I64d All=%I64d Zero=%I64d",
#endif
			m_szDesc, m_iCount, m_iMax, m_iMin, 
			(m_iCount == 0 ? 0 : m_llAll / m_iCount),
			m_llAll, m_iZeroCount);
	}

	void Reset()
	{
		m_iCount = 0;
		m_iZeroCount = 0;
		m_iMax = 0;
		m_iMin = 0;
		m_llAll = 0;
	}
};

class CTStat
{
public:
	CTStat()
	{
		m_pItems = NULL;
		m_iCount = 0;
	}

	CTStat(int iItemCount, const char pszItemDesc[][CTStatItem::CTLIB_DESC_MAX])
	{
		m_pItems = NULL;
		m_iCount = 0;
		Init(iItemCount, pszItemDesc);
	}

	virtual ~CTStat()
	{
		if (m_pItems)
		{
			delete [] m_pItems;
			m_pItems = 0;
			m_iCount = 0;
		}
	}

    int Init(int iItemCount)
    {
        if (iItemCount <= 0)
        {
            return -1;
        }

        m_iCount = iItemCount;
        m_pItems = new CTStatItem[m_iCount];
        if (!m_pItems)
        {
            CT_ERROR(("new CTStatItem[%d] failed", m_iCount));
            return -2;
        }

        for (int i = 0; i < m_iCount; ++i)
        {
            ct_snprintf(CT_STRSIZE(m_pItems[i].m_szDesc), "StatItem_%d", i);
        }

        return 0;
    }

	int Init(int iItemCount, const char pszItemDesc[][CTStatItem::CTLIB_DESC_MAX])
	{
		if (!pszItemDesc || iItemCount <= 0)
		{
			return -1;
		}

		m_iCount = iItemCount;
		m_pItems = new CTStatItem[m_iCount];
		if (!m_pItems)
		{
			CT_ERROR(("new CTStatItem[%d] failed", m_iCount));
			return -2;
		}

		for (int i = 0; i < m_iCount; ++i)
		{
			ct_snprintf(CT_STRSIZE(m_pItems[i].m_szDesc), "%s", pszItemDesc[i]);
		}

		return 0;
	}

	int Put(int iIndex, int i = 1)
	{
		if (iIndex < 0 || iIndex >= m_iCount)
		{
			CT_ERROR(("When put (%d, %d) to stat, index overflow %d", iIndex, i, m_iCount));
			return -1;
		}

		m_pItems[iIndex].Put(i);
		return 0;
	}

    int Put2(int iIndex, int i = 1)
    {
        if (iIndex < 0 || iIndex >= m_iCount)
        {
            CT_ERROR(("When put (%d, %d) to stat, index overflow %d", iIndex, i, m_iCount));
            return -1;
        }

        m_pItems[iIndex].Put(i);
        return 0;
    }

	void Dump(CTLog* plog, ELogFilter filter = EInfo)
	{
		for (int i = 0; i < m_iCount; ++i)
		{
			m_pItems[i].Dump(plog, filter);
			m_pItems[i].Reset();
		}
	}

    void Dump2(CTLog* plog, ELogFilter filter = EInfo)
    {
        for (int i = 0; i < m_iCount; ++i)
        {
            if (m_pItems[i].m_iCount == 0)
            {
                continue;
            }
            
            m_pItems[i].Dump(plog, filter);
            m_pItems[i].Reset();
        }
    }

	int GetCount() const
	{
		return m_iCount;
	}

	CTStatItem& operator[] (int i)
	{
		return m_pItems[i];
	}

private:
	CTStatItem*		m_pItems;
	int				m_iCount;
};

} // namespace

#endif //__CTLIB_STAT_H__
