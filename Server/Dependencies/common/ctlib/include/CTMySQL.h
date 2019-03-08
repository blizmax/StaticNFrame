/**
* MySQL类
*/

#ifndef __CTLIB_MYSQL_H__
#define __CTLIB_MYSQL_H__

#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include "CTNull.h"
#else
#include <mysql.h>
#endif
#include "CTBase.h"

namespace CTLib
{

/**
* 字段类型定义, 参见CTMySQL::GetFieldType
*/
enum
{
    MYSQL_NULL = 0,
    MYSQL_TINY = 1,
    MYSQL_SHORT = 2,
    MYSQL_LONG = 4,
    MYSQL_LONGLONG = 8,
    MYSQL_VARIANT = 9,
    MYSQL_BLOB = 10,
};

class CTMySQL
{
public:
    /**
    * param[in] bUseFieldName 能否用字段名来获取字段的值
    */
    CTMySQL()
    {
        mysql_init (&m_connection);
        m_bConnected = false;
        m_uiFields = 0;
        m_uiRows = 0;
        m_result = NULL;
        m_fields = NULL;
        m_lengths= NULL;
        memset(m_szHost, 0, sizeof(m_szHost));
        memset(m_szUser, 0, sizeof(m_szUser));
        memset(m_szPass, 0, sizeof(m_szPass));
        memset(m_szSocket, 0, sizeof(m_szSocket));
        m_ushPort = 0;
        m_bFieldIndexInitialized = false;
        m_bUseFieldName = true;
    }

    virtual ~CTMySQL()
    {
        Close();		
    }

public:
    void Connect(const char* pszHost, const char* pszUser, const char* pszPass,
        unsigned short ushPort = 0, const char* pszSocket = NULL,
        bool bUseFieldName = true)
    {
        if (strcmp(pszHost, m_szHost) == 0)
        {
            return Connect();
        }

        ct_snprintf(CT_STRSIZE(m_szHost), "%s", CT_NOTNULL(pszHost));
        ct_snprintf(CT_STRSIZE(m_szUser), "%s", CT_NOTNULL(pszUser));
        ct_snprintf(CT_STRSIZE(m_szPass), "%s", CT_NOTNULL(pszPass));
        ct_snprintf(CT_STRSIZE(m_szSocket), "%s", CT_NOTNULL(pszSocket));
        m_ushPort = ushPort;
        m_bUseFieldName = bUseFieldName;
        Close();

        Connect();		
    }

    void Connect()
    {
        if (!m_bConnected)
        {
            if (m_szHost[0] == '\0' && m_szSocket[0] == '\0')
            {
                ct_snprintf(CT_STRSIZE(m_szErrMsg), "连接数据库失败:缺少IP地址");
                CT_THROW((m_szErrMsg));
            }

            //mysql自动重连
            /*
            Note: mysql_real_connect() incorrectly reset the MYSQL_OPT_RECONNECT option to its default value 
            before MySQL 5.0.19. Therefore, prior to that version, if you want reconnect to be enabled for each connection, 
            you must call mysql_options() with the MYSQL_OPT_RECONNECT option after each call to mysql_real_connect(). 
            This is not necessary as of 5.0.19: Call mysql_options() only before mysql_real_connect() as usual. 
            当前使用的数据库版本为5.0.18
            */
            char cReconnect = 1;
            mysql_options(&m_connection, MYSQL_OPT_RECONNECT, (char *)&cReconnect);
            mysql_options(&m_connection, MYSQL_SET_CHARSET_NAME, "utf8");
            // 连接不成功抛出异常
            if (mysql_real_connect(&m_connection, m_szHost, m_szUser, m_szPass,
                    NULL, m_ushPort, m_szSocket, 0) == NULL)
            {
                ct_snprintf(CT_STRSIZE(m_szErrMsg), "连接数据库失败:%u(%s)", GetErrorNo(), GetErrorMsg());
                CT_THROW((GetErrorNo(), m_szErrMsg));
            }
            mysql_options(&m_connection, MYSQL_OPT_RECONNECT, (char *)&cReconnect);
            m_bConnected = true;
        }
        else
        {
            mysql_ping(&m_connection);
        }
    }

    void Close()
    {
        if (m_bConnected)
        {
            FreeResult();
            mysql_close(&m_connection);
        }
    }

