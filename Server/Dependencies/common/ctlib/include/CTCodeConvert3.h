/**
* 更多的编解码方式.
* 字符串文本到二进制对象的转换
* 文本串格式为4字节头部+Data串
* 
*/

#ifndef __CTLIB_CODECONVERT3_H__
#define __CTLIB_CODECONVERT3_H__

#include "CTBase.h"
#include "CTCodeConvert.h"

#ifndef CODECONVERT_STRING_LENGTH 
    #define CODECONVERT_STRING_LENGTH 4
#endif

namespace CTLib
{
    /** 字符串协议解码 */
    struct string_decode {};
    struct string_encode {};

    /** json协议编码 */
    struct json_encode {};

    // 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_STRING_ENCODE_BEGIN \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    if(s.m_ushPos + sizeof(unsigned char) * CODECONVERT_STRING_LENGTH >= s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    unsigned short iPosBegin = s.m_ushPos; \
    s.m_ushPos = (unsigned short)((s.m_ushPos + sizeof(unsigned char) * CODECONVERT_STRING_LENGTH) & 0xFFFF);

#define CT_STRING_ENCODE_END \
    unsigned short iPosEnd = s.m_ushPos; \
    s.m_ushPos = iPosBegin; \
    char sTemp[CODECONVERT_STRING_LENGTH + 1]; \
    ct_snprintf(sTemp, sizeof(sTemp), "%0*hd", CODECONVERT_STRING_LENGTH,  \
        iPosEnd - iPosBegin - sizeof(unsigned char) * CODECONVERT_STRING_LENGTH); \
    s.AppendBinary(sTemp, CODECONVERT_STRING_LENGTH); \
    s.m_ushPos = iPosEnd; 

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, char& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%hd", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;
    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, unsigned char& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%hu", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;
    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, short& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%hd", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;

    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, unsigned short& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%hu", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;

    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, int& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%d", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;

    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, unsigned int& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%u", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;

    return s.m_iRet;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, long& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%ld", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;

    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, unsigned long& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

    if(s.Append("%lu", t))
    {
        return (s.m_iRet = -2);
    }

    CT_STRING_ENCODE_END;

    return s.m_iRet;
}
#endif


template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, int64& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

#ifdef WIN32
    if(s.Append("%I64d", t))
    {
        return (s.m_iRet = -2);
    }
#else
    if(s.Append("%lld", t))
    {
        return (s.m_iRet = -2);
    }
#endif
    CT_STRING_ENCODE_END;
    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, uint64& t, const char* pszName, const string_encode& m)
{
    CT_STRING_ENCODE_BEGIN;

#ifdef WIN32
    if(s.Append("%I64u", t))
    {
        return (s.m_iRet = -2);
    }
#else
    if(s.Append("%llu", t))
    {
        return (s.m_iRet = -2);
    }
#endif
    CT_STRING_ENCODE_END;
    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, float& t, const char* pszName, const string_encode& m)
{
    return s.m_iRet;
}

template<> inline
int CodeConvert<string_encode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const string_encode& m)
{
    if (s.m_iRet != 0) 
    { 
        return s.m_iRet; 
    } 
    if(s.m_ushPos + sizeof(unsigned char) * CODECONVERT_STRING_LENGTH + t.GetDataLen() >= s.m_ushBufSize) 
    { 
        return (s.m_iRet = -1); 
    } 
    
	if(t.GetDataLen() == 0)  //长度不能为0，否则解码时会出错
	{
		if(s.Append("%0*d0", CODECONVERT_STRING_LENGTH, 1))
		{
			return (s.m_iRet = -2); 
		}
		return s.m_iRet;
	}

    if(s.Append("%0*d%s", CODECONVERT_STRING_LENGTH, t.GetDataLen(), t.GetBuffer()))
    {
        return (s.m_iRet = -2); 
    }

    return s.m_iRet;
}

template<> inline
int CodeConvertBegin<string_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const string_encode& m)
{
    t.__max__ = s.m_ushBufSize;
    
    if (s.m_iRet != 0) 
    { 
        return s.m_iRet; 
    } 
    if(s.m_ushPos + sizeof(unsigned char) * CODECONVERT_STRING_LENGTH >= s.m_ushBufSize) 
    { 
        return (s.m_iRet = -1); 
    }

    t.__res__ = s.m_ushPos; 
    s.m_ushPos = (unsigned short)((s.m_ushPos + sizeof(unsigned char) * CODECONVERT_STRING_LENGTH) & 0xFFFF);

    CodeConvert(s, t.__ver__, NULL, m);
    
    return s.m_iRet;
}

