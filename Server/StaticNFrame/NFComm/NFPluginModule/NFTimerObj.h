#pragma once
#include <stdint.h>

//��ʱ���ӿ�
struct NFTimerObj
{
public:
	NFTimerObj();
	virtual ~NFTimerObj();
public:
	virtual void OnTimer(uint32_t nTimerID) = 0;

	//���ö�ʱ��
	virtual bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, uint32_t nCallCount);

	//�رն�ʱ��
	virtual bool KillTimer(uint32_t nTimerID);

	//�ر����ж�ʱ��
	virtual bool KillAllTimer();

	//���ù̶�ʱ��Ķ�ʱ��
	virtual bool SetFixTimer(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount);

	//�رչ̶�ʱ�䶨ʱ��
	virtual bool KillFixTimer(uint32_t nTimerID);

	//�ر����й̶�ʱ�䶨ʱ��
	virtual bool KillAllFixTimer();
public:
	virtual void** GetTimerInfoPtr()
	{
		return &m_pTimerInfoPtr;
	}

	virtual void** GetFixTimerInfoPtr()
	{
		return &m_pFixTimerInfoPtr;
	}

private:
	void* m_pTimerInfoPtr; //��ʱ����ص�ָ���Ӧ�Ķ�ʱ����Ϣ
	void* m_pFixTimerInfoPtr; //��ʱ����ص�ָ���Ӧ�Ĺ̶�ʱ��Ķ�ʱ����Ϣ
};