    void SelectDB(const char* szDBName)
    {
        if (mysql_select_db(&m_connection, szDBName) != 0)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "选择数据库失败:%u(%s)", GetErrorNo(), GetErrorMsg());
            CT_THROW((GetErrorNo(), m_szErrMsg));
        }
    }

    void Query(const char* szSqlString)
    {
        Connect();
        FreeResult();	// 每次执行SQL都释放上次的结果集
        if (mysql_real_query(&m_connection, szSqlString, strlen(szSqlString)) != 0)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "查询数据库失败:%u(%s)", GetErrorNo(), GetErrorMsg());
            CT_THROW((GetErrorNo(), m_szErrMsg));
        }
    }

    void Query(ICodeRaw& stSql)
    {
        Connect();
        FreeResult();	// 每次执行SQL都释放上次的结果集
        if (mysql_real_query(&m_connection, stSql.GetBuffer(), stSql.GetDataLen()) != 0)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "查询数据库失败:%u(%s)", GetErrorNo(), GetErrorMsg());
            CT_THROW((GetErrorNo(), m_szErrMsg));
        }
    }

    void Query(const string& sql)
    {
        Query(sql.c_str());
    }

    // 返回NULL表示到了最后一条记录
    char** FetchRow()
    {
        if (m_result == NULL)
        {
            StoreResult();
        }

        if ((m_row = mysql_fetch_row(m_result)) == NULL && GetErrorNo() != 0)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "取记录失败:%u(%s)", GetErrorNo(), GetErrorMsg());
            CT_THROW((GetErrorNo(), m_szErrMsg));
        }

        m_lengths = mysql_fetch_lengths(m_result);
        return m_row;
    }

    // 定位到某行
    void DataSeek(unsigned int iRow)
    {
        if (m_result == NULL)
        {
            StoreResult();
        }

        if (iRow > GetRowCount()-1)
        {
            CT_THROW(("超出了记录集的范围"));
        }

        mysql_data_seek(m_result, iRow);
    }

    // 返回在结果集合中行的数量
    // 只对SELECT有效, 对UPDATE/INSERT/DELETE操作调用该方法会出错
    unsigned int GetRowCount()
    {
        if (m_result == NULL)
        {
            StoreResult();
        }
        return m_uiRows;
    }

    // 返回在结果集合中字段(列)的数量
    // 只对SELECT有效, 对UPDATE/INSERT/DELETE操作调用该方法会出错
    unsigned int GetFieldCount()
    {
        if (m_result == NULL)
        {
            StoreResult();
        }
        return m_uiFields;
    }

    // 返回受到影响的记录数
    // 对UPDATE/INSERT/DELETE操作有效
    unsigned int GetAffectedRows()
    {
        if (!m_bConnected)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取影响的记录数失败:没有连接到数据库");
            CT_THROW((m_szErrMsg));
        }

        return (unsigned int) mysql_affected_rows(&m_connection);
    }

    MYSQL* GetConnectHandle()
    {
        return &m_connection; 
    }

    // 返回有前一个查询为一个AUTO_INCREMENT列生成的ID
    unsigned int GetInsertId()
    {
        return (unsigned int) mysql_insert_id(&m_connection);
    }

    bool IsConnected() const
    {
        return m_bConnected;
    }

    unsigned int GetErrorNo()
    {
        return mysql_errno(&m_connection);
    }

    char* GetErrorMsg()
    {
        return (char*) mysql_error(&m_connection);
    }

public:
    // 
    // 本段中的这些函数只对SELECT有效. 使用这些函数前, 需要FetchRow()
    // 并且在FetchRow() != NULL 时才能使用这些函数.
    //
    bool IsNull(unsigned int iField)
    {
        CheckRowExist(iField);	
        return (m_row[iField] == NULL);
    }

    bool IsNull(const string& szFieldName)
    {
        return IsNull(GetFieldIndex(szFieldName));
    }

    // 如果字段值为NULL, 返回NULL
    // 需要调用者自己处理
    char* GetField(unsigned int iField)
    {
        CheckRowExist(iField);
        return m_row[iField];
    }

    char* GetField(const string& szFieldName)
    {
        return GetField(GetFieldIndex(szFieldName));
    }

    unsigned long GetFieldLength(unsigned int iField)
    {
        CheckRowExist(iField);
        return m_lengths[iField];
    }

    unsigned long GetFieldLength(const string& szFieldName)
    {
        return GetFieldLength(GetFieldIndex(szFieldName));
    }

    const char* GetFieldName(unsigned int iField)
    {
        CheckRowExist(iField);
        return m_fields[iField].name;
    }

    // 如果字段值为NULL, 返回0
    int GetInt(unsigned int iField)
    {
        CheckRowExist(iField);

        // 字段类型不匹配
        // mysql5.0中新增加了FIELD_TYPE_NEWDECIMAL(246)这种类型, SUM字段就是这种类型
        // 但我们的client是mysql4.x版本, IS_NUM中没有判断246, 所以自己加上
        if (!(IS_NUM(m_fields[iField].type)) && m_fields[iField].type != 246)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取字段数据失败:字段类型不匹配(%u)(index=%d)", m_fields[iField].type, iField);
            CT_THROW((m_szErrMsg));
        }

        if (m_row[iField] != NULL)
        {
            return atoi(m_row[iField]);
        }
        else
        {
            return 0;	// 如果字段为NULL, 返回0
        }
    }

    int GetInt(const string& szFieldName)
    {
        return GetInt(GetFieldIndex(szFieldName));
    }

    /**
    * 返回字段的类型, 类型值参照...
    */
    char GetFieldType(int iFieldIndex)
    {
        CT_TRACE(("GetFieldType %d", m_fields[iFieldIndex].type));
        switch (m_fields[iFieldIndex].type)
        {
        case MYSQL_TYPE_NULL: return MYSQL_NULL;
        case MYSQL_TYPE_TINY: return MYSQL_TINY;
        case MYSQL_TYPE_SHORT: return MYSQL_SHORT;
        case MYSQL_TYPE_LONG: return MYSQL_LONG;
        case MYSQL_TYPE_LONGLONG: return MYSQL_LONGLONG;
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB: return MYSQL_BLOB;
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
            return MYSQL_VARIANT;
        default:
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "不支持的字段类型%d", m_fields[iFieldIndex].type);
            CT_THROW((m_szErrMsg));
        }
    }

