#include "NFTaskActor.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFITaskComponent.h"

/**
* @brief ���캯��
*        �������RegisterHandler��ע���첽��������������ĺ���
*/
NFTaskActor::NFTaskActor(Theron::Framework& framework, NFITaskModule* pTaskModule) : Actor(framework)
{
	RegisterHandler(this, &NFTaskActor::DefaultHandler);
	m_pTaskModule = pTaskModule;
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
		m_pTaskModule->MonitorStartTask(pTask);
		ProcessTaskStart(pTask);
		ProcessTask(pTask);
		ProcessTaskEnd(pTask);
		m_pTaskModule->MonitorEndTask(pTask);

		m_pTaskModule->MonitorTask(pTask);
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
		m_pComponent->ProcessTask(pTask);
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
