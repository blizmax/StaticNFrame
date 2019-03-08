/**
* MySQL数据编解码
*/

#ifndef __CTLIB_CODEMYSQL_H__
#define __CTLIB_CODEMYSQL_H__

#include "CTCodeConvert.h"
#include "CTMySQL.h"
#include "CTMySQLCharset.h"
#ifdef WIN32
#include "CTNull.h"
#else
//#include <mysql.h>
//#include <my_global.h>
//#include <my_sys.h>
#endif

namespace CTLib
{

/** MySQL编码方式 */
struct mysql_encode {};

/** MySQL解码方式 */
struct mysql_decode {};

//=====================================
// mysql_encode
// 是数据库记录的编码方式，所以只会编码原生类型和ICodeRaw，不能编码复合类型和数组

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_MYSQL_ENCODE(s, t) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(t) + sizeof(unsigned char))) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = sizeof(t); \
    s.m_ushPos += sizeof(unsigned char);


template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, char& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(char*)(s.m_pszBuf + s.m_ushPos) = t;
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, unsigned char& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = t;
    s.m_ushPos += sizeof(unsigned char);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, short& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(static_cast<unsigned short>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, unsigned short& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(static_cast<unsigned short>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, int& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl(static_cast<unsigned int>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, unsigned int& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl(t);
    s.m_ushPos += sizeof(t);

    return 0;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, long& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned long*)(s.m_pszBuf + s.m_ushPos) = htonl(static_cast<unsigned long>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, unsigned long& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    *(unsigned long*)(s.m_pszBuf + s.m_ushPos) = htonl(t);
    s.m_ushPos += sizeof(t);

    return 0;
}
#endif

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, int64& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    // 先编高位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int) ((t >> 32) & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    // 再编低位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int)(t & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, uint64& t, const char* pszName, const mysql_encode& m)
{
    CT_MYSQL_ENCODE(s, t);

    // 先编高位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int) ((t >> 32) & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    // 再编低位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int)(t & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<mysql_encode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const mysql_encode& m)
{
    if (s.m_iRet != 0)
    {
        return s.m_iRet;
    }
    if ((s.m_ushPos + static_cast<int>(sizeof(t) + sizeof(unsigned char))) > s.m_ushBufSize)
    {
        return (s.m_iRet = -1);
    }
    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = MYSQL_VARIANT;
    s.m_ushPos += sizeof(unsigned char);

    // 先编长度
    unsigned short ushLen = (unsigned short)(t.GetDataLen() & 0xFFFF);
    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(ushLen);
    s.m_ushPos += sizeof(unsigned short);

    // 再编内容
    s.AppendBinary(t.GetBuffer(), t.GetDataLen());

    return 0;
}


template<> inline
int CodeConvertBegin<mysql_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const mysql_encode& m)
{
    t.__max__ = s.m_ushBufSize;   // 把__max__设到最大
    t.__len__ = s.m_ushPos;       // 记录起始位置

    if(s.m_iRet != 0)
    {
        return s.m_iRet;
    }

    if((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize)
    {
        return (s.m_iRet = -1);
    }

    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = MYSQL_BLOB;
    s.m_ushPos += sizeof(unsigned char);

    int iLen = t.__len__;
    CodeConvert(s, iLen, pszName, m);  //先保存起始位置
    return s.m_iRet;
}

template<> inline
int CodeConvertEnd<mysql_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const mysql_encode& m)
{
    // 保存当前的ushPos
    unsigned short ushOldPos = s.m_ushPos;

    // 计算编码长度
    t.__len__ = static_cast<unsigned short>(s.m_ushPos - t.__len__);

    // 定位到头部
    s.m_ushPos = s.m_ushPos - t.__len__;

    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = MYSQL_BLOB;
    s.m_ushPos += sizeof(unsigned char);

    int iLen = t.__len__;
    CodeConvert(s, iLen, pszName, m);

    // 回到原来的位置
    s.m_ushPos = ushOldPos;

    return s.m_iRet;
}


//=====================================
// mysql_decode
// 是数据库记录的解码方式，所以只会解码原生类型和ICodeRaw，不能解码复合类型和数组

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_MYSQL_DECODE(s, t) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    unsigned char& uch = *(unsigned char*)(s.m_pszBuf + s.m_ushPos); \
    s.m_ushPos += sizeof(unsigned char); \
    if (uch == MYSQL_NULL) \
    { \
        return -3; \
    } \
    if (uch != MYSQL_VARIANT && uch != sizeof(t)) \
    { \
        return (s.m_iRet = -4); \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(t))) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    }

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, char& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

    t = (char) s.m_pszBuf[s.m_ushPos];
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, unsigned char& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

    t = (unsigned char) s.m_pszBuf[s.m_ushPos];
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, short& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

    t = static_cast<short>(ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, unsigned short& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

    t = ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, int& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);
    
    t = static_cast<int>(ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, unsigned int& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

    t = ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, long& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);
    
    t = static_cast<long>(ntohl(*(unsigned long*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, unsigned long& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

    t = ntohl(*(unsigned long*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}

#endif


template<> inline
int CodeConvert<mysql_decode>(CTCodeStream& s, int64& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

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
int CodeConvert<mysql_decode>(CTCodeStream& s, uint64& t, const char* pszName, const mysql_decode& m)
{
    CT_MYSQL_DECODE(s, t);

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
int CodeConvert<mysql_decode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const mysql_decode& m)
{
    if (s.m_iRet != 0) 
    { 
        return s.m_iRet; 
    } 
    if ((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize) 
    { 
        return (s.m_iRet = -1); 
    } 
    unsigned char& uch = *(unsigned char*)(s.m_pszBuf + s.m_ushPos); 
    s.m_ushPos += sizeof(unsigned char); 
    if (uch == MYSQL_NULL) 
    { 
        return -3; 
    } 
    if (uch != MYSQL_VARIANT) 
    { 
        return (s.m_iRet = -4); 
    } 

    // 先解码长度
    unsigned short ushLen = ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(unsigned short);
    if (ushLen > t.GetBufferLen())
    {
        return (s.m_iRet = -3);
    }

    if (s.m_ushPos + ushLen > s.m_ushBufSize)
    {
        return (s.m_iRet = -1);
    }

    // 再解码内容
    memcpy(t.GetBuffer(), (s.m_pszBuf + s.m_ushPos), ushLen);
    s.m_ushPos = (unsigned short)(s.m_ushPos + ushLen);
    t.SetDataLen(ushLen);

    return 0;
}


template<> inline
int CodeConvertBegin<mysql_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const mysql_decode& m)
{
    t.__max__ = s.m_ushBufSize; // mysql_decode这种解析不知道这个对象具体有多大, 所以直接设为最大
    t.__len__ = s.m_ushPos;     //记录原始长度

    if(s.m_iRet)
    {
        return s.m_iRet;
    }
    if(s.m_ushPos + static_cast<int>(sizeof(unsigned char)) > s.m_ushBufSize)
    {
        return (s.m_iRet = -1);
    }

    unsigned char uch = *(char*)(s.m_pszBuf + s.m_ushPos);
    //s.m_ushPos += static_cast<int>(sizeof(unsigned char));  //MySQLEncode时，不会区分结构，uch可能用来标识下一个字段

    if(uch == MYSQL_BLOB)
    {
        s.m_ushPos += static_cast<int>(sizeof(unsigned char));

        int iLen = 0;
        CodeConvert(s, iLen, NULL, m);
        if(s.m_iRet != 0)
        {
            return s.m_iRet;
        }

        if((s.m_ushPos + iLen) > s.m_ushBufSize)
        {
            return (s.m_iRet = -1);
        }

        t.__max__ = (unsigned short)iLen + t.__len__;
        if(t.__max__ == 0)
        {
            t.__res__ = MYSQL_NULL;
            return (s.m_iRet = -2);  //在结束后会根据__res__的值将m_iRet恢复
        }
    }

    return s.m_iRet;
}

template<> inline
int CodeConvertEnd<mysql_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const mysql_decode& m)
{
    if(t.__res__ == MYSQL_NULL)
    {
        return (s.m_iRet = 0);
    }

    s.m_ushPos = t.__max__; //如果成员有删除，则跳过相关的数据，从下个结构开始decode

    return s.m_iRet;
}

template<> inline
int CodeConvertVersion<mysql_decode>(CTCodeObject& t, unsigned short ushVersion, const mysql_decode& m)
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
int CodeConvertVersion<mysql_encode>(CTCodeObject& t, unsigned short ushVersion, const mysql_encode& m)
{
	CT_ASSERT(t.__ver__ <= ushVersion);		// 要求旧版本数据在前，新版本数据在后
	t.__ver__ = ushVersion;
	return 0;
}

template<> inline
int CodeConvertArrayBegin<mysql_encode>(CTCodeStream& s, int& t, const char* pszName, const mysql_encode& m)
{
    if(s.m_iRet != 0)
    {
        return s.m_iRet;
    }

    if((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize)
    {
        return (s.m_iRet = -1);
    }

    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = MYSQL_BLOB;
    s.m_ushPos += sizeof(unsigned char);

    CodeConvert(s, t, pszName, m);
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<mysql_encode>(CTCodeStream& s, int& t, const char* pszName, const mysql_encode& m)
{
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayBegin<mysql_decode>(CTCodeStream& s, int& t, const char* pszName, const mysql_decode& m)
{
    if(s.m_iRet)
    {
        return s.m_iRet;
    }

    if ((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize) 
    { 
        return (s.m_iRet = -1); 
    } 
    unsigned char& uch = *(unsigned char*)(s.m_pszBuf + s.m_ushPos); 
    s.m_ushPos += sizeof(unsigned char); 

    if (uch != MYSQL_BLOB) 
    {
        return (s.m_iRet = -4); 
    }

    CodeConvert(s, t, pszName, m);
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<mysql_decode>(CTCodeStream& s, int& t, const char* pszName, const mysql_decode& m)
{
    return s.m_iRet;
}


//=====================================

/**
* mysql操作类型.
*/
enum
{
    MYSQL_SELECT = 1,
    MYSQL_INSERT = 2,
    MYSQL_UPDATE = 3,
    MYSQL_DELETE = 4,
    MYSQL_REPLACE = 5,
};

class CTCodeMySQLResult: public CTCodeObject
{
public:
    int m_iResult;
    int m_iOpType;
    int m_iRowCount;
    int m_iFieldCount;
    int m_iLastInsertId;
};

} // namespace CTLib

BEGIN_CODE_CONVERT(CTCodeMySQLResult)
    CODE_CONVERT(m_iResult)
    CODE_CONVERT(m_iOpType)
    CODE_CONVERT(m_iRowCount)
    CODE_CONVERT(m_iFieldCount)
    CODE_CONVERT(m_iLastInsertId)
END_CODE_CONVERT(CTCodeMySQLResult)

namespace CTLib
{

/** 把记录集编码 */
inline int MySQLEncode(CTCodeStream& s, CTMySQL& db, int iOpType)
{
    CTCodeMySQLResult ret;
    ret.m_iResult = 0;
    ret.m_iOpType = iOpType;
    ret.m_iLastInsertId = (iOpType == MYSQL_INSERT ? (int)db.GetInsertId() : -1);
    if (iOpType != MYSQL_SELECT)
    {
        ret.m_iRowCount = (int)db.GetAffectedRows();
        ret.m_iFieldCount = 0;
        CodeConvert(s, ret, NULL, bin_encode());
        return s.m_iRet;
    }

    ret.m_iRowCount = (int)db.GetRowCount();
    ret.m_iFieldCount = (int)db.GetFieldCount();
    CodeConvert(s, ret, NULL, bin_encode());

    int iTmp = 0;
    while (db.FetchRow())
    {
        for (unsigned int i = 0; i < db.GetFieldCount(); ++i)
        {
            char c = db.GetFieldType(i);
            CT_TRACE(("mysql get field %d", c));
            if (c == MYSQL_NULL)
            {
                return -3;  // 不支持NULL字段
            }
            else if (c == MYSQL_TINY)
            {
                char cTmp = (char)(db.GetInt(i) & 0xFF);
                CodeConvert(s, cTmp, NULL, mysql_encode());
            }
            else if (c == MYSQL_SHORT)
            {
                short shTmp = (short)(db.GetInt(i) & 0xFFFF);
                CodeConvert(s, shTmp, NULL, mysql_encode());
            }
            else if (c == MYSQL_LONG)
            {
                iTmp = db.GetInt(i);
                CodeConvert(s, iTmp, NULL, mysql_encode());
            }
            else if (c == MYSQL_LONGLONG)
            {
                int64 llTmp = strtoll(db.GetField(i), NULL, 10);
                CodeConvert(s, llTmp, NULL, mysql_encode());
            }
            else if (c == MYSQL_VARIANT)
            {
                iTmp = (int) db.GetFieldLength(i);
                CTCodeMemory t(db.GetField(i), iTmp, iTmp);
                CodeConvert(s, t, NULL, mysql_encode());
            }
            else if (c == MYSQL_BLOB)
            {
                iTmp = (int) db.GetFieldLength(i);
                if(iTmp == 0)
                {
                    unsigned char uch = MYSQL_BLOB;
                    s.AppendBinary((char*)&uch, (int)sizeof(uch));
                    CodeConvert(s, iTmp, NULL, mysql_encode());
                }
                else
                {
                    s.AppendBinary(db.GetField(i), iTmp);
                }
            }
            else
            {
                return -4;  // 不支持的字段类型
            }

            if (s.m_iRet)
            {
                return s.m_iRet;
            }
        }
    }

    return 0;
}

typedef CTSingleton<CTMySQLCharset, 0> TMySQLCharset;
/** 把结构编码成二进制串，用于构造sql */
template<typename T>
inline int MysqlEncodeStruct(ICodeRaw &stSql, T& stCodeObj, CTBuffer &stEncodeBuf)
{
    CTCodeStream s(stEncodeBuf); 
    CodeConvert(s, stCodeObj, NULL, mysql_encode());
    if(s.m_iRet)
    {
        return -1;
    }
    stEncodeBuf.Append(s.m_ushPos);

    const int iOrgLen = stSql.GetDataLen();
    const int iFreeLen = stSql.GetBufferLen() - stSql.GetDataLen();
    char* pszFreeBuf = stSql.GetBuffer() + stSql.GetDataLen();
    if (iFreeLen < s.m_ushPos) // 不是*2, 因为escape_string_for_mysql会判断长度, 确保不溢出
    {
        return -2;
    }

    int iLength = TMySQLCharset::Instance()->EscapeString(pszFreeBuf, iFreeLen,
        stEncodeBuf.GetUsedBuf(), stEncodeBuf.GetUsedLen());

    if(iLength < 0)
    {
        return -3;
    }
    stSql.SetDataLen(iLength+iOrgLen);

    return 0;
}

} // namespace CTLib

#endif //__CTLIB_CODEMYSQL_H__
