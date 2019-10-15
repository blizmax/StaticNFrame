// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoopThreadPool.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFEventLoopThreadPool
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerStatus.h"
#include "NFEventLoop.h"
#include "NFEventLoopThread.h"

class _NFExport NFEventLoopThreadPool : public NFServerStatus
{
public:
	typedef std::function<void()> DoneCallback;

	NFEventLoopThreadPool(NFEventLoop* base_loop, uint32_t thread_num);

	~NFEventLoopThreadPool();

	bool Start(bool wait_thread_started = false);

	void Stop(bool wait_thread_exited = false);

	void Stop(DoneCallback fn);

	void Join();
public:
	NFEventLoop* GetNextLoop();
	NFEventLoop* GetNextLoopWithHash(uint64_t hash);

	uint32_t thread_num() const;
private:
	void Stop(bool wait_thread_exit, DoneCallback fn);
	void OnThreadStarted(uint32_t count);
	void OnThreadExited(uint32_t count);
private:
	NFEventLoop* m_baseLoop;

	uint32_t m_threadNum = 0;

	std::atomic<int64_t> m_next = { 0 };

	DoneCallback m_stoppedCb;

	typedef std::shared_ptr<NFEventLoopThread> EventLoopThreadPtr;
	std::vector<EventLoopThreadPtr> m_threads;
};
