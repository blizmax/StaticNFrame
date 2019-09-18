// -------------------------------------------------------------------------
//    @FileName         :    NFAtomic.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"
#include <atomic>

class _NFExport NFAtomic
{
public:
	/**
	Constructor.
	*/
	inline NFAtomic() : mValue(0)
	{

	}

	/**
	Explicit constructor that initializes the value.
	*/
	inline explicit NFAtomic(const uint32_t initialValue) : mValue(static_cast<int32_t>(initialValue))
	{

	}

	/**
	Destructor.
	*/
	inline ~NFAtomic()
	{

	}

	/**
	Atomic compare-and-exchange with 'acquire' memory ordering semantics.
	*/
	NF_FORCEINLINE bool CompareExchangeAcquire(uint32_t &currentValue, const uint32_t newValue)
	{
		return mValue.compare_exchange_weak(
			currentValue,
			newValue,
			std::memory_order_acquire);
	}

	/**
	Atomic compare-and-exchange with 'release' memory ordering semantics.
	*/
	NF_FORCEINLINE bool CompareExchangeRelease(uint32_t &currentValue, const uint32_t newValue)
	{
		return mValue.compare_exchange_weak(
			currentValue,
			newValue,
			std::memory_order_release);
	}

	/**
	Atomic increment.
	*/
	NF_FORCEINLINE void Increment()
	{
		++mValue;
	}

	/**
	Atomic decrement.
	*/
	NF_FORCEINLINE void Decrement()
	{
		--mValue;
	}

	/**
	Atomically get the current value.
	*/
	NF_FORCEINLINE volatile uint32_t Load() const
	{
		return mValue.load();
	}

	/**
	Atomically set the current value.
	*/
	NF_FORCEINLINE void Store(const uint32_t val)
	{
		mValue.store(val);
	}
private:

	NFAtomic(const NFAtomic &other);
	NFAtomic &operator=(const NFAtomic &other);

	volatile std::atomic_uint_least32_t mValue;
};
