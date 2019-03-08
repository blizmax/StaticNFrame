/**
* 通用数据编解码转换.
* @note 编码后的缓冲区不能超过64K
*/

#ifndef __CTLIB_DATACONVERT_H__
#define __CTLIB_DATACONVERT_H__

#include "base/CTBaseDefine.h"
#include "CTSizeString.h"
#include "CTArray.h"
#include "CTBuffer.h"

#ifdef WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
	#include <arpa/inet.h>
#endif

namespace CTLib
{

/*
//编解码方式
enum CODE_TYPE
{
    CODE_BIN = 1,       
    CODE_STRING = 2,
    CODE_JSON = 3,
    CODE_MYSQL = 4,
    CODE_ENCRYPTED = 5, //加密协议，用于经过ptkey加密的数据包
};
*/

/** 二进制编码 */
struct bin_encode
{
};

/** 二进制解码 */
struct bin_decode
{
};

/** 输出对象 */
struct dump
{
};

/**
* 可编码类的基类.
* 要用CTCodeConvert进行编码的类都要从此类继承
*/
class CTCodeObject
{
public:
    CTCodeObject() : __len__(0), __ver__(0), __max__(0), __res__(0)
    {
    }

    virtual ~CTCodeObject()
    {
    }

    unsigned short  __len__;
    unsigned short  __ver__;
    unsigned short  __max__;    ///< 记录解析的最后哨位
    unsigned short  __res__;    ///< 保留字段
};

/**
* 用于DataConvert的缓冲区类.
* CTCodeStream并不分配内存，是外部分配内存后传入此类中
* @note 编码后的缓冲区不能超过64K
*/
class CTCodeStream
{
public:
    char*		    m_pszBuf;		///< 指向Buffer的指针
    unsigned short	m_ushBufSize;	///< Buffer总长度 
    unsigned short  m_ushPos;       ///< Buffer当前游标
    int			    m_iRet;			///< 编解码错误号
    void*           m_pReserve1;    ///< 扩展字段1
    void*           m_pReserve2;    ///< 扩展字段2
    int             m_iReserve3;    ///< 扩展字段3
    int             m_iReserve4;    ///< 扩展字段4

public:
    CTCodeStream() :
        m_pszBuf(0), m_ushBufSize(0), m_ushPos(0), m_iRet(0),
        m_pReserve1(0), m_pReserve2(0), m_iReserve3(0), m_iReserve4(0)
    {
    }

    virtual ~CTCodeStream()
    {
    }

    CTCodeStream(char* pszBuffer, unsigned short ushBufSize) :
        m_ushPos(0), m_iRet(0), 
        m_pReserve1(0), m_pReserve2(0), m_iReserve3(0), m_iReserve4(0)
    {
        Attach(pszBuffer, ushBufSize);
    }

    /**
    * 绑定到CTBuffer.
    * @note 这个操作会清空CTBuffer, 只能在编码时使用
    */
    CTCodeStream(CTBuffer& stBuf) :
        m_ushPos(0), m_iRet(0),
        m_pReserve1(0), m_pReserve2(0), m_iReserve3(0), m_iReserve4(0)
    {
        stBuf.Clear();
        Attach(stBuf.GetFreeBuf(), (unsigned short)(stBuf.GetFreeLen() & 0xFFFF));
    }

    /**
    * 绑定外部的内存缓冲区
    */
    int Attach(char* pszBuffer, unsigned short ushBufSize)
    {
        CT_ASSERT((pszBuffer != NULL && ushBufSize > 0));

        if (!(pszBuffer != NULL && ushBufSize > 0))
        {
            return -1;
        }

        m_pszBuf = pszBuffer;
        m_ushBufSize = ushBufSize;
        m_ushPos = 0;
        m_iRet = 0;
        m_pReserve1 = 0;
        m_pReserve2 = 0;
        m_iReserve3 = 0;
        m_iReserve4 = 0;

        return 0;
    }

    /**
    * 开始转换时调用此函数.
    * 用来把一些变量置为0
    */
    void InitConvert()
    {
        m_ushPos = 0;
        m_iRet = 0;
        m_pReserve1 = 0;
        m_pReserve2 = 0;
        m_iReserve3 = 0;
        m_iReserve4 = 0;
    }

