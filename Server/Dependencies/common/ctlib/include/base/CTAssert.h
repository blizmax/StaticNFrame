/**
* 断言
*/

#ifndef __CTLIB_ASSERT_H__
#define __CTLIB_ASSERT_H__

#include "CTLog.h"

/**
* 断言宏
*/
#ifdef _DEBUG
    #define CT_ASSERT(exp) \
        if(!(exp)) { \
	        CT_FATAL(("assert failed at %s line %d: %s", __FILE__, __LINE__, #exp)); \
	        assert(exp); \
        } else {}
#else
    #define CT_ASSERT(exp)      
#endif

#endif //__CTLIB_ASSERT_H__
