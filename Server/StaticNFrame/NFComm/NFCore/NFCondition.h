// -------------------------------------------------------------------------
//    @FileName         :    NFContidion.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"
#include "NFMutex.h"
#include "NFLock.h"
#include <thread>
#include <condition_variable>

/**
Portable condition variable synchronization primitive, sometimes also called a monitor.
*/
class _NFExport NFCondition
{
public:

	/**
	Constructor.
	*/
	inline NFCondition()
	{

	}

	/**
	Destructor.
	\note If the condition is destroyed while threads are still waiting on it, the result is undefined.
	*/
	inline ~NFCondition()
	{

	}

	/**
	Returns a reference to the Mutex owned by the monitor.
	*/
	NF_FORCEINLINE NFMutex &GetMutex()
	{
		return mMutex;
	}

	/**
	Suspends the calling thread until it is woken by another thread calling \ref Pulse or \ref PulseAll.
	\note The calling thread must hold a lock on the mutex associated with the condition.
	The lock owned by the caller is released, and automatically regained when the thread is woken.
	*/
	NF_FORCEINLINE void Wait(NFLock &lock)
	{
		// The lock must be locked before calling Wait().
		NF_ASSERT(lock.mLock.owns_lock());
		mCondition.wait(lock.mLock);
	}

	/**
	Wakes a single thread that is suspended after having called \ref Wait.
	*/
	NF_FORCEINLINE void Pulse()
	{
		mCondition.notify_one();
	}

	/**
	Wakes all threads that are suspended after having called \ref Wait.
	*/
	NF_FORCEINLINE void PulseAll()
	{
		mCondition.notify_all();
	}

private:

	NFCondition(const NFCondition &other);
	NFCondition &operator=(const NFCondition &other);

	NFMutex mMutex;
	std::condition_variable mCondition;
};
