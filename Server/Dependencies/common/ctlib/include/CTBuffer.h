/**
* Buffer类
*/

#ifndef __CTLIB_BUFFER_H__
#define __CTLIB_BUFFER_H__

#include <CTBase.h>
#include <string.h>

namespace CTLib
{

class CTBuffer
{
protected:
    int		m_iPos;	// 前两个成员必须是m_iPos, m_pszBuf, 和WSABUF保持一致
    char *	m_pszBuf;
    int		m_iSize;
    int		m_iAct;
    bool    m_bNew;

public:
    CTBuffer(): m_iPos(0), m_pszBuf(0), m_iSize(0), m_iAct(0), m_bNew(false)
    {
    }

    virtual ~CTBuffer()
    {
        Detach();
    }

    int Attach(char* pszBuf, int iSize, int iAct)
    {
        Detach();

        if (pszBuf == NULL)
        {
            m_bNew = true;
            m_pszBuf = new char[iSize];
            if (m_pszBuf == NULL)
            {
                return -1;
            }
        }
        else
        {
            m_bNew = false;
            m_pszBuf = pszBuf;
        }

        m_iSize = iSize;
        m_iPos = 0;
        m_iAct = iAct;

        return 0;
    }

    void Detach()
    {
        if (m_pszBuf != NULL && m_bNew)
        {
            delete [] m_pszBuf;
        }

        m_pszBuf = 0;
        m_iSize = 0;
        m_iPos = 0;
        m_iAct = 0;
        m_bNew = false;
    }

    int Append(const char* pszBuf, int iLen)
    {
        CT_ASSERT(pszBuf && iLen >= 0);

        if (iLen > GetFreeLen())   //org: if (iLen >= GetFreeLen()), modified by will 2008-10-17
        {
            return -1;
        }

        memcpy(m_pszBuf + m_iPos, pszBuf, iLen);
        m_iPos += iLen;
        return 0;
    }

    int Append(int iLen)
    {
        CT_ASSERT(iLen >= 0);

        if (iLen > GetFreeLen())  //org: if (iLen >= GetFreeLen()), modified by will 2008-10-17
        {
            return -1;
        }

        m_iPos += iLen;
        return 0;
    }

    int Remove(int iLen)
    {
        if (iLen > m_iPos)
        {
            return -1;
        }
        else if (iLen < m_iPos) // 优化
        {
            memmove(m_pszBuf, m_pszBuf + iLen, m_iPos - iLen);
        }

        m_iPos -= iLen;
        return 0;
    }

    int Clear()
    {
        m_iPos = 0;
        return 0;
    }

public:
    char* GetUsedBuf() const
    {
        return m_pszBuf;
    }

    int GetUsedLen() const
    {
        return m_iPos;
    }

    char* GetFreeBuf() const
    {
        return m_pszBuf + m_iPos;
    }

    int GetFreeLen() const
    {
        return m_iSize - m_iPos;
    }

    int GetBufferSize() const
    {
        return m_iSize;
    }

    const int Act() const
    {
        return m_iAct;
    }

    int& Act()
    {
        return m_iAct;
    }

    bool IsAttached() const
    {
        return m_pszBuf != 0;
    }

    bool IsNewMemory() const
    {
        return m_bNew;
    }
};

} //namespace CTLib

#endif //__CTLIB_BUFFER_H__