template<> inline
int CodeConvertEnd<string_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const string_encode& m)
{
    unsigned short iPosEnd = s.m_ushPos; 
    s.m_ushPos = t.__res__; 
    char sTemp[CODECONVERT_STRING_LENGTH + 1]; 
    ct_snprintf(sTemp, sizeof(sTemp), "%0*hd", CODECONVERT_STRING_LENGTH,  
            iPosEnd - t.__res__ - sizeof(unsigned char) * CODECONVERT_STRING_LENGTH); 
    s.AppendBinary(sTemp, CODECONVERT_STRING_LENGTH); 
    s.m_ushPos = iPosEnd; 

    return s.m_iRet;
}

template<> inline
int CodeConvertArrayBegin<string_encode>(CTCodeStream& s,int& t, const char* pszName, const string_encode& m)
{

    if (s.m_iRet != 0) 
    { 
        return s.m_iRet; 
    } 
    if(s.m_ushPos + sizeof(unsigned char) * CODECONVERT_STRING_LENGTH >= s.m_ushBufSize) 
    { 
        return (s.m_iRet = -1); 
    }

    CodeConvert(s, t, NULL, m);

    return 0;
}

template<> inline 
int CodeConvertArrayEnd<string_encode>(CTCodeStream& s,int& t,  const char* pszName, const string_encode& m)
{
    return 0;
}

template<> inline
int CodeConvertVersion<string_encode>(CTCodeObject& t, unsigned short ushVersion, const string_encode& m)
{
	// 存在新版本向旧版本转换的情况，需要这样处理
	if (t.__ver__ >= ushVersion)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

#define CT_STRING_DECODE(s) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(unsigned char) * CODECONVERT_STRING_LENGTH)) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    unsigned short ush = CTLib::CTStringUtils::StrToInt<unsigned short>(s.m_pszBuf + s.m_ushPos, CODECONVERT_STRING_LENGTH); \
    s.m_ushPos = (unsigned short)((s.m_ushPos + CODECONVERT_STRING_LENGTH) & 0xFFFF); \
    if (ush <= 0) \
    { \
        return (s.m_iRet = -3); \
    }

template<> inline
int CodeConvert<string_decode>(CTCodeStream& s, char& t, const char* pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<char>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream& s,unsigned char& t, const char* pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<unsigned char>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s ,short &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<short>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, unsigned short &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);		

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<unsigned short>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, int &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<int>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, unsigned int &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<unsigned int>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, long &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<long>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, unsigned long &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<unsigned long>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}
#endif

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, int64 &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<int64>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, uint64 &t, const char *pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    int iRet = 0;
    t = CTLib::CTStringUtils::StrToInt<uint64>(s.m_pszBuf + s.m_ushPos, ush, &iRet);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);
    if (iRet)
    {
        return (s.m_iRet = -5);
    }

    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream &s, float &t, const char *pszName, const string_decode& m)
{
    return 0;
}

template<> inline
int CodeConvert<string_decode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);

    if (ush >= t.GetBufferLen())
    {
        return (s.m_iRet = -4);
    }

    memcpy(t.GetBuffer(), s.m_pszBuf + s.m_ushPos, ush);
    t.SetDataLen(ush);
    s.m_ushPos = (unsigned short)((s.m_ushPos + ush) & 0xFFFF);

    return 0;
}

template<> inline
int CodeConvertVersion<string_decode>(CTCodeObject& t, unsigned short ushVersion, const string_decode& m)
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
int CodeConvertArrayBegin<string_decode>(CTCodeStream& s, int& t, const char* pszName, const string_decode& m)
{
    CodeConvert<string_decode>(s, t, pszName, m);
    return 0;
}

template<> inline 
int CodeConvertArrayEnd<string_decode>(CTCodeStream& s, int& t,  const char* pszName, const string_decode& m)
{
    return 0;
}


template<> inline
int CodeConvertBegin<string_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const string_decode& m)
{
    CT_STRING_DECODE(s);
    
    t.__len__ = (unsigned short) ush;
    t.__max__ = s.m_ushPos + t.__len__;

    CodeConvert<string_decode>(s, t.__ver__, pszName, m);
    return 0;
}

template<> inline
int CodeConvertEnd<string_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const string_decode& m)
{
    t.__ver__ = static_cast<unsigned short>(t.__res__);
    s.m_ushPos = t.__max__;     // 从高版本转到低版本时需要这样设置，以便跳过低版本没有的字段
    return 0;
}

#define CT_CHECK_JSON(s) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    }

// json一个包只允许有一个对象，故定义这一组宏把数据包整合成一个对象
#define CT_CONVERT_TO_JSON_BEGIN(s)     s.Append("{")

