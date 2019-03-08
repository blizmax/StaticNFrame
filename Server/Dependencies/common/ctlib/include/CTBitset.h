

#ifndef __CTLIB_BITSET_H__
#define __CTLIB_BITSET_H__

namespace CTLib
{

/**
* 位操作的接口类.
* 小写的方法和bitset保持一致
*/
class CTBitsetBase
{
public:
    virtual ~CTBitsetBase() {}

    /**
    * 返回位元素的个数.
    */
    virtual size_t size() const = 0;

    /**
    * 返回数组.
    */
    virtual unsigned int* GetData() const = 0;

public:
    size_t GetElementSize() const
    {
        return sizeof(unsigned int) * 8;
    }

    /**
    * 返回数组的CountOf.
    */
    int GetCountOf() const
    {
        return CT_CEIL((int)size(), (int)GetElementSize());
    }

    /**
    * iPos 位是否为1.
    * @note iPos从开始计数
    */
    bool test(size_t iPos) const
    {
        if (iPos >= size())
        {
            throw CTException(-1, "invalid CTBitsetBase<%d> position %d", size(), iPos);
        }

        return ((GetData()[iPos / GetElementSize()]
            & ((unsigned int)1 << (iPos % GetElementSize()))) != 0);
    }

    /**
    * 任意位是否为1.
    */
    bool any() const
    {
        for (int i = GetCountOf() - 1; i >= 0; --i)
        {
            if (GetData()[i] != 0)
            {
                return true;
            }
        }

        return false;
    }

    /**
    * 是否没有位为1.
    */
    bool none() const
    {
        return !any();
    }

    /**
    * 值是1 的位的个数.
    */
    size_t count() const
    {
        static char _Bitsperhex[] = "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4";
        size_t iRet = 0;
        for (int i = GetCountOf() - 1; i >= 0; --i)
        {
            for (unsigned int ui = GetData()[i]; ui != 0; ui >>= 4)
            {
                iRet += _Bitsperhex[ui & 0xF];
            }
        }
        return iRet;
    }

    /**
    * 翻转所有的位.
    */
    void flip()
    {
        for (int i = GetCountOf() - 1; i >= 0; --i)
        {
            GetData()[i] = (unsigned int)~GetData()[i];
        }

        Trim();
    }

    /**
    * 翻转pos 位
    */
    void flip(size_t iPos)
    {
        if (iPos >= size())
        {
            throw CTException(-1, "invalid CTBitsetBase<%d> position %d", size(), iPos);
        }

        GetData()[iPos / GetElementSize()] ^= (unsigned int)1 << iPos % GetElementSize();
    }

    /**
    * 将所有位置1
    */
    void set()
    {
        Tidy((unsigned int)~0);
    }

    /**
    * 将pos 位置bVal
    */
    void set(size_t iPos, bool bVal = true)
    {
        if (iPos >= size())
        {
            throw CTException(-1, "invalid CTBitsetBase<%d> position %d", size(), iPos);
        }

        if (bVal)
        {
            GetData()[iPos / GetElementSize()] |= (unsigned int)1 << iPos % GetElementSize();
        }
        else
        {
            GetData()[iPos / GetElementSize()] &= ~((unsigned int)1 << iPos % GetElementSize());
        }
    }

    /**
    * 将所有位置0.
    */
    void reset()
    {
        Tidy();
    }

    /**
    * 将pos 位置0.
    */
    void reset(size_t iPos)
    {
        set(iPos, false);
    }

private:
    /**
    * set all words to iVal.
    */
    void Tidy(unsigned int iVal = 0)
    {
        for (int i = GetCountOf(); i >= 0; --i)
        {
            GetData()[i] = iVal;
        }
        if (iVal != 0)
        {
            Trim();
        }
    }

    /**
    * clear any trailing bits in last word.
    */
    void Trim()
    {
        if (size() % GetElementSize() != 0)
        {
            GetData()[GetCountOf() - 1] &= ((unsigned int)1 << size() % GetElementSize()) - 1;
        }
    }
};

/**
* 位操作类
*/
template<int COUNT>
class CTBitset: public CTBitsetBase
{
private:
    enum
    {
        TYPE_SIZE = sizeof (unsigned int) * 8,	   ///< 每个unsigned int能表示多少个bit
        ARRAY_SIZE = CT_CEIL(COUNT, TYPE_SIZE),    ///< 共需要多少个unsigned int来表示所有的bit
    };

    unsigned int m_astArray[ARRAY_SIZE];

public:
    virtual size_t size() const
    {
        return COUNT;
    }

    virtual unsigned int* GetData() const
    {
        return (unsigned int*)(m_astArray);
    }
};

/**位管理接口类,buff由调用者提供*/
class CTBitsetMgr: public CTBitsetBase
{
private:
    enum
    {
        TYPE_SIZE  = sizeof (unsigned int) * 8,	   ///< 每个unsigned int能表示多少个bit    
    };

    int m_BitCounts;
    unsigned int* m_pastArray;

public:

    virtual size_t size() const
    {
        return m_BitCounts;
    }

    virtual unsigned int* GetData() const
    {
        return m_pastArray;
    }

    CTBitsetMgr()
    {
        m_BitCounts = 0;
        m_pastArray = NULL;
    }

    CTBitsetMgr(void* pBuff, int iBitSize)
    {
        m_BitCounts = iBitSize;
        m_pastArray = (unsigned int*)pBuff;
    }

    static int GetByteCount(int iBitSize)
    {
        return CT_CEIL(iBitSize, TYPE_SIZE) * sizeof (unsigned int);
    }

    int Init(void* pBuff, int iBitSize)
    {
        m_BitCounts = iBitSize;
        m_pastArray = (unsigned int*)pBuff;
        return GetByteCount(m_BitCounts);
    }
  
};

} //namespace CTLib


#endif //__CTLIB_BITSET_H__
