// -------------------------------------------------------------------------
//    @FileName         :    NFThread.hpp
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include<thread>
#include<functional>

#include "NFPlatform.h"

/**
A process thread.
\note This implementation uses C++0x threads so requires C++0x compiler (e.g. GCC >= 4.6)
*/
class _NFExport NFThread
{
public:

	/**
	Defines a function that can serve as a thread entry point.
	\note Entry point functions must be static -- implying that they can't be
	non-static class member functions.
	*/
	typedef void(*EntryPoint)(void *const context);

	/**
	Default constructor
	*/
	inline NFThread() : mThread(0)
	{
	}

	/**
	Destructor
	*/
	inline ~NFThread()
	{
		NF_ASSERT(mThread == 0);
	}

	/**
	Starts the thread, executing the given entry point function.
	\param entryPoint The entry point function that the thread should execute.
	\param context Pointer to a context object providing the environment in which the thread runs.
	\return True, if the thread was started successfully.
	*/
	inline bool Start(EntryPoint entryPoint, void *const context)
	{
		NF_ASSERT(mThread == 0);

		// Construct a std::thread in the allocated memory
		// Pass it a callable object that in turn calls the entry point, passing it some context.
		NFThreadStarter starter(entryPoint, context);
		mThread = new std::thread(starter);

		if (mThread == 0)
		{
			return false;
		}

		return true;
	}

	/**
	Waits for the thread to finish and return.
	The semantics are that Start and Join can be called repeatedly in pairs.
	*/
	inline void Join()
	{
		NF_ASSERT(mThread);

		// This waits for the thread function to return.
		mThread->join();

		NF_SAFE_DELETE(mThread);
		mThread = 0;
	}

	/**
	Returns true if the thread is currently running.
	The thread is running if Start was called more recently than Join.
	*/
	NF_FORCEINLINE bool Running() const
	{
		return (mThread != 0);
	}
private:

	/**
	A callable object implementing operator() that starts a thread.
	Basically a struct that holds a pointer to a thread entry point function and some context data.
	*/
	class NFThreadStarter
	{
	public:

		inline NFThreadStarter(EntryPoint entryPoint, void *const context) :
			mEntryPoint(entryPoint),
			mContext(context)
		{
		}

		inline void operator()()
		{
			mEntryPoint(mContext);
		}

	private:

		EntryPoint mEntryPoint;
		void *mContext;
	};

	NFThread(const NFThread &other);
	NFThread &operator=(const NFThread &other);

	std::thread *mThread;     ///< Pointer to the owned std::thread.
};

