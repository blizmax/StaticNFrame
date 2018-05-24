#ifndef NF_PLATFORM_H
#define NF_PLATFORM_H

#include "NFMacros.h"
#include "NFPlatformMacros.h"

#include <stdint.h>
#include <chrono>
#include <string>
#include <memory>

#ifndef NDEBUG
#include <assert.h>
#define NF_ASSERT(condition)					if (!(condition)) NFAssertFail(__FILE__, __LINE__); else { }
#define NF_ASSERT_MSG(condition, msg)			if (!(condition)) NFAssertFail(__FILE__, __LINE__, msg); else { }

inline void NFAssertFail(const char*const file, const unsigned int line, const std::string& message)
{
	fprintf(stderr, "FAIL in %s (%d)", file, line);
	if (!message.empty())
	{
		fprintf(stderr, ": %s", message.c_str());
	}

	fprintf(stderr, "\n");
	assert(false);
}
#else
#define NF_ASSERT(condition)
#define NF_ASSERT_MSG(condition, msg_)
#endif

#define NF_CHECK(EXPRESSION) \
  NF_ASSERT_MSG(EXPRESSION, "CHECK failed: " #EXPRESSION)

#define NF_CHECK_EQ(A, B) NF_CHECK((A) == (B))
#define NF_CHECK_NE(A, B) NF_CHECK((A) != (B))
#define NF_CHECK_LT(A, B) NF_CHECK((A) <  (B))
#define NF_CHECK_LE(A, B) NF_CHECK((A) <= (B))
#define NF_CHECK_GT(A, B) NF_CHECK((A) >  (B))
#define NF_CHECK_GE(A, B) NF_CHECK((A) >= (B)

//#define GOOGLE_GLOG_DLL_DECL=

///////////////////////////////////////////////////////////////
#include <string>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include "common/lexical_cast.hpp"

#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#define EPOCHFILETIME 11644473600000000ULL
#else
#include <windows.h>
#include <time.h>
#include <process.h>
#define EPOCHFILETIME 11644473600000000Ui64
#endif

#if NF_PLATFORM == NF_PLATFORM_WIN
#define NFSPRINTF sprintf_s
#define NFSTRICMP stricmp
#define NFSLEEP(s) Sleep(s)
#define NFGetPID() lexical_cast<std::string>(getpid())
typedef unsigned int NF_THREAD_ID;

inline NF_THREAD_ID ThreadId()
{
	return GetCurrentThreadId();
}

inline struct tm* localtime_r(const time_t* timep, struct tm* result)
{
	localtime_s(result, timep);
	return result;
}
#else
#define NFSPRINTF snprintf
#define NFSTRICMP strcasecmp
#define NFSLEEP(s) usleep((s)*1000)
#define NFGetPID() lexical_cast<std::string>(getpid())
typedef pid_t NF_THREAD_ID;
inline NF_THREAD_ID ThreadId()
{
	return pthread_self();
}
#endif

//#ifndef NF_DYNAMIC_PLUGIN
//#define NF_DYNAMIC_PLUGIN
//#endif

//use actor mode--begin
#define NF_ACTOR_THREAD_COUNT 3

#ifndef NF_USE_ACTOR
#define NF_USE_ACTOR
#endif

#ifdef NF_USE_ACTOR

#ifdef NF_DEBUG_MODE
#define THERON_DEBUG 1
#else
#define THERON_DEBUG 0
#endif

#ifndef THERON_CPP11
#define THERON_CPP11 1
#endif

//#ifndef USE_THREAD_CLIENT
//#define USE_THREAD_CLIENT 1
//#endif

#endif
//use actor mode--end

#define GET_CLASS_NAME(className) (#className)

#define NF_SHARE_PTR std::shared_ptr

#define NF_NEW new

#define  NFSafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

#define  NFSafeDelete(pData) { try { delete pData; } catch (...) { NF_ASSERT_MSG(false, "NFSafeDelete error"); } pData=NULL; }

#define  NFSafeDeleteArray(pData) { try { delete [] pData; } catch (...) {  NF_ASSERT_MSG(false, "NFSafeDeleteArray error"); } pData=NULL; }

inline int64_t NFGetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

inline int64_t NFGetSecondTime()
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

inline int64_t NFGetMicroSecondTime()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

inline int64_t NFGetNanoSeccondTime()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

#endif

