/**
* 基本定义.
*
*/

#ifndef __CTLIB_BASEDEFINE_H__
#define __CTLIB_BASEDEFINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>

// 虽然建议不要在头文件中使用using namespace，但std太常用到了，这里就把它using了
using namespace std;

#ifdef WIN32
    #pragma warning(disable: 4503)  // 屏蔽关于超出修饰名的长度，名称被截断的WARNING
    #pragma warning(disable: 4786)  // 屏蔽关于函数名超过256的WARNING
    #pragma warning(disable: 4127)  // 屏蔽关于条件表达式是常量的WARNING
    #pragma warning(disable: 4996)  // 屏蔽关于被声明为否决的_CRT_SECURE_NO_DEPRECATE的WARNING
    #pragma warning(disable: 4100)  // 屏蔽关于未引用的形参的WARNING

    #ifndef _WINSOCKAPI_
        #define _WINSOCKAPI_
    #endif
    #include <Windows.h>
    #include <WinSock2.h>
    #include <io.h>
#else
    #include <fcntl.h>
    #include <ctype.h>
    #include <unistd.h>
    #include <sys/time.h>
#endif

#if __WORDSIZE == 64
#define CT_BIT64
#else
#define CT_BIT32
#endif

typedef void* CT_POINT;

#define CT_STRSIZE(x)	x,sizeof(x)					///< 用在snprintf中的简化宏
#define CT_COUNTOF(x)	(sizeof(x)/sizeof(x[0]))
#define CT_NOTNULL(x)   (x == NULL ? "" : x)

#define CT_MAX(a, b)	((a) > (b) ? (a) : (b))
#define CT_MIN(a, b)	((a) < (b) ? (a) : (b))
#define CT_CEIL(a, b) ((a) == 0 ? 0 : ((a) - 1) / (b) + 1)

#define CT_UNUSED(x)    // 如果有些参数在函数类没用到，用这个可以避免警告
#define CT_NAME(x)  #x

// 从windef.h拿来的宏
// 2个unsigned char和unsignd short的转换
#define CT_USHORT_MAKE(high, low)   ((unsigned short)(((unsigned char)((low) & 0xFF)) | ((unsigned short)((unsigned char)((high) & 0xFF))) << 8))
#define CT_USHORT_LOW(i)            ((unsigned char)((unsigned short)(i) & 0xFF))
#define CT_USHORT_HIGH(i)           ((unsigned char)((unsigned short)(i) >> 8))

// 2个unsigned short和unsigned int的转换
#define CT_UINT_MAKE(high, low)   ((unsigned int)(((unsigned short)((low) & 0xFFFF)) | ((unsigned int)((unsigned short)((high) & 0xFFFF))) << 16))
#define CT_UINT_LOW(i)            ((unsigned short)((unsigned int)(i) & 0xFFFF))
#define CT_UINT_HIGH(i)           ((unsigned short)((unsigned int)(i) >> 16))

// 2个unsigned int和uint64的转换
#define CT_UINT64_MAKE(high, low)   ((uint64)(((unsigned int)((low) & 0xFFFFFFFF)) | ((uint64)((unsigned int)((high) & 0xFFFFFFFF))) << 32))
#define CT_UINT64_LOW(i)            ((unsigned int)((uint64)(i) & 0xFFFFFFFF))
#define CT_UINT64_HIGH(i)           ((unsigned int)((uint64)(i) >> 32))

// 32位的位操作
// pos的取值范围是0-31
#define CT_BIT_SET(i, pos)      (i |= ((unsigned int)1 << pos))        // 把第pos位置1
#define CT_BIT_CLR(i, pos)      (i &= ~((unsigned int)1 << pos))       // 把第pos位置0
#define CT_BIT_TEST(i, pos)     ((i & ((unsigned int)1 << pos)) != 0)  // 判断第pos是不是1

#define CHECK_RETURN(iRet)  do { if (iRet) { return iRet; } } while(0)
#define CHECK_EXIT(iRet) do { if (iRet) { exit(iRet); } } while(0)
#define CHECK_THROW(iRet) do { if (iRet) { CT_THROW((iRet)); } } while(0)

