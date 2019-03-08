#ifndef __CTLIB_OBJ_RELOAD_CHECKER_H__
#define __CTLIB_OBJ_RELOAD_CHECKER_H__

#include "base/CTSingleton.h"

namespace CTLib
{

class CObjStat
{
public:
    CObjStat()
        :m_bResetShm(true), m_bCheck(false)
    {

    }

    void SetNotReset() { m_bResetShm = false;};
    void SetReset() { m_bResetShm = true;};
    bool IsReset() { return m_bResetShm;};

    bool GetStat() { return m_bResetShm;}
    void SetStat(bool bResetShm) { m_bResetShm = bResetShm;};

    void ClearCheck() { m_bCheck = false;};
    void SetCheck() {  m_bCheck = true;};
    bool IsCheckOK() { return m_bCheck;};

private:
    bool m_bResetShm;
    bool m_bCheck;
};

#define  OBJ_STAT  (CTSingleton<CObjStat>::Instance())

//////////////////////////////////////////////////////////////////////////


#define OBJ_CHECK_BEGIN(CLASS) \
    static int _OBJCheck()\
    {\
        bool bTemp = OBJ_STAT->GetStat();\
        OBJ_STAT->SetNotReset();\
        OBJ_STAT->ClearCheck();\
        CLASS();\
        OBJ_STAT->SetStat(bTemp);\
        if (!OBJ_STAT->IsCheckOK())\
        {\
            return -1;\
        }\

#define OBJ_CHECK(CLASS) \
        if (CLASS::_OBJCheck() != 0)\
        return -1;\


#define OBJ_CHECK_END() \
        return 0; \
    }\

#define OBJ_CHECK_RELOAD() \
    OBJ_STAT->SetCheck();\
    if (!OBJ_STAT->IsReset()) return;\

}

#endif

