// -------------------------------------------------------------------------
//    @FileName         :    NFBuffer.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFMutex.h"

#define THREAD_CPP11	1

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <windows.h>

struct NFMutex::NFInternal
{
	CRITICAL_SECTION mMutex;
#ifndef NDEBUG
	//线程拥有者，只会被在函数AssertHeld中使用
	// Used only to implement AssertHeld().
	DWORD mThreadId;
#endif
};

NFMutex::NFMutex() :mInternal(new NFInternal)
{
	InitializeCriticalSection(&mInternal->mMutex);
}

NFMutex::~NFMutex()
{
	DeleteCriticalSection(&mInternal->mMutex);
	delete mInternal;
}

void NFMutex::Lock()
{
	EnterCriticalSection(&mInternal->mMutex);
#ifndef NDEBUG
	mInternal->mThreadId = GetCurrentThreadId();
#endif
}

void NFMutex::Unlock()
{
#ifndef NDEBUG
	mInternal->mThreadId = 0;
#endif
	LeaveCriticalSection(&mInternal->mMutex);
}

void NFMutex::AssertHeld()
{
#ifndef NDEBUG
	NF_CHECK_EQ(mInternal->mThreadId, GetCurrentThreadId());
#endif
}

#elif THREAD_CPP11
#include <thread>
#include <mutex>
struct NFMutex::NFInternal
{
	std::mutex mMutex;
};

NFMutex::NFMutex() :mInternal(new NFInternal)
{
}

NFMutex::~NFMutex()
{
	delete mInternal;
}

void NFMutex::Lock()
{
	mInternal->mMutex.lock();
}

void NFMutex::Unlock()
{
	mInternal->mMutex.unlock();
}

void NFMutex::AssertHeld()
{

}
#else

#include <pthread.h>

struct NFMutex::NFInternal
{
	pthread_mutex_t mMutex;
};

NFMutex::NFMutex(): mInternal(new NFInternal)
{
	pthread_mutex_init(&mInternal->mMutex, NULL);
}

NFMutex::~NFMutex()
{
	pthread_mutex_destroy(&mInternal->mMutex);
	delete mInternal;
}

void NFMutex::Lock()
{
	int result = pthread_mutex_lock(&mInternal->mMutex);
	if (result != 0)
	{
		NF_ASSERT_MSG(false, "pthread_mutex_lock:" + std::string(strerror(result)));
	}
}

void NFMutex::Unlock()
{
	int result = pthread_mutex_unlock(&mInternal->mMutex);
	if (result != 0)
	{
		NF_ASSERT_MSG(false, "pthread_mutex_unlock:" + std::string(strerror(result)));
	}
}

void NFMutex::AssertHeld() {
  // pthreads dosn't provide a way to check which thread holds the mutex.
  // TODO(kenton):  Maybe keep track of locking thread ID like with WIN32?
}

#endif
