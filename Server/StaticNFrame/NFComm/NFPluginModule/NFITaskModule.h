#pragma once

#include "NFIModule.h"

class NFTaskActorMessage;
class NFTask;
class NFITaskComponent;

class NFITaskModule : public NFIModule
{
public:
	/**
	* @brief ��Ϣ���ݴ����������б�Ҫ�����ݷ��ظ����̣߳�
	*		 ��������������Ϣ�������͸����̣߳����������actor�߳���ִ��
	*
	* @param messag	��Ϣ����
	* @param from	������Ϣ��actor��ַ
	* @return �Ƿ�ɹ�
	*/
	virtual bool HandlerEx(const NFTaskActorMessage& message, const int from) = 0;

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
	virtual bool AddActorComponent(const int nActorIndex, NFITaskComponent* pComonnet) = 0;
};
