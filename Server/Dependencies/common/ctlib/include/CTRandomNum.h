/*************************************************
Author: will
Date: 10/20/2012
Description:
Notes: 
*************************************************/

#ifndef __CTRandomNum_h__
#define __CTRandomNum_h__

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "base/CTSingleton.h"

namespace CTLib
{

class CTRandomNum
{
public:
    CTRandomNum ()
    {
        m_bInited = false;
    }

    //roll取一个随机数 不包含max
    int GetRand(unsigned int iMax)
    {
        if (iMax == 0)
        {
            return 0;
        }

        if (!m_bInited)
        {
            Init();
        }

        return (unsigned int)rand() % iMax;
    }
    //roll 取一个随机数包涵max
    unsigned int GetRandHaveMax(unsigned int iMax)
    {
        return GetRand(iMax) + 1;
    }
private:
    bool m_bInited;

    void Init()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        srand((unsigned int)(tv.tv_sec + tv.tv_usec));
        m_bInited = true;
    }
};

}

#define CT_RANDOM_NUM  (CTLib::CTSingleton<CTLib::CTRandomNum>::Instance())

#endif // __CTRandomNum_h__

