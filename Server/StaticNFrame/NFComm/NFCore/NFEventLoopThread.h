// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoopThread.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFEventLoopThread
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerStatus.h"
#include "NFEventLoop.h"


class _NFExport NFEventLoopThread : public NFServerStatus
{
public:
	enum { kOK = 0 };
	typedef std::function<int()> Functor;
	NFEventLoopThread();
	~NFEventLoopThread();

	bool Start(bool wait_thread_started = true, Functor pre = Functor(), Functor post = Functor());

	void Stop(bool wait_thread_exit = false);

	void Join();

	void SetName(const std::string& n);
	const std::string& GetName() const;
	NFEventLoop* loop() const;
private:
	void Run(const Functor& pre, const Functor& post);
	bool IsRunning() const;
private:
	std::shared_ptr<NFEventLoop> m_eventLoop;
	NFMutex m_mutex;
	std::shared_ptr<std::thread> m_thread;
	std::string m_name;
};