// 这里的处理类似CodeConvertArrayEnd, 参见CodeConvertArrayEnd的解释
#define CT_CONVERT_TO_JSON_END(s) \
    do \
    { \
        if (s.m_ushPos > 0) \
        { \
            char ch = s.m_pszBuf[s.m_ushPos - 1]; \
            CT_ASSERT(ch == ',' || ch == '{'); \
            if (ch == ',') \
            { \
                --s.m_ushPos; \
            } \
        } \
        s.Append("}"); \
    } \
    while(0)

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, char& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

    // json中数组元素与属性元素的表示方式有区别，根据是否具有属性名来判断其适用哪种编码方式
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%hd,", pszName, t);
    } 
    else
    {
         s.Append("%hd,", t);
    }

    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, unsigned char& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%hu,", pszName, t);
    }
    else
    {
        s.Append("%hu,", t);
    }

    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, short& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%hd,", pszName, t);
    }
    else
    {
        s.Append("%hd,", t);
    }

    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, unsigned short& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%hu,", pszName, t);
    }
    else
    {
         s.Append("%hu,", t);
    }

    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, int& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%d,", pszName, t);
    }
    else
    {
        s.Append("%d,", t);
    }

    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, unsigned int& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%u,", pszName, t);
    }
    else
    {
        s.Append("%u,", t);
    }

    return s.m_iRet;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, long& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);
        
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%ld,", pszName, t);
    }
    else
    {
        s.Append("%ld,", t);
    }

    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, unsigned long& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);
        
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":%lu,", pszName, t);
    }
    else
    {
        s.Append("%lu,", t);
    }

    return s.m_iRet;
}
#endif

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, int64& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

#ifdef WIN32
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":\"%I64d\",", pszName, t);
    }
    else
    {
        s.Append("\"%I64d\",", t);
    }

#else
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":\"%lld\",", pszName, t);
    }
    else
    {
        s.Append("\"%lld\",", t);
    }

#endif
    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, uint64& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);

#ifdef WIN32
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":\"%I64u\",", pszName, t );
    }
    else
    {
        s.Append("\"%I64u\",", t );
    }

#else
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":\"%llu\",", pszName, t);
    }
    else
    {
        s.Append("\"%llu\",", t);
    }

#endif
    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, float& t, const char* pszName, const json_encode& m)
{
    return s.m_iRet;
}

template<> inline
int CodeConvert<json_encode>(CTCodeStream& s, ICodeRaw& t, const char* pszName, const json_encode& m)
{
    CT_CHECK_JSON(s);
    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":\"", pszName);
    }
    else
    {
        s.Append("\"");
    }

    s.AppendJsonEscapeString(t.GetBuffer());
    s.Append("\",");
    return s.m_iRet;
}

template<> inline
int CodeConvertBegin<json_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const json_encode& m)
{
    t.__max__ = s.m_ushBufSize;

    if (pszName && !CT_BIT_TEST(s.m_iReserve3, s.m_iReserve4))
    {
        s.Append("\"%s\":{", pszName);
    }
    else
    {
        s.Append("{");
    }

    ++s.m_iReserve4;
    CT_BIT_CLR(s.m_iReserve3, s.m_iReserve4);

    return 0;
}

template<> inline
int CodeConvertEnd<json_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const json_encode& m)
{
    s.Append("\"__ver__\":%hu},", t.__ver__);
    --s.m_iReserve4;
    return 0;
}

template<> inline
int CodeConvertVersion<json_encode>(CTCodeObject& t, unsigned short ushVersion, const json_encode& m)
{
    CT_ASSERT(t.__ver__ <= ushVersion);		// 要求旧版本数据在前，新版本数据在后
    t.__ver__ = ushVersion;
    return 0;
}

template<> inline
int CodeConvertArrayBegin<json_encode>(CTCodeStream& s,int& t, const char* pszName, const json_encode& m)
{
    // CodeConvertArrayBegin最后必须是]
    // 参见CodeConvertArrayEnd的解释
    s.Append("\"%s\":[", pszName);
    ++s.m_iReserve4;
    CT_BIT_SET(s.m_iReserve3, s.m_iReserve4);

    return 0;
}

template<> inline 
int CodeConvertArrayEnd<json_encode>(CTCodeStream& s,int& t,  const char* pszName, const json_encode& m)
{
    // 这里的处理是为了删除数组最后一个元素的逗号
    // 这要求每个对象转换必须以,结尾 后面不能再有空格或\t\r\n
    // 如果数组为空, 最后一个字符是[, 对这种情况不处理
    if (s.m_ushPos > 0)
    {
        char ch = s.m_pszBuf[s.m_ushPos - 1];
        CT_ASSERT(ch == ',' || ch == '[');
        if (ch == ',')
        {
            --s.m_ushPos;
        }
    }

    s.Append("],");
    --s.m_iReserve4;
    return 0;
}

} // namespace CTLib

#endif //__CTLIB_CODECONVERT3_H__
