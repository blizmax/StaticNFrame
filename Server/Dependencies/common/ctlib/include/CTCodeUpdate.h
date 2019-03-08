/**
* Update模版类.
*/

#ifndef __CTLIB_CODEUPDATE_H__
#define __CTLIB_CODEUPDATE_H__

#include "CTBase.h"
#include "CTBitset.h"
#include "CTCodeConvert.h"
#include "CTCodeConvert2.h"

namespace CTLib
{

/** 使用CTCodeUpdate时，用这个宏来设置字段的值 */
#define CT_UPDATE_SET(CMD, FIELD) CMD.Object(&CMD.m_stObject.FIELD).FIELD

/** 使用CTCodeUpdate时，用这个宏来获取字段的值 */
#define CT_UPDATE_GET(CMD, FIELD) CMD.m_stObject.FIELD

/** 使用CTCodeUpdate时，用这个宏来判断字段是否有被设置 */
#define CT_UPDATE_TEST(CMD, FIELD) CMD.m_stBitset.test((size_t)&CMD.m_stObject.FIELD - (size_t)&CMD.m_stObject)

/**
* Update模版类
*/
template<typename T>
class CTCodeUpdate: public CTCodeObject
{
public:
    /**
    * 更改/删除操作类型定义
    */
    enum tagEnumUpdateType
    {
        EUT_NULL = 0,
        EUT_INSERT = 1,		// 更新，如果记录已存在就报错
        EUT_UPDATE = 2,		// 更新，如果记录不存在就报错，注意和EUT_INC的区别
        EUT_SET = 3,		// 更新，不管记录是否存在，都能成功
        EUT_INC = 4,        // 增量更新，如果记录不存在就报错
        EUT_DELETE = 5,		// 删除，如果记录不存在就报错
        EUT_ERASE = 6,		// 删除，不管记录是否存在，都能成功
    };

    CTCodeUpdate()
    {
        m_stBitset.reset();
        m_ucUpdateType = EUT_NULL;
    }

public:
    CTBitset<sizeof(T)> m_stBitset;
    unsigned char       m_ucUpdateType;
    T                   m_stObject;

public:
    T& Object(void* pField)
    {
        // 这里在VS2005下编译时，如果打开了“检测64位可一致性问题”就会告警
        int i = (int)(pField) - (int)(&m_stObject);
        m_stBitset.set(i);
        return m_stObject;
    }
};


template<typename T> inline
int EncodeUpdate(CTCodeStream& s, CTCodeUpdate<T>& t, const char* pszName)
{
    CodeConvertBegin<bin_encode>(s, t, pszName);

    // 编码UpdateType
    CodeConvert<bin_encode>(s, t.m_ucUpdateType, CT_NAME(m_ucUpdateType));

    // 编码对象
    s.m_pReserve1 = &t.m_stBitset;
    s.m_pReserve2 = &t.m_stObject;
    CodeConvert<update_encode>(s, t.m_stObject, CT_NAME(m_stObject));

    CodeConvertEnd<bin_encode>(s, t, pszName);
    return s.m_iRet;
}

template<typename T> inline
int DecodeUpdate(CTCodeStream& s, CTCodeUpdate<T>& t, const char* pszName)
{
    CodeConvertBegin<bin_decode>(s, t, pszName);

    // 解码UpdateType
    CodeConvert<bin_decode>(s, t.m_ucUpdateType, CT_NAME(m_ucUpdateType));

    // 解码对象
    s.m_pReserve1 = &t.m_stBitset;
    s.m_pReserve2 = &t.m_stObject;
    CodeConvert<update_decode>(s, t.m_stObject, CT_NAME(m_stObject));

    CodeConvertEnd<bin_decode>(s, t, pszName);
    return s.m_iRet;
}

template<typename T> inline
int DumpUpdate(CTCodeStream& s, CTCodeUpdate<T>& t, const char* pszName)
{
    CodeConvertBegin<dump>(s, t, pszName);

    // 输出UpdateType
    CodeConvert<dump>(s, t.m_ucUpdateType, CT_NAME(m_ucUpdateType));

    // 解码对象
    s.m_pReserve1 = &t.m_stBitset;
    s.m_pReserve2 = &t.m_stObject;
    CodeConvert<update_dump>(s, t.m_stObject, CT_NAME(m_stObject));

    CodeConvertEnd<dump>(s, t, pszName);
    return s.m_iRet;
}

} //namespace CTLib

#endif //__CTLIB_CODEUPDATE_H__
