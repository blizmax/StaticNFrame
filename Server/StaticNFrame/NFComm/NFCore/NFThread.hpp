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
#include<memory>
#include <atomic>
#include<mutex>

#include "NFPlatform.h"

//线程对象

class NFThread
{
public:
	NFThread()
	{
		m_running = false;
	}

	virtual ~NFThread()
	{
		StopThread();
	}

public:
	//启动线程
	template <class BaseType>
	bool StartThread(BaseType* baseType, void (BaseType::*HandleFunction)())
	{
		m_running = true;
		m_thread = std::make_shared<std::thread>(HandleFunction, baseType);
		return true;
	}

	//终止线程
	virtual bool StopThread()
	{
		std::call_once(m_flag, [this]
		               {
			               StopThreadFunction();
		               }); //保证多线程情况下只调用一次StopThread
		return true;
	}

private:
	void StopThreadFunction()
	{
		m_running = false;
		m_thread->join();
		m_thread = nullptr;
	}

protected:
	NF_SHARE_PTR<std::thread> m_thread; //线程对象
	atomic_bool m_running; //运行标志
	std::once_flag m_flag;
};

