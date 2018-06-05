#ifndef KBE_THREADTASK_H
#define KBE_THREADTASK_H

#include <string>

/*
线程池的线程基类
*/

class NFTask
{
public:
	NFTask()
	{
		m_balanceId = 0;
	}

	virtual ~NFTask()
	{
	}

	enum TPTaskState
	{
		/// 一个任务已经完成
		TPTASK_STATE_COMPLETED = 0,

		/// 继续在主线程执行
		TPTASK_STATE_CONTINUE_MAINTHREAD = 1,

		// 继续在子线程执行
		TPTASK_STATE_CONTINUE_CHILDTHREAD = 2,
	};

	/**
	返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState presentMainThread()
	{
		return TPTASK_STATE_COMPLETED;
	}

	virtual uint64_t GetBalanceId() const { return m_balanceId; }
	void SetBalanceId(uint64_t balanceId) { m_balanceId = balanceId; }

	virtual bool db_thread_process() = 0;
protected:
	uint64_t  m_balanceId; //动态均衡ID, 如果是玩家就是玩家CharId, 如果不是一般填0
};

#endif
