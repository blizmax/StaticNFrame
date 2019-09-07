#pragma once

#include "NFIModule.h"

#include <vector>

class NFTaskActorMessage;
class NFTask;
class NFITaskComponent;

class NFITaskModule : public NFIModule
{
public:
	/**
	* @brief ��ʼ��actorϵͳ, �����̸߳���
	*
	* @param thread_num	�߳���Ŀ������Ϊ1
	* @return < 0 : Failed
	*/
	virtual int InitActorThread(int thread_num) = 0;

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
	virtual const std::vector<NFITaskComponent*>& GetTaskComponent(int nActorIndex) = 0;

	/**
	* @brief ������Actor Thread Num
	*
	* @return
	*/
	virtual int GetMaxThreads() = 0;
};
