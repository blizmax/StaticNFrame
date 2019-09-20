#include "NFTaskActor.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFITaskComponent.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

/**
* @brief ���캯��
*        �������RegisterHandler��ע���첽��������������ĺ���
*/
NFTaskActor::NFTaskActor(Theron::Framework& framework, NFITaskModule* pTaskModule) : Actor(framework)
{
	RegisterHandler(this, &NFTaskActor::DefaultHandler);
	m_pTaskModule = pTaskModule;
	m_curTaskStartTime = 0;
	m_pCurProcessTask = nullptr;
	m_pComponent = nullptr;
}

/**
* @brief ��������
*
*/
NFTaskActor::~NFTaskActor()
{
	if (m_pComponent)
	{
		NF_SAFE_DELETE(m_pComponent);
	}
}

/**
* @brief ��ʼ��
*
* @param
* @return
*/
bool NFTaskActor::Init()
{
	return true;
}

/**
* @brief ������һ��actor������Ϣ
*
* @param address ������Ϣ��actor�ĵ�ַ
* @param message ���ֵ���Ϣ
* @return ���ط����Ƿ�ɹ�
*/
bool NFTaskActor::SendMsg(const Theron::Address& address, const NFTaskActorMessage& message)
{
	return Send(message, address);
}

/**
* @brief ���component
*
* @param
* @return
*/
bool NFTaskActor::AddComponnet(NFITaskComponent* pComponnet)
{
	m_pComponent = pComponnet;
	return true;
}

void NFTaskActor::Handler(const NFTaskActorMessage& message, const Theron::Address from)
{
	NFTask* pTask = message.pData;
	if (pTask)
	{
		ProcessTaskStart(pTask);
		ProcessTask(pTask);
		ProcessTaskEnd(pTask);

		if (pTask->IsNeedMainThreadProcess() == false)
		{
			NF_SAFE_DELETE(pTask);
			return;
		}
	}

	////////////////////////////////////////////////////////
	// must return message
	NFTaskActorMessage xReturnMessage;

	xReturnMessage.nMsgType = NFTaskActorMessage::ACTOR_MSG_TYPE_END_FUNC;
	xReturnMessage.nFromActor = this->GetAddress().AsInteger();
	xReturnMessage.pData = pTask;

	Send(xReturnMessage, from);
}

void NFTaskActor::HandlerEx(const NFTaskActorMessage& message, const Theron::Address from)
{
	m_pTaskModule->HandlerEx(message, from.AsInteger());
}

void NFTaskActor::DefaultHandler(const NFTaskActorMessage& message, const Theron::Address from)
{
	if (message.nMsgType == NFTaskActorMessage::ACTOR_MSG_TYPE_COMPONENT)
	{
		Handler(message, from);
	}
	else
	{
		HandlerEx(message, from);
	}
}

void NFTaskActor::ProcessTaskStart(NFTask* pTask)
{
	if (m_pComponent)
	{
		m_pComponent->ProcessTaskStart(pTask);
	}
}

void NFTaskActor::ProcessTask(NFTask* pTask)
{
	if (m_pComponent)
	{
		m_curCheckMutex.Lock();
		m_pCurProcessTask = pTask;
		m_curTaskStartTime = NFGetTime();
		m_curCheckMutex.Unlock();

		m_pComponent->ProcessTask(pTask);

		m_curCheckMutex.Lock();
		pTask->m_useTime = NFGetTime() - m_curTaskStartTime;
		m_curTaskStartTime = 0;
		m_pCurProcessTask = nullptr;
		m_curCheckMutex.Unlock();

		m_pTaskModule->MonitorTask(pTask);
	}
}

/**
* @brief ��鳬ʱtask, �����̴߳���
*
* @param
* @return
*/
void NFTaskActor::CheckTimeoutTask()
{
	if (m_pComponent == nullptr)
	{
		return;
	}

	bool checktimeout = false;
	bool checkdeadcycle = false;
	uint64_t useTime = 0;
	std::string taskName;
	m_curCheckMutex.Lock();
	if (m_pCurProcessTask && m_curTaskStartTime > 0)
	{
		useTime = NFGetTime() - m_curTaskStartTime;
		taskName = m_pCurProcessTask->m_taskName;
		if (m_curTaskStartTime > 0 && useTime > 1000)
		{
			checktimeout = true;
		}

		if (m_curTaskStartTime > 0 && useTime > 1000)
		{
			checkdeadcycle = true;
		}
	}
	m_curCheckMutex.Unlock();

	if (checktimeout)
	{
		m_pComponent->HandleTaskTimeOut(taskName, useTime);
	}

	if (checkdeadcycle)
	{
		m_pComponent->HandleTaskDeadCycle(taskName, useTime);
	}
}

void NFTaskActor::ProcessTaskEnd(NFTask* pTask)
{
	if (m_pComponent)
	{
		m_pComponent->ProcessTaskEnd(pTask);
	}
}

/**
* @brief �������component
*
* @param
* @return
*/
NFITaskComponent* NFTaskActor::GetTaskComponent()
{ 
	return m_pComponent;
}

/**
* @brief ���ActorId
*
* @param
* @return
*/
int NFTaskActor::GetActorId() const
{ 
	return this->GetAddress().AsInteger(); 
}
