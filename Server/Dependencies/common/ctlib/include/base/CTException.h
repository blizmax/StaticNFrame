/**
* 异常类定义.
*/

#ifndef __CTLIB_EXCEPTION_H__
#define __CTLIB_EXCEPTION_H__

#include "base/CTBaseDefine.h"
#include "base/CTStringUtils.h"
#ifndef WIN32
    #include <execinfo.h>
#endif

#ifdef CT_NO_EXCEPTION
    #define CT_THROW(x)
#else
    #define CT_THROW(x) throw CTException x
#endif

/**
*  CTLib命名空间
*/
namespace CTLib
{

class CTException
{
public:
    /**
    * 构造/析构
    */
    CTException() : m_iErrorCode(-1)
    {
        DumpBackTrace();
    }

    CTException(int iErrorCode) : m_iErrorCode(iErrorCode)
    {
        DumpBackTrace();
    }

    CTException(const char* pszFormat, ...) : m_iErrorCode(-1)
    {
        va_list ap;
        va_start(ap, pszFormat);
        CTStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
        va_end(ap);

        DumpBackTrace();
    }

	CTException(int iErrorCode, const char* pszFormat, ...)
	{
		m_iErrorCode = iErrorCode;

		va_list ap;
		va_start(ap, pszFormat);
		CTStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
		va_end(ap);

        DumpBackTrace();
	}

    virtual ~CTException()
    {
    }

    /**
    * Get系列
    */
	int GetErrorCode() const
	{
		return m_iErrorCode;
	}

	const char* GetErrorMsg() const
	{
		return m_strErrorMsg.c_str();
	}

	const string GetErrorString() const
	{
		return m_strErrorMsg;
	}

    /** 和std::exception一致 */
    virtual const char* what() const
    {
        return GetErrorMsg();
    }

    /**
    * Set系列
    * 感觉没必要Set, 注释掉 thinkry 2008.05.26
    */
    /*void Set(int iErrorCode, const char* pszFormat, ...)
    {
        m_iErrorCode = iErrorCode;

        va_list ap;
        va_start(ap, pszFormat);
        CTStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
        va_end(ap);
    }

    void SetErrorCode(int iErrorCode)
    {
        m_iErrorCode = iErrorCode;
    }

    void SetErrorMsg(const char* pszErrorMsg)
    {
        m_strErrorMsg = pszErrorMsg;
    }

    void SetErrorMsg(int iErrorCode, const char* pszFormat, ...)
    {
        m_iErrorCode = iErrorCode;

        va_list ap;
        va_start(ap, pszFormat);
        CTStringUtils::FormatV(m_strErrorMsg, pszFormat, ap);
        va_end(ap);
    }*/

protected:
    void DumpBackTrace()
    {
        // 编译时如果不加-rdynamic参数, backtrace_symbols只会返回地址信息
        // 可以通过addr2line工具查到地址对应的文件行号
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

        CT_ERROR(("Exception: ErrorCode=%d Msg=%s", m_iErrorCode, m_strErrorMsg.c_str()));
        CT_ERROR(("%s", sBackTrace.c_str()));
#endif
    }

protected:
	int			m_iErrorCode;
	string	    m_strErrorMsg;
};

// 异常类由于有堆栈跟踪, 不适用作为错误值类
// 注释掉 thinkry 2008.05.26
//typedef CTException CTError;

} // namespace CTLib

#endif //__CTLIB_EXCEPTION_H__
