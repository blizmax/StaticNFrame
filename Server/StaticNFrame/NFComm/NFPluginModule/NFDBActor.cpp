#include "NFDBActor.h"
#include "NFActorMgr.h"
#include "NFTask.h"



NFDBActor::NFDBActor(Theron::Framework& framework, NFXActorMgr* pActorMgr):NFXActor(framework, pActorMgr)
{
    m_bInit = false;
}


NFDBActor::~NFDBActor()
{
}

//-------------------------------------------------------------------------------------
bool NFDBActor::Init()
{
	m_bInit = true;
	return true;
}

void NFDBActor::Handler(const NFXActorMessage& message, const Theron::Address from)
{
	NFTask* pTask = static_cast<NFTask*>(message.pData);
	if (pTask)
	{
		onProcessTaskStart(pTask);
		processTask(pTask);
		onProcessTaskEnd(pTask);
	}

    ////////////////////////////////////////////////////////
    // must return message
    NFXActorMessage xReturnMessage;

    xReturnMessage.nMsgType = NFXActorMessage::ACTOR_MSG_TYPE_END_FUNC;
    xReturnMessage.nFromActor = this->GetAddress().AsInteger();
	xReturnMessage.pData = (void*)pTask;

	//HandlerEx(xReturnMessage, from);
    Send(xReturnMessage, from);
}

void NFDBActor::HandlerEx(const NFXActorMessage& message, const Theron::Address from)
{
    m_pActorMgr->HandlerEx(message, from.AsInteger());
}

void NFDBActor::onProcessTaskStart(NFTask* pTask)
{
}

void NFDBActor::processTask(NFTask* pTask)
{
	if (pTask)
	{
		pTask->db_thread_process();
	}
}

void NFDBActor::onProcessTaskEnd(NFTask* pTask)
{
}