    /**
    * 向缓冲区附加内容
    * @return 如果缓冲区不够就返回-1，否则返回向缓冲区写了多少个字节
    */
    int Append(const char* pszFormat, ...)
    {
        char* pszBuf = (char*)(m_pszBuf + m_ushPos);
        int iBufSize = m_ushBufSize - m_ushPos;

        va_list ap;
        va_start(ap, pszFormat);
        int iRet = ct_vsnprintf(pszBuf, iBufSize, pszFormat, ap);
        va_end(ap);

        if (iRet < 0)
        {
            return (m_iRet = -1);
        }

        m_ushPos = m_ushPos + (unsigned short)(iRet);
        return 0;
    }

    /**
    * 向缓冲区附加二进制内容.
    */
    int AppendBinary(const char* psz, int iLen)
    {
        if (m_ushBufSize - m_ushPos < iLen)
        {
            return (m_iRet = -1);
        }

        memcpy((m_pszBuf + m_ushPos), psz, iLen);
        m_ushPos = m_ushPos + (unsigned short)(iLen);
        return 0;
    }

    /**
    * 计算一个字符串JsonEscpae后的长度.
    * 这里认为字符串是utf8编码方式
    */
    static size_t JsonEscapeLen(const char* pszSrc)
    {
		size_t iLen = 0;

		unsigned char* p = (unsigned char*) pszSrc;
		while (*p != 0)
		{
			switch (*p)
			{
				case '\n':
				case '\t':
				case '\b':
				case '\r':
				case '\f':
				case '\\':
				case '\"':
				case '/':
					iLen += 2;
					break;

				default:
					iLen += ((*p < ' ' ? 6 : 1)); // 小于' '的字符会被转化为\\u00xx
					break;
			}
			++p;
		}

		return iLen;
    }

    /**
    * 把utf8字符串做JsonEscape后附加到缓冲区.
    */
    int AppendJsonEscapeString(const char* psz)
    {
    	size_t iLen = JsonEscapeLen(psz);
    	if (m_ushBufSize - m_ushPos < (unsigned short)iLen)
        {
            return (m_iRet = -1);
        }
        
		unsigned char* p = (unsigned char*) psz;
		unsigned char* pszDst = (unsigned char*) m_pszBuf + m_ushPos;
		const char* json_hex_chars = "0123456789abcdef";
		while (*p != 0)
		{
			switch (*p)
			{
				case '\n':	*pszDst = '\\'; *(++pszDst) = 'n';  ++pszDst; break;
				case '\t':	*pszDst = '\\'; *(++pszDst) = 't';  ++pszDst; break;
				case '\b':	*pszDst = '\\'; *(++pszDst) = 'b';  ++pszDst; break;
				case '\r':	*pszDst = '\\'; *(++pszDst) = 'r';  ++pszDst; break;
				case '\f':	*pszDst = '\\'; *(++pszDst) = 'f';  ++pszDst; break;
				case '\\':	*pszDst = '\\'; *(++pszDst) = '\\'; ++pszDst; break;
				case '\"':  *pszDst = '\\'; *(++pszDst) = '\"'; ++pszDst; break;
				case '/':   *pszDst = '\\'; *(++pszDst) = '/';  ++pszDst; break;
				default:
					if (*p < ' ')
					{
						
						*pszDst = '\\';
						*(++pszDst) = 'u';
						*(++pszDst) = '0';
						*(++pszDst) = '0';
						*(++pszDst) = json_hex_chars[*p >> 4];
						*(++pszDst) = json_hex_chars[*p & 0xf];
						++pszDst;
					}
					else
					{
						*pszDst = *p;
						++pszDst;
					}
					break;
			}
			++p;
		}

		// *pszDst = 0; // 最后不加0
		m_ushPos = m_ushPos + (unsigned short)iLen;
		return 0;
    }
};

class CTCodeString: public ICodeRaw
{
protected:
    char*       m_pszBuf;
    int         m_iBufLen;

public:
    CTCodeString(char* pszBuf, int iBufLen):
        m_pszBuf(pszBuf), m_iBufLen(iBufLen)
    {
    }

