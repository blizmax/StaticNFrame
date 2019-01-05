

#include "NFTaskActor.h"
#include "NFTask.h"
#include "NFITaskModule.h"
#include "NFLogMgr.h"

/**
* @brief 构造函数
*        必须调用RegisterHandler，注册异步过程中用来处理的函数
*/
NFTaskActor::NFTaskActor(Theron::Framework& framework, NFITaskModule* pTaskModule) : Theron::Actor(framework)
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