namespace CTLib
{

/**
* 常量定义
*/
enum
{
    CT_PATH_MAX = 1024,       ///< 路径的最大长度
    CT_INT_MAX = 0x7FFFFFFF, ///< int类型的最大正数
    CT_CONF_TXT_MAX = 1024,  ///< 最大文本配置长度

};

} // namespace CTLib


// 与i18n有关的宏定义
// 要翻译成其他语言的字符串，请用_("")括起来，以便gettext工具能直接从代码中抽取出来
#ifndef _
    #ifdef CT_GETTEXT
        #define _(x) gettext(x)
    #else
        #define _(x) x
    #endif
#endif

#ifdef WIN32
    typedef unsigned __int64 uint64;
    typedef __int64 int64;
    typedef int socklen_t;
    typedef unsigned int pid_t;

    // Windows socket的错误码和含义和linux不一样，通过宏统一
    // 而是返回WSAEWOULDBLOCK
    #define CT_ERRNO			GetLastError()
    #define CT_WSA_ERRNO		WSAGetLastError()
    #define CT_EWOULDBLOCK      WSAEWOULDBLOCK	// 10035
    #define CT_EINPROGRESS      WSAEINPROGRESS	// 10036
    #define CT_ETIME            WSAETIMEDOUT	// 10060
    #define CT_EINTR            WSAEINTR		// 10000
    #define	CT_ECONNRESET       WSAECONNRESET	// 10054, 用来表示远程关闭了连接
    #define GetNowProcessId GetCurrentProcessId 
#else   //not WIN32
    #define closesocket			close
    #define GetNowProcessId	getpid
    #define GetCurrentThreadId	pthread_self
#ifdef CT_BIT64
    typedef long int64;
    typedef unsigned long uint64;
#else
    typedef long long int64;
    typedef unsigned long long uint64;
#endif

#define INT64_1 ((int64)1)

    typedef int SOCKET;

#ifndef INVALID_SOCKET
    #define INVALID_SOCKET		(SOCKET)(~0)
#endif

    #define CT_ERRNO            errno
    #define CT_WSA_ERRNO        errno
    #define CT_EWOULDBLOCK      EWOULDBLOCK
    #define CT_EINPROGRESS      EINPROGRESS
    #define CT_ETIME            ETIME
    #define CT_EINTR            EINTR
    #define	CT_ECONNRESET       ECONNRESET
#endif //end WIN32

// 屏蔽Windows和Linux的这些函数差异
#ifdef WIN32
    #define strcasecmp          _stricmp
    #define strncasecmp		    _strnicmp
    #define wcscasecmp          _wcsicmp
    #define wcsncasecmp		    _wcsnicmp
    #define strtoll             _strtoi64
    #define strtoull            _strtoui64
    #define wcstoll             _wcstoi64
    #define wcstoull            _wcstoui64

    #define sleep               CTLib::CTPlatform::_ct_sleep
    #define usleep              CTLib::CTPlatform::_ct_usleep

    #define ct_fopen            CTLib::CTPlatform::_ct_fopen
    #define ct_stat             _stat32
    #define ct_access           _access
    #define ct_rename           rename
    #define ct_unlink           _unlink
    #define ct_remove           remove
#else
    #define ct_fopen            fopen
    #define ct_stat             stat
    #define ct_access           access
    #define ct_rename           rename
    #define ct_unlink           unlink
    #define ct_remove           remove
#endif

// windows和linux的snprintf的返回值不一样，通过这种方式屏蔽
#define ct_snprintf         CTLib::CTPlatform::_ct_snprintf
#define ct_vsnprintf		CTLib::CTPlatform::_ct_vsnprintf
#define ct_snwprintf        CTLib::CTPlatform::_ct_snwprintf
#define ct_vsnwprintf		CTLib::CTPlatform::_ct_vsnwprintf

#endif //__CTLIB_BASEDEFINE_H__