    virtual char* GetBuffer() const
    {
        return m_pszBuf;
    }

    virtual int GetBufferLen() const
    {
        return m_iBufLen;
    }

    virtual int GetDataLen()
    {
        return (int) strlen(m_pszBuf);
    }

    virtual void SetDataLen(int iLen)
    {
        m_pszBuf[iLen] = '\0';
    }
};

class CTCodeMemory: public ICodeRaw
{
protected:
    char*       m_pszBuf;
    int         m_iBufLen;
    int&        m_iDataLen; ///< DataLen是外部变量

    // 禁止赋值
    CTCodeMemory& operator = (const CTCodeMemory& obj)
    {
        return *this;
    }

public:
    CTCodeMemory(char* pszBuf, int iBufLen, int& iDataLen):
        m_pszBuf(pszBuf), m_iBufLen(iBufLen), m_iDataLen(iDataLen)
    {
    }

    int Init(char* pszBuf, int iBufLen)
    {
        m_pszBuf = pszBuf;
        m_iBufLen = iBufLen;
        return 0;
    }

    virtual char* GetBuffer() const
    {
        return m_pszBuf;
    }

    virtual int GetBufferLen() const
    {
        return m_iBufLen;
    }

    virtual int GetDataLen() const
    {
        return m_iDataLen;
    }
    
