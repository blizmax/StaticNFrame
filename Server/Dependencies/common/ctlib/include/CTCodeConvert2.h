/**
* 更多的编解码方式.
*/

#ifndef __CTLIB_CODECONVERT2_H__
#define __CTLIB_CODECONVERT2_H__

#include "CTBitset.h"
#include "CTCodeConvert.h"

namespace CTLib
{

/** 带类型的二进制编码 */
struct update_encode {};

/** 带类型的二进制解码 */
struct update_decode {};

/** 带类型的输出 */
struct update_dump {};

/**
* 用于type_bin_encode/type_bin_decode中表示类型
*/
enum
{
    EBT_NULL = 0,   ///< 表示后续是空值
    EBT_CHAR1 = 1,  ///< 表示后续是1字节
    EBT_CHAR2 = 2,  ///< 表示后续是2字节
    EBT_CHAR4 = 4,  ///< 表示后续是4字节
    EBT_CHAR8 = 8,  ///< 表示后续是8字节
    EBT_OBJECT = 255,   ///< 表示后续是对象
};

//=====================================
// update_encode

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_UPDATE_ENCODE(s, t) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    CTBitsetBase* pstBitset = (CTBitsetBase*)(s.m_pReserve1); \
    if (!pstBitset) \
    { \
        return (s.m_iRet = -3); \
    } \
    if (!pstBitset->test(static_cast<size_t>((char*)(&t) - (char*)(s.m_pReserve2)))) \
    { \
        if ((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize) \
        { \
            return (s.m_iRet = -1); \
        } \
        *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = EBT_NULL; \
        s.m_ushPos += sizeof(unsigned char); \
        return 0; \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(t) + sizeof(unsigned char))) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = sizeof(t); \
    s.m_ushPos += sizeof(unsigned char);

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, char& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(char*)(s.m_pszBuf + s.m_ushPos) = t;
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, unsigned char& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = t;
    s.m_ushPos += sizeof(unsigned char);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, short& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(static_cast<unsigned short>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, unsigned short& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned short*)(s.m_pszBuf + s.m_ushPos) = htons(static_cast<unsigned short>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, int& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl(static_cast<unsigned int>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, unsigned int& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl(t);
    s.m_ushPos += sizeof(t);

    return 0;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, long& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned long*)(s.m_pszBuf + s.m_ushPos) = htonl(static_cast<unsigned long>(t));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, unsigned long& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    *(unsigned long*)(s.m_pszBuf + s.m_ushPos) = htonl(t);
    s.m_ushPos += sizeof(t);

    return 0;
}
#endif 

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, int64& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    // 先编高位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int) ((t >> 32) & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    // 再编低位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int)(t & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}

template<> inline
int CodeConvert<update_encode>(CTCodeStream& s, uint64& t, const char* pszName, const update_encode& m)
{
    CT_UPDATE_ENCODE(s, t);

    // 先编高位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int) ((t >> 32) & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    // 再编低位的四个字节
    *(unsigned int*)(s.m_pszBuf + s.m_ushPos) = htonl((unsigned int)(t & 0xFFFFFFFF));
    s.m_ushPos += sizeof(unsigned int);

    return 0;
}


template<> inline
int CodeConvertBegin<update_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const update_encode& m)
{
    *(unsigned char*)(s.m_pszBuf + s.m_ushPos) = EBT_OBJECT;
    s.m_ushPos += sizeof(unsigned char);

    // 编码len，但此时并不知道真正的长度，所以先把0编码进去
    // 然后把m_ushPos保存在__len__中
    t.__ver__ = s.m_ushPos;
    t.__len__ = 0;
    CodeConvert<bin_encode>(s, t.__len__, pszName, bin_encode()); //
    t.__len__ = t.__ver__; // 先记录下开始编码的位置

    // 编码ver，默认是0
    t.__ver__ = 0;
    CodeConvert<bin_encode>(s, t.__ver__, pszName, bin_encode());

    t.__max__ = s.m_ushBufSize;   // 把__max__设到最大
    return 0;
}

template<> inline
int CodeConvertEnd<update_encode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const update_encode& m)
{
    // 保存当前的ushPos
    unsigned short ushOldPos = s.m_ushPos;

    // 计算编码长度
    t.__len__ = static_cast<unsigned short>(s.m_ushPos - t.__len__);

    // 定位到头部
    s.m_ushPos = s.m_ushPos - t.__len__;

    CodeConvert<bin_encode>(s, t.__len__, pszName, bin_encode());
    CodeConvert<bin_encode>(s, t.__ver__, pszName, bin_encode());

    // 回到原来的位置
    s.m_ushPos = ushOldPos;
    return 0;
}

template<> inline
int CodeConvertVersion<update_encode>(CTCodeObject& t, unsigned short ushVersion, const update_encode& m)
{
    CT_ASSERT(t.__ver__ <= ushVersion);		// 要求旧版本数据在前，新版本数据在后
    t.__ver__ = ushVersion;
    return 0;
}

