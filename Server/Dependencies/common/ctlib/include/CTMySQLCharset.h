#ifndef __CTLIB_MYSQL_CHARSET__
#define __CTLIB_MYSQL_CHARSET__

/**
* escape_string_for_mysql中需要charset_info, 默认用utf8
* 为避免反复生成并初始化, 用这个类包装并做成单件
*/
struct CHARSET_INFO;
class CTMySQLCharset
{
private:
    CHARSET_INFO* m_pstCharset;

public:
    CTMySQLCharset();

    /** 设定字符集 */
    int InitCharset(const char* pszCharsetName = "utf8");

    /** 转义 */
    int EscapeString(char* pszDst, unsigned int uiDstLen, const char* pszSrc, unsigned int uiSrcLen);

    CHARSET_INFO* GetCharset() const;
};

#endif

