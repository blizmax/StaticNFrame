// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoop.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFEventLoop
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerStatus.h"
#include <functional>
#include <thread>
#include <vector>
#include "NFComm/NFCore/NFMutex.h"
#include "NFComm/NFCore/NFCondition.h"
#include "NFComm/NFCore/NFPlatform.h"

class _NFExport NFEventLoop : public NFServerStatus
{
public:
	typedef std::function<void()> Functor;
public:
	NFEventLoop();
	~NFEventLoop();

	void Run();

	void Stop();

	void RunInLoop(const Functor& handler);
	void RunInLoop(Functor&& handler);

	void QueueInLoop(const Functor& handler);
	void QueueInLoop(Functor&& handler);
private:
	void Init();
	void StopInLoop();
	void DoPendingFunctors();
	size_t GetPendingQueueSize();
	bool IsPendingQueueEmpty();
	bool IsInLoopThread() const {
		return m_tid == std::this_thread::get_id();
	}
public:
	std::thread::id m_tid;

	std::atomic<bool> m_notified;

	std::vector<Functor>* m_pendingFunctors;

	std::atomic<int> m_pendingFunctorCount;

	NFCondition m_condition;

	std::atomic<bool> m_exitLoop;
};


