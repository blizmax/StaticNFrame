// -------------------------------------------------------------------------
//    @FileName         :    NFBuffer.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFPlatform.h"

/**
* @brief 线程锁封装,参考google protobuf
*
*/
class _NFExport NFMutex
{
public:
	/**
	* @brief 构造函数.
	*
	*/
	NFMutex();

	/**
	* @brief 析构函数.
	*
	*/
	virtual ~NFMutex();

	/**
	* @brief 锁住线程， 如果有必要直到被解锁，不然会一直堵塞
	* Block if necessary until this Mutex is free, then acquire it exclusively.
	*/
	void Lock();

	/**
	* @brief 解锁 Release this Mutex.  Caller must hold it exclusively.
	*
	*/
	void Unlock();

	/**
	* @brief 如果线程不能完全持有这个锁，会导致崩溃
	* Crash if this Mutex is not held exclusively by this thread.
	* May fail to crash when it should; will never crash when it should not
	*
	*/
	void AssertHeld();
private:
	/**
	* @brief 内部结构
	*
	*/
	struct NFInternal;
	NFInternal* mInternal;

	/**
	* @brief 禁止复制
	*
	*/
	NF_DISALLOW_EVIL_CONSTRUCTORS(NFMutex);
};

/**
* @brief 自动释放的线程锁封装,参考google protobuf
*
*/
class _NFExport NFMutexLock
{
public:
	/**
	* @brief 构造函数.
	*
	*/
	explicit NFMutexLock(NFMutex* mu) : mu_(mu)
	{
		this->mu_->Lock();
	}

	/**
	* @brief 析构函数.
	*
	*/
	~NFMutexLock()
	{
		this->mu_->Unlock();
	}

private:
	NFMutex* const mu_;
	/**
	* @brief 禁止复制
	*
	*/
	NF_DISALLOW_EVIL_CONSTRUCTORS(NFMutexLock);
};

