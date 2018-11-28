// -------------------------------------------------------------------------
//    @FileName         :    NFAtomic.h
//    @Author           :    GaoYi
//    @Date             :    2018-11-27
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <atomic>

/////////////////////////////////////////////////
/**
* @file  NFAtomic.h
* @brief  原子计数类.
*/

/**
* @brief 原子操作类,对int做原子操作
*/
class NFAtomic
{
public:

	/**
	* 原子类型
	*/
	typedef int atomic_type;

	/**
	* @brief 构造函数,初始化为0
	*/
	NFAtomic(atomic_type at = 0)
	{
		set(at);
	}

	NFAtomic& operator++()
	{
		inc();
		return *this;
	}

	NFAtomic& operator--()
	{
		dec();
		return *this;
	}

	operator atomic_type() const
	{
		return get();
	}

	NFAtomic& operator+=(atomic_type n)
	{
		add(n);
		return *this;
	}

	NFAtomic& operator-=(atomic_type n)
	{
		sub(n);
		return *this;
	}

	NFAtomic& operator=(atomic_type n)
	{
		set(n);
		return *this;
	}

	/**
	* @brief 获取值
	*
	* @return int
	*/
	atomic_type get() const { return _value.load(); }

	/**
	* @brief 添加
	* @param i
	*
	* @return int
	*/
	atomic_type add(atomic_type i) { return add_and_return(i); }

	/**
	* @brief 减少
	* @param i
	*
	* @return int
	*/
	atomic_type sub(atomic_type i) { return add_and_return(-i); }

	/**
	* @brief 自加1
	*
	* @return int
	*/
	atomic_type inc() { return add(1); }

	/**
	* @brief 自减1
	*/
	atomic_type dec() { return sub(1); }

	/**
	* @brief 自加1
	*
	* @return void
	*/
	void inc_fast()
	{
		_value++;
	}

	/**
	* @brief 自减1
	* Atomically decrements @_value by 1 and returns true if the
	* result is 0, or false for all other
	*/
	bool dec_and_test()
	{
		unsigned char c = _value.fetch_sub(1);
		return c != 0;
	}

	/**
	* @brief 设置值
	*/
	atomic_type set(atomic_type i)
	{
		_value.store(i);
		return i;
	}

protected:

	/**
	* @brief 增加并返回值
	*/
	int add_and_return(int i)
	{
		return _value.fetch_add(i);
	}

protected:

	/**
	* 值
	*/
	std::atomic_int    _value;
};

#else

#include <stdint.h>

/////////////////////////////////////////////////
/**
* @file  NFAtomic.h
* @brief  原子计数类.
*/

__BEGIN_DECLS

#define TARS_LOCK "lock ; "

typedef struct { volatile int counter; } tars_atomic_t;

#define tars_atomic_read(v)        ((v)->counter)

#define tars_atomic_set(v,i)       (((v)->counter) = (i))

__END_DECLS

/**
* @brief 原子操作类,对int做原子操作
*/
class NFAtomic
{
public:

	/**
	* 原子类型
	*/
	typedef int atomic_type;

	/**
	* @brief 构造函数,初始化为0
	*/
	NFAtomic(atomic_type at = 0)
	{
		set(at);
	}

	NFAtomic& operator++()
	{
		inc();
		return *this;
	}

	NFAtomic& operator--()
	{
		dec();
		return *this;
	}

	operator atomic_type() const
	{
		return get();
	}

	NFAtomic& operator+=(atomic_type n)
	{
		add(n);
		return *this;
	}

	NFAtomic& operator-=(atomic_type n)
	{
		sub(n);
		return *this;
	}

	NFAtomic& operator=(atomic_type n)
	{
		set(n);
		return *this;
	}

	/**
	* @brief 获取值
	*
	* @return int
	*/
	atomic_type get() const { return _value.counter; }

	/**
	* @brief 添加
	* @param i
	*
	* @return int
	*/
	atomic_type add(atomic_type i) { return add_and_return(i); }

	/**
	* @brief 减少
	* @param i
	*
	* @return int
	*/
	atomic_type sub(atomic_type i) { return add_and_return(-i); }

	/**
	* @brief 自加1
	*
	* @return int
	*/
	atomic_type inc() { return add(1); }

	/**
	* @brief 自减1
	*/
	atomic_type dec() { return sub(1); }

	/**
	* @brief 自加1
	*
	* @return void
	*/
	void inc_fast()
	{
		__asm__ __volatile__(
			TARS_LOCK "incl %0"
			:"=m" (_value.counter)
			: "m" (_value.counter));
	}

	/**
	* @brief 自减1
	* Atomically decrements @_value by 1 and returns true if the
	* result is 0, or false for all other
	*/
	bool dec_and_test()
	{
		unsigned char c;

		__asm__ __volatile__(
			TARS_LOCK "decl %0; sete %1"
			:"=m" (_value.counter), "=qm" (c)
			: "m" (_value.counter) : "memory");

		return c != 0;
	}

	/**
	* @brief 设置值
	*/
	atomic_type set(atomic_type i)
	{
		_value.counter = i;

		return i;
	}

protected:

	/**
	* @brief 增加并返回值
	*/
	int add_and_return(int i)
	{
		/* Modern 486+ processor */
		int __i = i;
		__asm__ __volatile__(
			TARS_LOCK "xaddl %0, %1;"
			:"=r"(i)
			: "m"(_value.counter), "0"(i));
		return i + __i;
	}

protected:

	/**
	* 值
	*/
	tars_atomic_t    _value;
};
#endif