template<> inline
int CodeConvertArrayBegin<update_encode>(CTCodeStream& s, int& t, const char* pszName, const update_encode& m)
{
    CodeConvert<bin_encode>(s, t, pszName, bin_encode());
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<update_encode>(CTCodeStream& s, int& t, const char* pszName, const update_encode& m)
{
    return s.m_iRet;
}

//=====================================
// update_decode

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_UPDATE_DECODE(s, t) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    CTBitsetBase* pstBitset = (CTBitsetBase*)(s.m_pReserve1); \
    if (!pstBitset) \
    { \
        return (s.m_iRet = -3); \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(unsigned char))) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    unsigned char& uch = *(unsigned char*)(s.m_pszBuf + s.m_ushPos); \
    s.m_ushPos += sizeof(unsigned char); \
    if (uch == EBT_NULL) \
    { \
        return 0; \
    } \
    if (uch != EBT_OBJECT && uch != sizeof(t)) \
    { \
        return (s.m_iRet = -4); \
    } \
    if ((s.m_ushPos + static_cast<int>(sizeof(t))) > s.m_ushBufSize) \
    { \
        return (s.m_iRet = -1); \
    } \
    pstBitset->set(static_cast<size_t>((char*)(&t) - (char*)(s.m_pReserve2)));

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, char& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = (char) s.m_pszBuf[s.m_ushPos];
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, unsigned char& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = (unsigned char) s.m_pszBuf[s.m_ushPos];
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, short& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = static_cast<short>(ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, unsigned short& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = ntohs(*(unsigned short*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, int& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = static_cast<int>(ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, unsigned int& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = ntohl(*(unsigned int*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, long& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = static_cast<long>(ntohl(*(unsigned long*)(s.m_pszBuf + s.m_ushPos)));
    s.m_ushPos += sizeof(t);

    return 0;
}

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, unsigned long& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

    t = ntohl(*(unsigned long*)(s.m_pszBuf + s.m_ushPos));
    s.m_ushPos += sizeof(t);

    return 0;
}
#endif

template<> inline
int CodeConvert<update_decode>(CTCodeStream& s, int64& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

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
int CodeConvert<update_decode>(CTCodeStream& s, uint64& t, const char* pszName, const update_decode& m)
{
    CT_UPDATE_DECODE(s, t);

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
int CodeConvertBegin<update_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const update_decode& m)
{
    unsigned char& uch = *(unsigned char*)(s.m_pszBuf + s.m_ushPos);
    s.m_ushPos += sizeof(unsigned char);
    if (uch != EBT_OBJECT)
    {
        return (s.m_iRet = -4);
    }

    unsigned short ushPos = s.m_ushPos;
    CodeConvert<bin_decode>(s, t.__len__, pszName, bin_decode());
    t.__max__ = ushPos + t.__len__;
    CodeConvert<bin_decode>(s, t.__ver__, pszName, bin_decode());
    return 0;
}

template<> inline
int CodeConvertEnd<update_decode>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const update_decode& m)
{
    t.__ver__ = static_cast<unsigned short>(t.__res__);
    s.m_ushPos = t.__max__;     // 从高版本转到低版本时需要这样设置，以便跳过低版本没有的字段
    return 0;
}

template<> inline
int CodeConvertVersion<update_decode>(CTCodeObject& t, unsigned short ushVersion, const update_decode& m)
{
    if (t.__ver__ >= static_cast<int>(ushVersion))
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
int CodeConvertArrayBegin<update_decode>(CTCodeStream& s, int& t, const char* pszName, const update_decode& m)
{
    CodeConvert<bin_decode>(s, t, pszName, bin_decode());
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<update_decode>(CTCodeStream& s, int& t, const char* pszName, const update_decode& m)
{
    return s.m_iRet;
}

//=====================================
// update_dump

// 编解码太容易出现缓冲区攻击了，牺牲些性能换取安全是值得的
#define CT_UPDATE_DUMP(s) \
    if (s.m_iRet != 0) \
    { \
        return s.m_iRet; \
    } \
    CTBitsetBase* pstBitset = (CTBitsetBase*)(s.m_pReserve1); \
    if (!pstBitset) \
    { \
        return (s.m_iRet = -3); \
    } \
    if (!pstBitset->test(static_cast<size_t>((char*)(&t) - (char*)(s.m_pReserve2)))) \
    { \
        return 0; \
    }

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, char& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%hd(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, unsigned char& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%hu(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, short& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%hd(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, unsigned short& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%hu(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, int& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%d(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, unsigned int& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%u(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

#ifndef CT_BIT64
template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, long& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%ld(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, unsigned long& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

    s.Append("%s=%lu(%u)\n", pszName, t, sizeof(t));
    return s.m_iRet;
}
#endif

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, int64& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

#ifdef WIN32
    s.Append("%s=%I64d(%u)\n", pszName, t, sizeof(t));
#else
    s.Append("%s=%lld(%u)\n", pszName, t, sizeof(t));
#endif
    return s.m_iRet;
}

template<> inline
int CodeConvert<update_dump>(CTCodeStream& s, uint64& t, const char* pszName, const update_dump& m)
{
    CT_UPDATE_DUMP(s);

#ifdef WIN32
    s.Append("%s=%I64u(%u)\n", pszName, t, sizeof(t));
#else
    s.Append("%s=%llu(%u)\n", pszName, t, sizeof(t));
#endif
    return s.m_iRet;
}

template<> inline
int CodeConvertBegin<update_dump>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const update_dump& m)
{
    t.__max__ = s.m_ushBufSize;
    s.Append("%s = { ", pszName);
    return 0;
}

template<> inline
int CodeConvertEnd<update_dump>(CTCodeStream& s, CTCodeObject& t, const char* pszName, const update_dump& m)
{
    s.Append("__ver__ = %hu };\n", t.__ver__);
    return 0;
}

template<> inline
int CodeConvertVersion<update_dump>(CTCodeObject& t, unsigned short ushVersion, const update_dump& m)
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
int CodeConvertArrayBegin<update_dump>(CTCodeStream& s, int& t, const char* pszName, const update_dump& m)
{
    CodeConvert<dump>(s, t, pszName, dump());
    return s.m_iRet;
}

template<> inline
int CodeConvertArrayEnd<update_dump>(CTCodeStream& s, int& t, const char* pszName, const update_dump& m)
{
    return s.m_iRet;
}

} //namespace CTLib

#endif //__CTLIB_CODECONVERT2_H__
