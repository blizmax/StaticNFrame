// -------------------------------------------------------------------------
//    @FileName         :    NFSpinLock.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"
#include "NFMutex.h"
#include "NFAtomic.h"
#include "NFThreadUtils.h"
#include <thread>

/**
A simple spinlock thread synchronization primitive.
*/
class _NFExport NFSpinLock
{
public:

	/**
	Default constructor.
	*/
	NF_FORCEINLINE NFSpinLock()
	{
		mValue.Store(0);
	}

	/**
	Destructor.
	*/
	NF_FORCEINLINE ~NFSpinLock()
	{

	}

	/**
	Locks the spinlock.
	Locking the spinlock guarantees exclusive access to a protected resource associated with it.
	\note The calling thread will busy-wait and hence this method should be used with care.
	*/
	NF_FORCEINLINE void Lock()
	{
		uint32_t backoff(0);
		while (true)
		{
			uint32_t currentValue(UNLOCKED);
			if (mValue.CompareExchangeAcquire(currentValue, LOCKED))
			{
				return;
			}

			NFThreadUtils::Backoff(backoff);
		}
	}

	/**
	Unlocks the spinlock.
	Unlocking the spinlock releases exclusive access previously acquired with Lock.
	*/
	NF_FORCEINLINE void Unlock()
	{
		NF_ASSERT(mValue.Load() == LOCKED);
		mValue.Store(UNLOCKED);
	}

private:

	NFSpinLock(const NFSpinLock &other);
	NFSpinLock &operator=(const NFSpinLock &other);

	static const uint32_t UNLOCKED = 0;
	static const uint32_t LOCKED = 1;

	NFAtomic mValue;
};