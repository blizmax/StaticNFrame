

#include "NFTaskActor.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"

/**
* @brief ���캯��
*        �������RegisterHandler��ע���첽��������������ĺ���
*/
NFTaskActor::NFTaskActor(Theron::Framework& framework, NFITaskModule* pTaskModule) : Theron::Actor(framework)
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
}

void NFTaskActor::ProcessTask(NFTask* pTask)
{
	if (pTask)
	{
		//NFLogError("actor id:{}, threadid:{}", this->GetAddress().AsInteger(), ThreadId());
		pTask->ThreadProcess();
	}
}

void NFTaskActor::ProcessTaskEnd(NFTask* pTask)
{
}