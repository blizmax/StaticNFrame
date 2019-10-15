#include "NFEventLoopThread.h"

NFEventLoopThread::NFEventLoopThread() :m_eventLoop(new NFEventLoop())
{

}

NFEventLoopThread::~NFEventLoopThread()
{
	Join();
}

bool NFEventLoopThread::Start(bool wait_thread_started, Functor pre, Functor post)
{
	m_status = kStarting;
	assert(m_thread.get() == nullptr);

	m_thread.reset(new std::thread(std::bind(&NFEventLoopThread::Run, this, pre, post)));

	if (wait_thread_started)
	{
		while (m_status < kRunning)
		{
			NFSLEEP(1);
		}
	}
	return true;
}

void NFEventLoopThread::Run(const Functor& pre, const Functor& post)
{
	if (m_name.empty())
	{
		std::ostringstream os;
		os << "thread-" << std::this_thread::get_id();
		m_name = os.str();
	}

	auto fn = [this, pre]() {
		m_status = kRunning;
		if (pre) {
			auto rc = pre();
			if (rc != kOK) {
				m_eventLoop->Stop();
			}
		}
	};

	m_eventLoop->QueueInLoop(std::move(fn));
	m_eventLoop->Run();

	if (post) {
		post();
	}

	m_status = kStopped;
}

void NFEventLoopThread::Stop(bool wait_thread_exit) {
	m_status = kStopping;
	m_eventLoop->Stop();

	if (wait_thread_exit)
	{
		while (!IsStopped())
		{
			NFSLEEP(1);
		}

		Join();
	}
}

void NFEventLoopThread::Join()
{
	NFLock lock(m_mutex);
	if (m_thread && m_thread->joinable())
	{
		try {
			m_thread->join();
		}
		catch (const std::system_error& e)
		{

		}
		m_thread.reset();
	}
}

bool NFEventLoopThread::IsRunning() const
{
	return m_eventLoop->IsRunning();
}

void NFEventLoopThread::SetName(const std::string& n)
{
	m_name = n;
}

const std::string& NFEventLoopThread::GetName() const
{
	return m_name;
}

NFEventLoop* NFEventLoopThread::loop() const
{
	return m_eventLoop.get();
}