protected:
    void StoreResult()
    {
        FreeResult();
        m_result = mysql_store_result(&m_connection);
        if (m_result == NULL)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "保存结果失败:%u(%s)", GetErrorNo(), GetErrorMsg());
            CT_THROW((GetErrorNo(), m_szErrMsg));
        }
        m_uiFields = mysql_num_fields(m_result);
        m_uiRows   = (unsigned int)(mysql_num_rows(m_result) & 0xFFFFFFFF);
        m_fields  = mysql_fetch_fields(m_result);

        if (m_bUseFieldName && !m_bFieldIndexInitialized)
        {
            for(unsigned int i = 0; i < m_uiFields; ++i)
            {
                // 把字段名转换为小写，中文字符不受影响
                string sFieldName(m_fields[i].name);
                CTStringUtils::MakeLower(sFieldName);
                m_FieldIndex[sFieldName] = i;
            }
            m_bFieldIndexInitialized = true;
        }
    }

    // 可以释放UPDATE/INSERT/DELETE语句而不出错
    void FreeResult()
    {
        if (m_result != NULL)
        {
            mysql_free_result(m_result);
        }

        m_uiFields = 0;
        m_uiRows = 0;
        m_fields = NULL;
        m_result = NULL;

        if (m_bUseFieldName && m_bFieldIndexInitialized)
        {
            m_FieldIndex.erase(m_FieldIndex.begin(), m_FieldIndex.end());
            m_bFieldIndexInitialized = false;
        }
    }

    void CheckRowExist(unsigned int iField)
    {
        // 没有FetchRow()
        if (m_result == NULL)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取字段数据失败:没有FetchRow()(index=%d)", iField);
            CT_THROW((m_szErrMsg));
        }
        // 记录集为空
        if (m_row == NULL)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取字段数据失败:记录集为空(index=%d)", iField);
            CT_THROW((m_szErrMsg));
        }
        // 下标溢出
        if (iField > m_uiFields)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取字段数据失败:下标超出范围(index=%d)", iField);
            CT_THROW((m_szErrMsg));
        }
    }

    unsigned int GetFieldIndex(const string& szFieldName)
    {
        if (szFieldName.size() == 0)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取字段下标失败:字段名是空指针");
            CT_THROW((m_szErrMsg));
        }

        if (!m_bUseFieldName)
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "初始化时指定了不能用字段名");
            CT_THROW((m_szErrMsg));
        }

        // 把字段名转换为小写，中文字符不受影响
        string sFieldName(szFieldName);
        CTStringUtils::MakeLower(sFieldName);
        map<string, unsigned int>::iterator it;
        it = m_FieldIndex.find(string(sFieldName));
        if (it == m_FieldIndex.end())
        {
            ct_snprintf(CT_STRSIZE(m_szErrMsg), "获取字段下标失败:字段名%s不存在", szFieldName.c_str());
            CT_THROW((m_szErrMsg));
        }
        return (*it).second;
    }

protected:
    char m_szErrMsg[1024];

    char m_szHost[CT_PATH_MAX];	    ///< 数据库主机名
    char m_szUser[CT_PATH_MAX];	    ///< 数据库用户名
    char m_szPass[CT_PATH_MAX];	    ///< 数据库用户密码
    char m_szSocket[CT_PATH_MAX];   ///< 数据库socket文件(适用于linux)
    unsigned short m_ushPort;       ///< 数据库端口

    unsigned int  m_uiFields;
    unsigned int  m_uiRows;

    MYSQL 		m_connection;
    MYSQL_RES*  m_result;
    MYSQL_FIELD* m_fields;
    unsigned long * m_lengths; // 数组指针,指向结果集合的当前行的各个字段数据的长度
    MYSQL_ROW   m_row;

    map<string, unsigned int> m_FieldIndex;
    bool m_bFieldIndexInitialized;
    bool m_bUseFieldName;
    bool m_bConnected;
};

} //namespace CTLib

#endif //__CTLIB_MYSQL_H__
