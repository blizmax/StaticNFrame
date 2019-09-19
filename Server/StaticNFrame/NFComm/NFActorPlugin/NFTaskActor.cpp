#include "NFTaskActor.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFITaskComponent.h"

/**
* @brief 构造函数
*        必须调用RegisterHandler，注册异步过程中用来处理的函数
*/
NFTaskActor::NFTaskActor(Theron::Framework& framework, NFITaskModule* pTaskModule) : Actor(framework)
{
	RegisterHandler(this, &NFTaskActor::DefaultHandler);
	m_pTaskModule = pTaskModule;
}

/**
* @brief 析构函数
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
* @brief 初始化
*
* @param
* @return
*/
bool NFTaskActor::Init()
{
	return true;
}

/**
* @brief 向另外一个actor发送消息
*
* @param address 接受消息的actor的地址
* @param message 发现的消息
* @return 返回发送是否成功
*/
bool NFTaskActor::SendMsg(const Theron::Address& address, const NFTaskActorMessage& message)
{
	return Send(message, address);
}

/**
* @brief 添加component
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
* @brief 获得所有component
*
* @param
* @return
*/
NFITaskComponent* NFTaskActor::GetTaskComponent()
{ 
	return m_pComponent;
}

/**
* @brief 获得ActorId
*
* @param
* @return
*/
int NFTaskActor::GetActorId() const
{ 
	return this->GetAddress().AsInteger(); 
}
