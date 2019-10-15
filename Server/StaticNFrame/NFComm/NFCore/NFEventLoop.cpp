#include "NFEventLoop.h"
#include <iostream>

NFEventLoop::NFEventLoop()
{
	Init();
}

NFEventLoop::~NFEventLoop()
{

}

void NFEventLoop::Init()
{
	m_exitLoop.store(false);
	m_status.store(kInitializing);

	m_pendingFunctors = new std::vector<Functor>();

	m_tid = std::this_thread::get_id();

	m_status.store(kInitialized);
}

void NFEventLoop::Run()
{
	m_status.store(kStarting);
	m_tid = std::this_thread::get_id();

	m_status.store(kRunning);
	m_exitLoop.store(false);
	while (!m_exitLoop.load())
	{
		std::vector<Functor> functors;
		{
			NFLock lock(m_condition.GetMutex());
			while (IsPendingQueueEmpty() && IsRunning())
			{
				m_condition.Wait(lock);
			}

			if (!IsPendingQueueEmpty())
			{
				m_notified.store(false);
				m_pendingFunctors->swap(functors);
			}
		}

		for (size_t i = 0; i < functors.size(); ++i)
		{
			functors[i]();
			--m_pendingFunctorCount;
		}
	}

	m_status.store(kStopped);
}

void NFEventLoop::Stop()
{
	assert(m_status.load() == kRunning);
	m_status.store(kStopping);

	QueueInLoop(std::bind(&NFEventLoop::StopInLoop, this));
}

void NFEventLoop::StopInLoop()
{
	std::cout << "stop in loop" << std::endl;
	assert(m_status.load() == kStopping);

	auto f = [this]() {
		for (int i = 0; ; i++)
		{
			DoPendingFunctors();
			if (IsPendingQueueEmpty()) {
				break;
			}
		}
	};

	f();

	m_exitLoop.store(true);

	f();
}

void NFEventLoop::RunInLoop(const Functor& functor)
{
	if (IsRunning() && IsInLoopThread())
	{
		functor();
	}
	else
	{
		QueueInLoop(functor);
	}
}

void NFEventLoop::RunInLoop(Functor&& functor)
{
	if (IsRunning() && IsInLoopThread())
	{
		functor();
	}
	else
	{
		QueueInLoop(functor);
	}
}

void NFEventLoop::QueueInLoop(const Functor& cb)
{
	{
		NFLock lock(m_condition.GetMutex());
		m_pendingFunctors->emplace_back(cb);
	}

	++m_pendingFunctorCount;

	if (!m_notified.load())
	{
		m_notified.store(true);
		m_condition.NotifyOne();
	}
}

void NFEventLoop::QueueInLoop(Functor&& cb)
{
	{
		NFLock lock(m_condition.GetMutex());
		m_pendingFunctors->emplace_back(cb);
	}

	++m_pendingFunctorCount;

	if (!m_notified.load())
	{
		m_notified.store(true);
		m_condition.NotifyOne();
	}
}

void NFEventLoop::DoPendingFunctors()
{
	std::vector<Functor> functors;
	{
		NFLock lock(m_condition.GetMutex());
		m_notified.store(false);
		m_pendingFunctors->swap(functors);
	}

	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
		--m_pendingFunctorCount;
	}
}

size_t NFEventLoop::GetPendingQueueSize()
{
	return m_pendingFunctors->size();
}

bool NFEventLoop::IsPendingQueueEmpty()
{
	return m_pendingFunctors->empty();
}

