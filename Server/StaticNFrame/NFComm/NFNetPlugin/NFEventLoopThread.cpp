// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoopThread.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#include "NFEventLoopThread.h"
#include "NFComm/NFCore/NFPlatform.h"
#include <iostream>
#include "NFEventLoop.h"

NFEventLoopThread::NFEventLoopThread()
	: event_loop_(new NFEventLoop)
{
	std::cout << "loop=" << event_loop_ << std::endl;
}

NFEventLoopThread::~NFEventLoopThread()
{
	std::cout << "loop=" << event_loop_ << std::endl;
	assert(IsStopped());
	Join();
}

bool NFEventLoopThread::Start(bool wait_thread_started, Functor pre, Functor post)
{
	mStatus = kStarting;

	assert(thread_.get() == nullptr);
	thread_.reset(new std::thread(std::bind(&NFEventLoopThread::Run, this, pre, post)));

	if (wait_thread_started)
	{
		while (mStatus < kRunning)
		{
			NFSLEEP(1);
		}
	}
	return true;
}

void NFEventLoopThread::Run(const Functor& pre, const Functor& post)
{
	std::cout << "loop=" << event_loop_ << std::endl;
	if (name_.empty())
	{
		std::ostringstream os;
		os << "thread-" << std::this_thread::get_id();
		name_ = os.str();
	}


	std::cout << "loop=" << event_loop_ << " execute pre functor." << std::endl;
	auto fn = [this, pre]()
		{
			mStatus = kRunning;
			if (pre)
			{
				auto rc = pre();
				if (rc != kOK)
				{
					event_loop_->Stop();
				}
			}
		};
	event_loop_->QueueInLoop(std::move(fn));
	event_loop_->Run();

	std::cout << "loop=" << event_loop_ << " execute post functor." << std::endl;
	if (post)
	{
		post();
	}

	assert(event_loop_->IsStopped());
	std::cout << "loop=" << event_loop_ << " EventLoopThread stopped" << std::endl;
	mStatus = kStopped;
}

void NFEventLoopThread::Stop(bool wait_thread_exit)
{
	std::cout << "loop=" << event_loop_ << " wait_thread_exit=" << wait_thread_exit << std::endl;
	assert(mStatus == kRunning && IsRunning());
	mStatus = kStopping;
	event_loop_->Stop();

	if (wait_thread_exit)
	{
		while (!IsStopped())
		{
			NFSLEEP(1);
		}

		std::cout << "loop=" << event_loop_ << " thread stopped." << std::endl;
		Join();
		std::cout << "loop=" << event_loop_ << " thread totally stopped." << std::endl;
	}
	std::cout << "loop=" << event_loop_ << std::endl;
}

void NFEventLoopThread::Join()
{
	// To avoid multi other threads call Join simultaneously
	std::lock_guard<std::mutex> guard(mutex_);
	if (thread_ && thread_->joinable())
	{
		std::cout << "loop=" << event_loop_ << " thread=" << thread_ << " joinable" << std::endl;
		try
		{
			thread_->join();
		}
		catch (const std::system_error& e)
		{
			std::cout << "Caught a system_error:" << e.what() << " code=" << e.code() << std::endl;
		}
		thread_.reset();
	}
}

void NFEventLoopThread::SetName(const std::string& n)
{
	name_ = n;
}

const std::string& NFEventLoopThread::GetName() const
{
	return name_;
}


NFEventLoop* NFEventLoopThread::Loop() const
{
	return event_loop_.get();
}


struct event_base* NFEventLoopThread::event_base()
{
	return Loop()->event_base();
}

std::thread::id NFEventLoopThread::GetTid() const
{
	if (thread_)
	{
		return thread_->get_id();
	}

	return std::thread::id();
}

bool NFEventLoopThread::IsRunning() const
{
	// Using event_loop_->IsRunning() is more exact to query where thread is
	// running or not instead of mStatus == kRunning
	//
	// Because in some particular circumstances,
	// when mStatus==kRunning and event_loop_::running_ == false,
	// the application will broke down
	return event_loop_->IsRunning();
}

