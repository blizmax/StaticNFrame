// -------------------------------------------------------------------------
//    @FileName         :    NFBuffer.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-16
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <cstdarg>
#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#include <map>
#include <typeinfo>
#include <iomanip>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdint>

#include "NFComm/NFCore/NFPlatform.h"
#include "googletest/googletest/include/gtest/gtest.h"

#include "NFComm/NFCore/NFThread.hpp"
#include <mutex>
#include <atomic>

//#define USE_MUTEX

class class_test_thread
{
public:
	class_test_thread()
	{
		mThread = std::shared_ptr<NFThread>(new NFThread());
	}

	~class_test_thread()
	{
	}

	bool StartThread()
	{
		return mThread->StartThread(this, &class_test_thread::click);
	}

	bool StopThread()
	{
		return mThread->StopThread();
	}

	void click()
	{
		for (int i = 0; i < 1000000; i++)
		{
#ifdef USE_MUTEX
			mMutex.lock();
#endif
			count++;
#ifdef USE_MUTEX
			mMutex.unlock();
#endif
		}
	}
private:
	std::shared_ptr<NFThread> mThread;
public:
#ifdef USE_MUTEX
	static std::mutex mMutex;
#endif
	static atomic_long count;
};

atomic_long class_test_thread::count = 0;
#ifdef USE_MUTEX
std::mutex class_test_thread::mMutex;
#endif

TEST(NF_Core, NF_Thread)
{
	{
		class_test_thread test;
		EXPECT_EQ(true, test.StartThread());
		EXPECT_EQ(true, test.StopThread());
		std::cout << class_test_thread::count << std::endl;
	}

	{
		std::vector<class_test_thread> vecTest;
		for (int i = 0; i < 10; i++)
		{
			vecTest.push_back(class_test_thread());
		}
		for (int i = 0; i < 10; i++)
		{
			EXPECT_EQ(true, vecTest[i].StartThread());
		}
		for (int i = 0; i < 10; i++)
		{
			EXPECT_EQ(true, vecTest[i].StopThread());
		}
		std::cout << class_test_thread::count << std::endl;
	}
}
