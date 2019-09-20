#pragma once

#include "NFIModule.h"

#include <vector>

class NFTaskActorMessage;
class NFTask;
class NFITaskComponent;

enum TaskModule_YieldStrategy
{
	TaskModule_YIELD_STRATEGY_CONDITION = 0,       ///< Threads wait on condition variables when no work is available.
	TaskModule_YIELD_STRATEGY_HYBRID,              ///< Threads spin for a while, then yield to other threads, when no work is available.
	TaskModule_YIELD_STRATEGY_SPIN,                ///< Threads busy-wait, without yielding, when no work is available.

	// Legacy section
	TaskModule_YIELD_STRATEGY_BLOCKING = 0,        ///< Deprecated - use YIELD_STRATEGY_CONDITION.
	TaskModule_YIELD_STRATEGY_POLITE = 0,          ///< Deprecated - use YIELD_STRATEGY_CONDITION.
	TaskModule_YIELD_STRATEGY_STRONG = 1,          ///< Deprecated - use YIELD_STRATEGY_HYBRID.
	TaskModule_YIELD_STRATEGY_AGGRESSIVE = 2       ///< Deprecated - use YIELD_STRATEGY_SPIN.
};

class NFITaskModule : public NFIModule
{
public:
	/**
	* @brief ��ʼ��actorϵͳ, �����̸߳���
	*
	* @param thread_num	�߳���Ŀ������Ϊ1
	* @return < 0 : Failed
	*/
	virtual int InitActorThread(int thread_num, int yieldstrategy = 0) = 0;

	/**
	* @brief ��Ϣ���ݴ����������б�Ҫ�����ݷ��ظ����̣߳�
	*		 ��������������Ϣ�������͸����̣߳����������actor�߳���ִ��
	*
	* @param message	��Ϣ����
	* @param from	������Ϣ��actor��ַ
	* @return �Ƿ�ɹ�
	*/
	virtual bool HandlerEx(const NFTaskActorMessage& message, int from) = 0;

	/**
	* @brief ���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	virtual bool AddTask(NFTask* pTask) = 0;

	/**
	* @brief ���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	template<typename NFTaskType>
	bool AddTaskToEveryActor(const NFTaskType& task)
	{
		std::vector<int> vecActorId = GetAllActorId();
		for (size_t i = 0; i < vecActorId.size(); i++)
		{
			auto pTask = new NFTaskType();
			*pTask = task;
			SendMsgToActor(vecActorId[i], pTask);
		}
		return true;
	}

	/**
	* @brief ���߳�ͨ���Լ������actorIndex���������ݸ�actor�߳�
	*
	* @param nActorIndex	actorΨһ����
	* @param pData			Ҫ���͵�����
	* @return �Ƿ�ɹ�
	*/
	virtual bool SendMsgToActor(int nActorIndex, NFTask* pData) = 0;

	/**
	* @brief ���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	virtual bool AddTask(int actorId, NFTask* pTask) = 0;

	/**
	* @brief ��ϵͳ��������һ��actor
	*
	* @return ����actor��Ψһ����
	*/
	virtual int RequireActor() = 0;

	/**
	* @brief ���ϵͳ��û�д������������Ŀ
	*
	* @return δ��ɵ�������Ŀ
	*/
	virtual int GetNumQueuedMessages() = 0;

	/**
	* @brief ���һ��Actor���
	*
	* @return
	*/
	virtual bool AddActorComponent(int nActorIndex, NFITaskComponent* pComonnet) = 0;

	/**
	* @brief �������component
	*
	* @param
	* @return
	*/
	virtual NFITaskComponent* GetTaskComponent(int nActorIndex) = 0;

	/**
	* @brief ������Actor Thread Num
	*
	* @return
	*/
	virtual int GetMaxThreads() = 0;

	/**
	* @brief ��¼���Task
	*
	* @return
	*/
	virtual void MonitorTask(NFTask* pTask) = 0;

	/**
	* @brief ��ȡ����ActorId
	*
	* @return
	*/
	virtual std::vector<int> GetAllActorId() const = 0;
};
