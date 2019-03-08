/**
* 单件类
*/

#ifndef __CTLIB_SINGLETON_H__
#define __CTLIB_SINGLETON_H__

#include <malloc.h>

namespace CTLib
{

/**
* 一个非常简单的智能指针
*/
template<typename T>
class CTAutoPtr
{
public:
	CTAutoPtr() : m_auto_ptr(0) {}
    CTAutoPtr(T* ptr) : m_auto_ptr(ptr) {}
    CTAutoPtr(const CTAutoPtr& obj) : m_auto_ptr(obj.get()) {}
	~CTAutoPtr() { reset (0); }

    CTAutoPtr& operator= (const CTAutoPtr& obj)
    {
        this->m_auto_ptr = obj.get();
        return *this;
    }

	T* get() const { return m_auto_ptr; }
    T* operator() () { return get(); }

	void reset(T* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            delete m_auto_ptr;
        }

        m_auto_ptr = ptr;
    }

private:
	T* m_auto_ptr;
};

/**
* 一个非常简单的用于数组的智能指针
*/
template<typename T>
class CTAutoArray
{
public:
    CTAutoArray() : m_auto_ptr(0) {}
    CTAutoArray(T* ptr) : m_auto_ptr(ptr) {}
    CTAutoArray(const CTAutoArray<T>& obj) : m_auto_ptr(obj.m_auto_ptr) {}
    ~CTAutoArray() { reset (0); }

    CTAutoArray& operator= (const CTAutoArray<T>& obj)
    {
        m_auto_ptr = obj.get();
        return *this;
    }

    T* get() const { return m_auto_ptr; }
    T* operator()() const { return get(); }

    void reset(T* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            delete [] m_auto_ptr;
        }

        m_auto_ptr = ptr;
    }

private:
    T* m_auto_ptr;
};

/**
* 一个非常简单的智能指针(free)
*/
class CTAutoFree
{
public:
    CTAutoFree() : m_auto_ptr(0) {}
    CTAutoFree(void* ptr) : m_auto_ptr(ptr) {}
    CTAutoFree(const CTAutoFree& obj) : m_auto_ptr(obj.get()) {}
    ~CTAutoFree() { reset (0); }

    CTAutoFree& operator= (const CTAutoFree& obj)
    {
        this->m_auto_ptr = obj.get();
        return *this;
    }

    void* get() const { return m_auto_ptr; }
    void* operator() () { return get(); }

    void reset(void* ptr)
    {
        if (m_auto_ptr == ptr)
        {
            return;
        }

        if (m_auto_ptr)
        {
            free(m_auto_ptr);
        }

        m_auto_ptr = ptr;
    }

private:
    void* m_auto_ptr;
};

/**
* 字符串经常需要自动释放，用这两个智能指针能解决
*/
typedef CTAutoArray<char>         CTAutoCharA;
typedef CTAutoArray<wchar_t>      CTAutoCharW;

#ifdef _UNICODE
    typedef CTAutoCharW CTAutoChar;
#else
    typedef CTAutoCharA CTAutoChar;
#endif

/**
* 单件类, X可以让单件类创建多个
*/
template<typename T, int X = 0> class CTSingleton 
{
private:
	CTSingleton() {}
	CTSingleton(const CTSingleton&) {}
	~CTSingleton() {}
	CTSingleton& operator= (const CTSingleton &) { return *this; } 

public:
	static T* Instance()
	{
		static CTAutoPtr<T> autoptr;
 		if(autoptr.get() == 0)
		{
 			autoptr.reset(new T);
		}
 		return autoptr.get();
	}
};

} // namespace CTLib

#endif //__CTLIB_SINGLETON_H__
