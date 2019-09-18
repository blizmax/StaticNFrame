// -------------------------------------------------------------------------
//    @FileName         :    NFMutex.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"
#include <thread>
#include <mutex>

/**
Portable mutex synchronization primitive.
*/
class _NFExport NFMutex
{
public:

	friend class NFCondition;
	friend class NFLock;

	/**
	Default constructor.
	*/
	NF_FORCEINLINE NFMutex()
	{
	}

	/**
	Destructor.
	*/
	NF_FORCEINLINE ~NFMutex()
	{

	}

	/**
	Locks the mutex, guaranteeing exclusive access to a protected resource associated with it.
	\note This is a blocking call and should be used with care to avoid deadlocks.
	*/
	NF_FORCEINLINE void Lock()
	{
		mMutex.lock();
	}

	/**
	Unlocks the mutex, releasing exclusive access to a protected resource associated with it.
	*/
	NF_FORCEINLINE void Unlock()
	{
		mMutex.unlock();
	}

private:

	NFMutex(const NFMutex &other);
	NFMutex &operator=(const NFMutex &other);

	std::mutex mMutex;
};

