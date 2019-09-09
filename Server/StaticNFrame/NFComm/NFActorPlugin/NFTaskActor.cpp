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
	for (size_t i = 0; i < m_taskComponents.size(); i++)
	{
		NF_SAFE_DELETE(m_taskComponents[i]);
	}
	m_taskComponents.clear();
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
	m_taskComponents.push_back(pComponnet);
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
	for (size_t i = 0; i < m_taskComponents.size(); i++)
	{
		NFITaskComponent* pComponent = m_taskComponents[i];
		if (pComponent)
		{
			pComponent->ProcessTaskStart(pTask);
		}
	}
}

void NFTaskActor::ProcessTask(NFTask* pTask)
{
	for (size_t i = 0; i < m_taskComponents.size(); i++)
	{
		NFITaskComponent* pComponent = m_taskComponents[i];
		if (pComponent)
		{
			pComponent->ProcessTask(pTask);
		}
	}
}

void NFTaskActor::ProcessTaskEnd(NFTask* pTask)
{
	for (size_t i = 0; i < m_taskComponents.size(); i++)
	{
		NFITaskComponent* pComponent = m_taskComponents[i];
		if (pComponent)
		{
			pComponent->ProcessTaskEnd(pTask);
		}
	}
}

/**
* @brief �������component
*
* @param
* @return
*/
const std::vector<NFITaskComponent*>& NFTaskActor::GetTaskComponent() const
{ 
	return m_taskComponents; 
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