    virtual void SetDataLen(int iLen)
    {
        m_iDataLen = iLen;
    }
};

//===============================================

// 注意这个宏有namespace定义，不要在namespace中使用这个宏
#define BEGIN_CODE_CONVERT(ClassName) \
namespace CTLib { \
template<typename METHOD> \
int CodeConvert(CTCodeStream& s, ClassName& t, const char* pszName, const METHOD& m) \
{ \
    CodeConvertBegin(s, t, #ClassName, m);

#define CODE_CONVERT(Field) \
    if (t.__max__ > s.m_ushPos) \
    { \
        CodeConvert(s, t.Field, #Field, m); \
    }

#define END_CODE_CONVERT(ClassName) \
    CodeConvertEnd(s, t, #ClassName, m); \
    return s.m_iRet; \
} \
}

//begin: added by LincLiu on 2010-04-16 for friend template CodeConvert functions 

//在类定义之前预先申明该类使用的CodeConvert友元是模板函数
#define PRE_DECL_CODECONVERT(ClassName) \
class ClassName; \
namespace CTLib{ \
    template<typename T> int CodeConvert(CTCodeStream& s, ClassName& t, const char* pszName, const T& m); \
    template<typename T> int MysqlEncodeStruct(ICodeRaw &stSql, ClassName& stCodeObj, CTBuffer &stEncodeBuf); \
}

//定义CodeConvert为类的友元，这样他就可以访问该类的受保护成员
#define FRIEND_CODECONVERT(ClassName) \
template<typename T> friend int CTLib::CodeConvert(CTCodeStream& s, ClassName& t, const char* pszName, const T& m); \
template<typename T> friend int CTLib::MysqlEncodeStruct(ICodeRaw &stSql, ClassName& stCodeObj, CTBuffer &stEncodeBuf); 

//end: added by LincLiu on 2010-04-16 for friend template CodeConvert functions

#define BEGIN_VERSION(Version)		if (CodeConvertVersion(t, Version, m) == 0) {
#define END_VERSION					}


// 原生类型的各种操作
template<typename METHOD> int CodeConvertBegin(CTCodeStream& s, CTCodeObject& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvertEnd(CTCodeStream& s, CTCodeObject& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvertVersion(CTCodeObject& t, unsigned short ushVersion, const METHOD& m);
template<typename METHOD> int CodeConvertArrayBegin(CTCodeStream& s, int& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvertArrayEnd(CTCodeStream& s, int& t, const char* pszName, const METHOD& m);

template<typename METHOD> int CodeConvert(CTCodeStream& s, char& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, unsigned char& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, short& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, unsigned short& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, int& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, unsigned int& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, long& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, unsigned long& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, int64& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, uint64& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, float& t, const char* pszName, const METHOD& m);
template<typename METHOD> int CodeConvert(CTCodeStream& s, ICodeRaw& t, const char* pszName, const METHOD& m);

//=====================================
// 静态数组

template<typename METHOD, typename T, int COUNT>
int CodeConvert(CTCodeStream& s, CTArray<T, COUNT>& t, const char* pszName, const METHOD& m)
{
    CodeConvertArrayBegin(s, t.m_iUsedCount, pszName, m);
    if (t.m_iUsedCount > COUNT)
    {
        return (s.m_iRet = -5);
    }

    for (int i = 0; i < t.m_iUsedCount; ++i)
    {
        /**
        *对于其他各式的编码方式,变量名没有意义,但是在json和dmp中变量名
        *具有一定意义，为了区别数组元素和属性元素，数组元素的变量名不传入
        */
        CodeConvert(s, t[i], NULL, m);
    }

    CodeConvertArrayEnd(s, t.m_iUsedCount, pszName, m);
    return s.m_iRet;
}

//=====================================
// bin_encode

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_CHECK_BIN_ENCODE(s, size) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    if ((s.m_ushPos + static_cast<int>(size)) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    }

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, char& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(char*)(s.m_pszBuf + s.m_ushPos) = t;
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, unsigned char& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = t;
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, short& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(static_cast<unsigned short>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, unsigned short& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(static_cast<unsigned short>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, int& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl(static_cast<unsigned int>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, unsigned int& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl(t);
    s.m_ushPos += sizeof(t);

    return 0;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, long& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned long*)(s.m_pszBuf + s.m_ushPos) = htonl(static_cast<unsigned long>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, unsigned long& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    *(unsigned long*)(s.m_pszBuf + s.m_ushPos) = htonl(t);
    s.m_ushPos += sizeof(t);

    return 0;
}
#endif 

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, int64& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    // 先编高位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int) ((t >> 32) & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    // 再编低位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int)(t & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, uint64& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    // 先编高位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int) ((t >> 32) & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    // 再编低位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int)(t & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, float& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, sizeof(t));

    int i = 0;
    memcpy(&i, &t, sizeof(t));

    return CodeConvert(s, i, pszName, m);
}

template<> inline
int CodeConvert<bin_encode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const bin_encode& m)
{
    CT_CHECK_BIN_ENCODE(s, (sizeof(unsigned short) + t.GetDataLen()));

    // 先编长度
    unsigned short ushLen = (unsigned short)(t.GetDataLen() & 0xFFFF);
    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(ushLen);
    s.m_ushPos += sizeof(unsigned short);

    // 再编内容
    s.AppendBinary(t.GetBuffer(), t.GetDataLen());

    return 0;
}

template<> inline
int CodeConvertBegin<bin_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const bin_encode& m)
{
    // 编码len，但此时并不知道真正的长度，所以先把0编码进去
    // 然后把m_ushPos保存在__len__中
    t.__ver__ = s.m_ushPos;
    t.__len__ = 0;
    CodeConvert<bin_encode>(s, t.__len__, pszName, m); //
    t.__len__ = t.__ver__; // 先记录下开始编码的位置

    // 编码ver，默认是0
    t.__ver__ = 0;
    CodeConvert<bin_encode>(s, t.__ver__, pszName, m);

    t.__max__ = s.m_ushBufSize;   // 把__max__设到最大
    return 0;
}

template<> inline
int CodeConvertEnd<bin_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const bin_encode& m)
{
    // 保存当前的ushPos
    unsigned short ushOldPos = s.m_ushPos;

    // 计算编码长度
    t.__len__ = static_cast<unsigned short>(s.m_ushPos - t.__len__);

    // 定位到头部
    s.m_ushPos = s.m_ushPos - t.__len__;

    CodeConvert<bin_encode>(s, t.__len__, pszName, m);
    CodeConvert<bin_encode>(s, t.__ver__, pszName, m);

    // 回到原来的位置
    s.m_ushPos = ushOldPos;
    return 0;
}

template<> inline
int CodeConvertVersion<bin_encode>(CTCodeObject& t, unsigned short ushVersion, const bin_encode& m)
{
    CT_ASSERT(t.__ver__ <= ushVersion);		// 要求旧版本数据在前，新版本数据在后
    t.__ver__ = ushVersion;
    return 0;
}

template<> inline
int CodeConvertArrayBegin<bin_encode>(CTCodeStream& s, int& t, const char* pszName, const bin_encode& m)
{
    CodeConvert<bin_encode>(s, t, pszName, m);
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<bin_encode>(CTCodeStream& s, int& t, const char* pszName, const bin_encode& m)
{
    return s.m_iRet;
}

//=====================================
// bin_decode

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_CHECK_BIN_DECODE(s, size) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    if ((s.m_ushPos + static_cast<int>(size)) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    }

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, char& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = (char) s.m_pszBuf[s.m_ushPos];
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, unsigned char& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = (unsigned char) s.m_pszBuf[s.m_ushPos];
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, short& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = static_cast<short>(ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, unsigned short& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, int& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = static_cast<int>(ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, unsigned int& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}
#ifndef CT_BIT64
template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, long& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = static_cast<long>(ntohl(*(unsigned long*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, unsigned long& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    t = ntohl(*(unsigned long*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}
#endif

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, int64& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    // 先解高位的四个字节
    t = ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(unsigned int);
    t <<= 32;

    // 再解低位的四个字节
    t += ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, uint64& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    // 先解高位的四个字节
    t = ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(unsigned int);
    t <<= 32;

    // 再解低位的四个字节
    t += ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, float& t, const char* pszName, const bin_decode& m)
{
    CT_CHECK_BIN_DECODE(s, sizeof(t));

    int i  = 0; 
    CodeConvert(s, i, pszName, m);

    if (s.m_iRet != 0)
    {
        return s.m_iRet;
    }

    memcpy(&t, &i, sizeof(int));

    return 0;
}

template<> inline
int CodeConvert<bin_decode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const bin_decode& m)
{
    // 先解码长度
    CT_CHECK_BIN_DECODE(s, sizeof(unsigned short));
    unsigned short ushLen = ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(unsigned short);

    if (ushLen > t.GetBufferLen())
    {
        return (s.m_iRet = -3);
    }

    // 再解码内容
    // 由于CTSizeString在没有附值之前采用GetDataLen获取到的字符串长度不一定正确
    // 在此处使用ushLen作为CT_CHECK_BIN_DECODE的第二个参数，
    // rapierwu修改
//    CT_CHECK_BIN_DECODE(s, t.GetDataLen());
//    memcpy(t.GetBuffer(), (s.m_pszBuf + s.m_ushPos), t.GetDataLen());
    CT_CHECK_BIN_DECODE(s, ushLen);
    memcpy(t.GetBuffer(), (s.m_pszBuf + s.m_ushPos), ushLen);
    s.m_ushPos = (unsigned short)(s.m_ushPos + ushLen);
    t.SetDataLen(ushLen);

    return 0;
}

template<> inline
int CodeConvertBegin<bin_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const bin_decode& m)
{
    unsigned short ushPos = s.m_ushPos;
    CodeConvert<bin_decode>(s, t.__len__, pszName, m);
    t.__max__ = ushPos + t.__len__;
    CodeConvert<bin_decode>(s, t.__ver__, pszName, m);
    return 0;
}

template<> inline
int CodeConvertEnd<bin_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const bin_decode& m)
{
    t.__ver__ = static_cast<unsigned short>(t.__res__);
    s.m_ushPos = t.__max__; // 从高版本转到低版本时需要这样设置，以便跳过低版本没有的字段
    return 0;
}

template<> inline
int CodeConvertVersion<bin_decode>(CTCodeObject& t, unsigned short ushVersion, const bin_decode& m)
{
    // 存在新版本向旧版本转换的情况，需要这样处理
    if (t.__ver__ >= ushVersion)
    {
        t.__res__ = ushVersion;
        return 0;
    }
    else
    {
        return -1;
    }
}

template<> inline
int CodeConvertArrayBegin<bin_decode>(CTCodeStream& s, int& t, const char* pszName, const bin_decode& m)
{
    CodeConvert<bin_decode>(s, t, pszName, m);
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<bin_decode>(CTCodeStream& s, int& t, const char* pszName, const bin_decode& m)
{
    return s.m_iRet;
}

//=====================================
// dump

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_CHECK_DUMP(s) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    }

template<> inline
int CodeConvert<dump>(CTCodeStream& s, char& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    if (pszName)
    {
        s.Append("%s=%hd(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %hd(%u)\n", t, sizeof(t));
    }
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, unsigned char& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);

    if (pszName)
    {
        s.Append("%s=%hu(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %hu(%u)\n", t, sizeof(t));
    }
    
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, short& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    if (pszName)
    {
        s.Append("%s=%hd(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %hd(%u)\n", t, sizeof(t));
    }
    
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, unsigned short& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    
    if (pszName)
    {
        s.Append("%s=%hu(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %hu(%u)\n", t, sizeof(t));
    }
    
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, int& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    
    if (pszName)
    {
        s.Append("%s=%d(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append("%d(%u)\n", t, sizeof(t));
    }
    
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, unsigned int& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    
    if (pszName)
    {
        s.Append("%s=%u(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append("%u(%u)\n", t, sizeof(t));
    }
    
    return s.m_iRet;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<dump>(CTCodeStream& s, long& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    if (pszName)
    {
        s.Append("%s=%ld(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append("%ld(%u)\n", t, sizeof(t));
    }
    
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, unsigned long& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);
    
    if (pszName)
    {
        s.Append("%s=%lu(%u)\n", pszName, t, sizeof(t));
    } 
    else
    {
        s.Append(" %lu(%u)\n", t, sizeof(t));
    }    
    return s.m_iRet;
}
#endif

template<> inline
int CodeConvert<dump>(CTCodeStream& s, int64& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);

#ifdef WIN32
    if (pszName)
    {
        s.Append("%s=%I64d(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %I64d(%u)\n", t, sizeof(t));
    }
#else
    if (pszName)
    {
        s.Append("%s=%lld(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %lld(%u)\n", t, sizeof(t));
    }
    
#endif
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, uint64& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);

#ifdef WIN32
    if (pszName)
    {
        s.Append("%s=%I64u(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %I64u(%u)\n", t, sizeof(t));
    }
    
#else
    if (pszName)
    {
        s.Append("%s=%llu(%u)\n", pszName, t, sizeof(t));
    }
    else
    {
        s.Append(" %llu(%u)\n", t, sizeof(t));
    }
    
#endif
    return s.m_iRet;
}

template<> inline
int CodeConvert<dump>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const dump& m)
{
    CT_CHECK_DUMP(s);

    if (pszName)
    {
        s.Append("%s=(binary: buffer=%p bufferlen=%d datalen=%d)\n",
            pszName, t.GetBuffer(), t.GetBufferLen(), t.GetDataLen());
    }
    else
    {
        s.Append(" (binary: buffer=%p bufferlen=%d datalen=%d)\n",
            t.GetBuffer(), t.GetBufferLen(), t.GetDataLen());
    }
   
    return s.m_iRet;
}

template<> inline
int CodeConvertBegin<dump>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const dump& m)
{
    t.__max__ = s.m_ushBufSize;
    s.Append("%s = { ", pszName);
    return 0;
}

template<> inline
int CodeConvertEnd<dump>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const dump& m)
{
    s.Append("__ver__ = %hu };\n", t.__ver__);
    return 0;
}

template<> inline
int CodeConvertVersion<dump>(CTCodeObject& t, unsigned short ushVersion, const dump& m)
{
    // 存在新版本向旧版本转换的情况，需要这样处理
    if (t.__ver__ >= ushVersion)
    {
        t.__res__ = ushVersion;
        return 0;
    }
    else
    {
        return -1;
    }
}

template<> inline
int CodeConvertArrayBegin<dump>(CTCodeStream& s, int& t, const char* pszName, const dump& m)
{
    CodeConvert<dump>(s, t, pszName, m);
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<dump>(CTCodeStream& s, int& t, const char* pszName, const dump& m)
{
    return s.m_iRet;
}

} //namespace CTLib

#endif //__CTLIB_DATACONVERT_H__
