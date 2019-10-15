#include "NFEventLoopThreadPool.h"
#include <iostream>

NFEventLoopThreadPool::NFEventLoopThreadPool(NFEventLoop* base_loop, uint32_t thread_num)
	: m_baseLoop(base_loop), m_threadNum(thread_num)
{
	std::cout << "NFEventLoopThreadPool thread_num:" << m_threadNum << std::endl;
}

NFEventLoopThreadPool::~NFEventLoopThreadPool()
{
	Join();
	m_threads.clear();
}

bool NFEventLoopThreadPool::Start(bool wait_thread_started)
{
	m_status.store(kStarting);
	std::cout << "NFEventLoopThreadPool start thread_num:" << m_threadNum << " wait_thread_started:" << wait_thread_started << std::endl;
	if (m_threadNum == 0)
	{
		m_status.store(kRunning);
		return true;
	}

	std::shared_ptr<std::atomic<uint32_t>> started_count(new std::atomic<uint32_t>());
	std::shared_ptr<std::atomic<uint32_t>> exited_count(new std::atomic<uint32_t>());
	for (uint32_t i = 0; i < m_threadNum; ++i)
	{
		auto prefn = [this, started_count]() {
			std::cout << "a working thread started tid=" << ThreadId() << std::endl;
			this->OnThreadStarted(started_count->fetch_add(1) + 1);
			return NFEventLoopThread::kOK;
		};

		auto postfn = [this, exited_count]() {
			std::cout << "a working thread exiting tid=" << ThreadId() << std::endl;
			this->OnThreadExited(exited_count->fetch_add(1) + 1);
			return NFEventLoopThread::kOK;
		};

		EventLoopThreadPtr t(new NFEventLoopThread());
		if (!t->Start(wait_thread_started, prefn, postfn)) {
			std::cout << "thread start failed" << std::endl;
			return false;
		}

		std::stringstream ss;
		ss << "EventLoopThreadPool-thread-" << i << "th";
		t->SetName(ss.str());
		m_threads.push_back(t);
	}

	if (wait_thread_started)
	{
		while (!IsRunning())
		{
			NFSLEEP(1);
		}
		assert(m_status.load() == kRunning);
	}
	return true;
}

void NFEventLoopThreadPool::Stop(bool wait_thread_exit)
{
	Stop(wait_thread_exit, DoneCallback());
}

void NFEventLoopThreadPool::Stop(DoneCallback fn)
{
	Stop(false, fn);
}

void NFEventLoopThreadPool::Stop(bool wait_thread_exit, DoneCallback fn)
{
	m_status.store(kStopping);

	if (m_threadNum == 0)
	{
		m_status.store(kStopped);

		if (fn)
		{
			std::cout << "calling stopped callback" << std::endl;
			fn();
		}
		return;
	}

	m_stoppedCb = fn;

	for (auto &t : m_threads)
	{
		t->Stop();
	}

	auto is_stopped_fn = [this]() {
		for (auto &t : this->m_threads)
		{
			if (!t->IsStopped())
			{
				return false;
			}
		}
		return true;
	};

	std::cout << "before promise wait" << std::endl;

	if (m_threadNum > 0 && wait_thread_exit)
	{
		while (!is_stopped_fn())
		{
			NFSLEEP(1);
		}
	}
	std::cout << "after promise wait" << std::endl;

	m_status.store(kStopped);
}

void NFEventLoopThreadPool::Join()
{
	std::cout << "Join" << std::endl;

	for (auto &t : m_threads)
	{
		t->Join();
	}

	m_threads.clear();
}

NFEventLoop* NFEventLoopThreadPool::GetNextLoop()
{
	NFEventLoop* loop = m_baseLoop;

	if (IsRunning() && !m_threads.empty())
	{
		int64_t next = m_next.fetch_add(1);
		next = next % m_threads.size();
		loop = (m_threads[next])->loop();
	}

	return loop;
}

NFEventLoop* NFEventLoopThreadPool::GetNextLoopWithHash(uint64_t hash)
{
	NFEventLoop* loop = m_baseLoop;

	if (!IsRunning() && !m_threads.empty())
	{
		uint64_t next = hash % m_threads.size();
		loop = (m_threads[next])->loop();
	}

	return loop;
}

uint32_t NFEventLoopThreadPool::thread_num() const
{
	return m_threadNum;
}

void NFEventLoopThreadPool::OnThreadStarted(uint32_t count)
{
	std::cout << "OnThreadStarted cout:" << count << std::endl;
	if (count == m_threadNum)
	{
		std::cout << "thread pool totally started" << std::endl;
		m_status.store(kRunning);
	}
}

void NFEventLoopThreadPool::OnThreadExited(uint32_t count)
{
	std::cout << "OnThreadExited cout:" << count << std::endl;
	if (count == m_threadNum)
	{
		m_status.store(kStopped);
		std::cout << "this is the last thread stopped. Thread pool totally exited." << count << std::endl;
		
		if (m_stoppedCb)
		{
			m_stoppedCb();
			m_stoppedCb = DoneCallback();
		}
	}
}