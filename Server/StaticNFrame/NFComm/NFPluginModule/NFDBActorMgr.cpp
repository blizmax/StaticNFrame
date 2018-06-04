#include "NFDBActorMgr.h"
#include "NFDBActor.h"
#include "NFTask.h"

NFDBActorMgr::NFDBActorMgr()
{
	mnSuitIndex = 0;
	nRecvTaskCount = 0;
	nHandleTaskCount = 0;
}

NFDBActorMgr::~NFDBActorMgr()
{
	CloseActorPool();
}

int NFDBActorMgr::Init(int actor_num)
{
	//首先初始化
	InitActor(2);

	StartActorPool(actor_num);

	for (size_t i = 0; i < m_vecActorPool.size(); i++)
	{
		int nActorId = m_vecActorPool[i];
		NFXActor* pActor = GetActor(nActorId);
		if (pActor)
		{
			NFDBActor* pDBActor = dynamic_cast<NFDBActor*>(pActor);
			if (pDBActor)
			{
				pDBActor->Init();
			}
		}
	}

	nIsInit = true;
	return 0;
}

int NFDBActorMgr::UnInit()
{
	CloseActorPool();
	return true;
}

bool NFDBActorMgr::StartActorPool(int actorNum)
{
	m_vecActorPool.resize(actorNum);
	for (int i = 0; i < actorNum; i++)
	{
		int nActorId = RequireActor();
		if (nActorId > 0)
		{
			m_vecActorPool[i] = nActorId;
		}
		else
		{
			//error
		}
	}
	return true;
}

bool NFDBActorMgr::CloseActorPool()
{
	ReleaseActor();
	m_vecActorPool.clear();
	return true;
}

bool NFDBActorMgr::HandlerEx(const NFXActorMessage& message, const int from)
{
	if (message.nMsgType != NFXActorMessage::ACTOR_MSG_TYPE_COMPONENT)
	{
		return m_mQueue.Push(message);
	}

	return false;
}

bool  NFDBActorMgr::AddTask(NFTask* pTask)
{
	if (nMainThreadRuning == false)
	{
		return false;
	}

	int nActorId = GetBalanceActor(pTask->GetBalanceId());
	if (nActorId <= 0)
	{
		return false;
	}

	if (!SendMsgToActor(nActorId, static_cast<void*>(pTask)))
	{
		return false;
	}

	nRecvTaskCount++;
	return true;
}

int NFDBActorMgr::GetBalanceActor(uint64_t balanceId)
{
	if (balanceId == 0)
	{
		return GetRandActor();
	}
	else
	{
		if (m_vecActorPool.size() <= 0)
		{
			return -1;
		}
		mnSuitIndex = balanceId % m_vecActorPool.size();
		return m_vecActorPool[mnSuitIndex];
	}
}

int NFDBActorMgr::GetRandActor()
{
	if (m_vecActorPool.size() <= 0)
	{
		return -1;
	}

	mnSuitIndex ++;
	mnSuitIndex = mnSuitIndex % m_vecActorPool.size();

	return m_vecActorPool[mnSuitIndex];
}

void NFDBActorMgr::OnMainThreadTick()
{
	std::list<NFXActorMessage> listTask;
	bool ret = m_mQueue.Pop(listTask);
	if (ret)
	{
		uint64_t start = NFGetTime();
		for (auto it = listTask.begin(); it != listTask.end(); it++)
		{
			NFXActorMessage& xMsg = *it;
			if (xMsg.nMsgType != NFXActorMessage::ACTOR_MSG_TYPE_COMPONENT)
			{
				NFTask* pTask = static_cast<NFTask*>(xMsg.pData);
				if (pTask)
				{
					NFTask::TPTaskState state = pTask->presentMainThread();
					switch (state)
					{
					case NFTask::TPTASK_STATE_COMPLETED:
					{
						nHandleTaskCount++;
						NFSafeDelete(pTask);
					}
					break;
					case NFTask::TPTASK_STATE_CONTINUE_CHILDTHREAD:
					{
						AddTask(pTask);
					}
					break;
					case NFTask::TPTASK_STATE_CONTINUE_MAINTHREAD:
					{
						m_mQueue.Push(xMsg);
					}
					break;
					default:
					{
						//error
						nHandleTaskCount++;
						NFSafeDelete(pTask);
					}
					}
				}
	        }
			else
			{
				//error
				NFTask* pTask = static_cast<NFTask*>(xMsg.pData);
				if (pTask)
				{
					NFSafeDelete(pTask);
				}
				else
				{
					//error
				}
			}
		}
		//std::cout << "handle main thread tick task num:" << listTask.size() << " use time:" << GetCurTick() - start << std::endl;;
	}

	listTask.clear();
}
