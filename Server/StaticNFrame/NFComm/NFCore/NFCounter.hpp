// -------------------------------------------------------------------------
//    @FileName         :    NFCounter.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include "NFNonCopyable.h"
#include "NFMutex.h"

/**
 *@brief  线程安全的计数器.
 */
class NFCounter : public NFNonCopyable
{
public:
	/**
	* @brief 构造函数.
	*
	*/
	NFCounter() : mValue(0)  { }

	/**
	* @brief 返回计数，线程安全
	*
	* @return 返回计数
	*/
	int64_t Value() const
	{
		NFMutexLock lock(&mMutex);
		return mValue;

	}

	/**
	* @brief 获得当前计数，并加1
	*
	* @return    获得当前计数 
	*/
	int64_t GetAndIncrease()
	{
		NFMutexLock lock(&mMutex);
		int64_t ret = mValue++;
		return ret;
	}
private:
	/**
	 *@brief  计数.
	 */
	int64_t mValue;
	/**
	 *@brief  线程锁.
	 */
	mutable NFMutex mMutex;
};
