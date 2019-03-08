/**
* 一个简单的定时器管理器
*/

#ifndef __CTLIB_SIMPLETIMERMGR_H__
#define __CTLIB_SIMPLETIMERMGR_H__

#include <time.h>
#include <string.h>

namespace CTLib
{

template<unsigned int TIMERCOUNT, unsigned int LOOP_GAP = 50>
class CTSimpleTimerMgr
{
public:
	typedef struct TSimpleTimerTag
	{
		unsigned int	timer;		// 定时器时长
		unsigned int	deadline;	// 定时器到期时间
		int				data;		// 注册的定时器数据
		bool			autoreg;	// 定时器触发后是否自动再注册
	} TSimpleTimerTag;

	CTSimpleTimerMgr()
	{
		m_uiNow = time(0);
		m_iLoop = 0;
		memset(&m_arrTimers, 0, sizeof(m_arrTimers));
		m_iUsed = 0;
	}

	~CTSimpleTimerMgr()
	{
	}

	int DoLoop()
	{
		++ m_iLoop;
		if (m_iLoop >= LOOP_GAP)
		{
			m_iLoop = 0;
 			m_uiNow = (unsigned int) time(0);
			return 1;
		}

		return 0;
	}

	// 获取当前时间
	unsigned int GetNow() const
	{
		return m_uiNow;
	}

	// 定时器在seconds后触发
	int RegisterTimer(unsigned int timer, int data, bool autoreg)
	{
		if (m_iUsed >= TIMERCOUNT || timer == 0)
		{
			return -1;
		}

		m_arrTimers[m_iUsed].timer = timer;
		m_arrTimers[m_iUsed].deadline = GetNow() + timer;
		m_arrTimers[m_iUsed].data = data;
		m_arrTimers[m_iUsed].autoreg = autoreg;

		++ m_iUsed;
		return 0;
	}

	// 判断是否有定时器触发了, 非0表示有定时器触发
	int CheckTimer(int& data)
	{
		if (m_iUsed == 0)
		{
			return 0;
		}

		for (i = 0; i < m_iUsed; ++i)
		{
			if (GetNow() >= m_arrTimers[i].deadline)
			{
				data = m_arrTimers[i].data;

				if (m_arrTimers[i].autoreg)
				{
					m_arrTimers[i].deadline = GetNow() + m_arrTimers[i].timer;
				}
				else
				{
					// 删除当前定时器
					-- m_iUsed;
					m_arrTimers[i].timer = m_arrTimers[m_iUsed].timer;
					m_arrTimers[i].deadline = m_arrTimers[m_iUsed].deadline;
					m_arrTimers[i].data = m_arrTimers[m_iUsed].data;
					m_arrTimers[i].autoreg = m_arrTimers[m_iUsed].autoreg;
				}

				return 1;
			}
		}

		return 0;
	}

protected:
	unsigned int	m_uiNow;
	unsigned int	m_iLoop;
	TSimpleTimerTag	m_arrTimers[TIMERCOUNT];
	unsigned int	m_iUsed;
	unsigned int	i;
};

}

#endif //__CTLIB_SIMPLETIMERMGR_H__
