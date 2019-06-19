// -------------------------------------------------------------------------
//    @FileName         :    NFTimerAxis.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once
#include <list>
#include <vector>
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFTimerObj.h"
#include "NFComm/NFCore/NFTime.h"

//ʱ������Ƶ�� ms
#define TIMER_AXIS_CHECK_FREQUENCE 32
//ʱ����̶�
#define TIME_GRID 64
//ʱ���᳤��
#define TIME_AXIS_LENGTH			120000		// ����Ϊ��λ��
#define TIME_AXIS_SECLENGTH			108000		// ��Ϊ��λ��֧�ֵ�30��Сʱ
#define INVALID_TIMER				0xffffffff  // ��Ч��ʱ��
#define INFINITY_CALL				0xffffffff	// �������޴�

//ʱ����
class _NFExport NFTimerAxis
{
public:
	NFTimerAxis();
	virtual ~NFTimerAxis();

	bool Init();
	bool UnInit();

	//���ö�ʱ��
	bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL);
	//�رն�ʱ��
	bool KillTimer(uint32_t nTimerID, NFTimerObj* handler);
	//�ر����ж�ʱ��
	bool KillAllTimer(NFTimerObj* handler);
	//���¶�ʱ��
	void Update();

	//���ù̶�ʱ��Ķ�ʱ��
	bool SetClocker(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterSec, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL);
	bool SetCalender(uint32_t nTimerID, const std::string& timeStr, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL);

private:
	//�����붨ʱ��
	bool SetTimerSec(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL);
	//���tick
	void CheckTick();
	//�����붨ʱ��
	void UpdateSec();

protected:
	static uint64_t GetTick()
	{
		return NFTime::Now().UnixMSec();
	}

	struct Timer
	{
		uint32_t nTimerID; //��ʱ��ID
		uint64_t nInterVal; //���
		uint32_t nCallCount; //���ô���
		uint64_t nLastTick; //���һ��tick
		NFTimerObj* pHandler;//�ص�ָ��
		uint8_t byType; //���� 0 - ���붨ʱ���� 1 - �붨ʱ��
		uint32_t nGridIndex; //���ڵ�ʱ��̶�
		std::list<Timer*>::iterator pos; //��ʱ�����е�λ�ã����ڿ��ٶ�λ
		Timer()
		{
			nTimerID = 0;
			nInterVal = 0;
			nCallCount = 0;
			nLastTick = 0;
			pHandler = nullptr;
			byType = 0;
			nGridIndex = 0;
		}
	};

	//���ö�ʱ����ʱ�����ϵ�λ��
	static bool ResetTimerPos(Timer* pTimer);

	typedef std::list<Timer*> TIMER_LIST; //ĳ���̶��ж�ʱ���б�
	typedef std::vector<TIMER_LIST*> TIMER_AXIS; //�����¼���Ķ�ʱ��

	TIMER_AXIS m_TimerAxis; //�����¼���
	uint64_t m_nInitTick; //����ʱ�����ʼTick
	uint64_t m_nLastTick; //����ʱ�������һ�μ���tick

	TIMER_AXIS m_TimerAxisSec; //��ʱ����
	uint64_t m_nInitSec; //��ʱ�����ʼʱ��
	uint64_t m_nLastSec; //��ʱ�������һ�μ���ʱ��
};

