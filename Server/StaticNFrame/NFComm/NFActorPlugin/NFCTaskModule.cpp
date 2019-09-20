// -------------------------------------------------------------------------
//    @FileName         :    NFCTaskModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFActorPlugin
//
// -------------------------------------------------------------------------

#include "NFCTaskModule.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"

#include "NFComm/NFPluginModule/NFITaskComponent.h"
#include "NFComm/NFCore/NFTime.h"
#include "NFComm/NFCore/NFCommon.h"

NFCTaskModule::NFCTaskModule(NFIPluginManager* p)
{
	m_loopCount = 0;
	m_pPluginManager = p;
	mnSuitIndex = 0;
	srand(static_cast<unsigned>(time(nullptr)));
}

NFCTaskModule::~NFCTaskModule()
{

}

bool NFCTaskModule::Awake()
{
	//首先初始化
#if NF_PLATFORM == NF_PLATFORM_WIN
	uint32_t threadNum = std::thread::hardware_concurrency();
#else
	uint32_t threadNum = std::thread::hardware_concurrency() * 2;
#endif
	InitActorThread(threadNum);
	return true;
}

bool NFCTaskModule::Init()
{
	return true;
}

bool NFCTaskModule::AfterInit()
{
	return true;
}

bool NFCTaskModule::BeforeShut()
{
	return true;
}

bool NFCTaskModule::Shut()
{
	uint32_t startTime = NFGetSecondTime();
	//等待异步处理完毕，然后再退出系统
	while (GetNumQueuedMessages() > 0)
	{
		NFSLEEP(1);
		if (NFGetSecondTime() - startTime >= 30)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "task module shut, but has task not finish after wait 30 second!");
			break;
		}
	}

	return true;
}

bool NFCTaskModule::Finalize()
{
	CloseActorPool();
	return true;
}

bool NFCTaskModule::Execute()
{
	m_loopCount++;
	OnMainThreadTick();
	if (m_loopCount % 10000 == 0)
	{
		NFLogDebug(NF_LOG_ACTOR_PLUGIN, 0, "task actor module, work thread num:{} peak work thread num:{}, max work thread num:{}, min work thread num:{}", m_pFramework->GetNumThreads(), m_pFramework->GetPeakThreads(), m_pFramework->GetMaxThreads(), m_pFramework->GetMinThreads());
		mMonitorMutex.Lock();
		for (auto iter = m_taskMonitorMap.begin(); iter != m_taskMonitorMap.end(); iter++)
		{
			NFLogDebug(NF_LOG_ACTOR_PLUGIN, 0, "task:{} use all time:{}, count:{}, per use time:{}", iter->first, iter->second.mAllUseTime, iter->second.mCount, iter->second.mPerUseTime);
		}
		mMonitorMutex.Unlock();
	}

	if (m_loopCount % 30 == 0)
	{
		CheckTimeOutTask();
	}

	return true;
}

/**
* @brief 初始化actor系统, 配置线程个数
*
* @param thread_num	线程数目，至少为1
* @return < 0 : Failed
*/
int NFCTaskModule::InitActorThread(int thread_num, int yieldstrategy)
{
	//根据物理硬件， 确定需要的线程数目
	if (thread_num <= 0) thread_num = 1;

	Theron::Framework::Parameters params;
	params.mThreadCount = thread_num;
	params.mYieldStrategy = (Theron::YieldStrategy)yieldstrategy;

	m_pFramework = new Theron::Framework(params);
	m_pMainActor = new NFTaskActor(*m_pFramework, this);

	return 0;
}

/**
* @brief 向系统请求请求一个actor
*
* @return 返回actor的唯一索引
*/
int NFCTaskModule::RequireActor()
{
	NFTaskActor* pActor(new NFTaskActor(*m_pFramework, this));
	if (pActor)
	{
		if (m_vecActorPool.empty())
		{
			m_vecActorPool.resize(pActor->GetAddress().AsInteger() + 1);
			for (size_t i = 0; i < m_vecActorPool.size(); i++)
			{
				m_vecActorPool[i] = nullptr;
			}
		}
		else
		{
			m_vecActorPool.push_back(nullptr);
		}

		NF_ASSERT((size_t)pActor->GetAddress().AsInteger() < m_vecActorPool.size());
		m_vecActorPool[pActor->GetAddress().AsInteger()] = pActor;

		return pActor->GetAddress().AsInteger();
	}
	return -1;
}

/**
* @brief 通过自己保存的actorIndex将发送数据给actor线程
*
* @param nActorIndex	actor唯一索引
* @param pData			要发送的数据
* @return 是否成功
*/
bool NFCTaskModule::SendMsgToActor(const int nActorIndex, NFTask* pData)
{
	NFTaskActor* pActor = GetActor(nActorIndex);
	return SendMsgToActor(pActor, pData);
}

/**
* @brief 主线程通过自己保存的actorIndex将发送数据给actor线程
*
* @param nActorIndex	actor唯一索引
* @param pData			要发送的数据
* @return 是否成功
*/
bool NFCTaskModule::SendMsgToActor(NFTaskActor* pActor, NFTask* pData)
{
	if (!m_pPluginManager->GetExitApp())
	{
		if (pActor != nullptr && m_pMainActor != nullptr && m_pFramework != nullptr)
		{
			NFTaskActorMessage xMessage;

			xMessage.nMsgType = NFTaskActorMessage::ACTOR_MSG_TYPE_COMPONENT;
			xMessage.pData = pData;
			xMessage.nFromActor = m_pMainActor->GetAddress().AsInteger();

			return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
		}
	}

	return false;
}

/**
* @brief 释放actor资源
*
* @return
*/
void NFCTaskModule::ReleaseActor()
{
	for (size_t i = 0; i < m_vecActorPool.size(); i++)
	{
		if (m_vecActorPool[i])
		{
			NF_SAFE_DELETE(m_vecActorPool[i]);
		}
	}

	if (m_pMainActor)
	{
		NF_SAFE_DELETE(m_pMainActor);
	}
	m_pMainActor = nullptr;

	if (m_pFramework)
	{
		NF_SAFE_DELETE(m_pFramework);
	}
	m_pFramework = nullptr;
}

/**
* @brief 通过actorIndex获得一个actor
*
* @param nActorIndex	actor索引地址
* @return 返回获得的actor, 若没有，为NULL
*/
NFTaskActor* NFCTaskModule::GetActor(const int nActorIndex)
{
	if (nActorIndex >= 0 && nActorIndex < (int)m_vecActorPool.size())
	{
		return m_vecActorPool[nActorIndex];
	}

	return nullptr;
}

/**
* @brief 添加一个Actor组件
*
* @return
*/
bool NFCTaskModule::AddActorComponent(const int nActorIndex, NFITaskComponent* pComonnet)
{
	NFTaskActor* pActor = GetActor(nActorIndex);
	if (pActor == nullptr)
	{
		return false;
	}

	pComonnet->SetActorId(pActor->GetActorId());
	pActor->AddComponnet(pComonnet);
	return true;
}

/**
* @brief 获得所有component
*
* @param
* @return
*/
NFITaskComponent* NFCTaskModule::GetTaskComponent(int nActorIndex)
{
	NFTaskActor* pActor = GetActor(nActorIndex);
	if (pActor == nullptr)
	{
		return nullptr;
	}
	
	return pActor->GetTaskComponent();
}

int NFCTaskModule::GetNumQueuedMessages()
{
	int count = 0;
	if (m_pMainActor)
	{
		count += m_pMainActor->GetNumQueuedMessages();
	}

	for (size_t i = 0; i < m_vecActorPool.size(); i++)
	{
		if (m_vecActorPool[i])
		{
			count += m_vecActorPool[i]->GetNumQueuedMessages();
		}
	}

	return count;
}

bool NFCTaskModule::CloseActorPool()
{
	ReleaseActor();
	m_vecActorPool.clear();
	return true;
}

bool NFCTaskModule::HandlerEx(const NFTaskActorMessage& message, const int from)
{
	if (message.nMsgType != NFTaskActorMessage::ACTOR_MSG_TYPE_COMPONENT)
	{
		return m_mQueue.Push(message);
	}

	return false;
}

bool NFCTaskModule::AddTask(NFTask* pTask)
{
	const int nActorId = GetBalanceActor(pTask->GetBalanceId());
	if (nActorId <= 0)
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return false;
	}

	if (!SendMsgToActor(nActorId, pTask))
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return false;
	}

	return true;
}

bool NFCTaskModule::AddTask(int actorId, NFTask* pTask)
{
	if (actorId <= 0)
	{
		return false;
	}

	if (!SendMsgToActor(actorId, pTask))
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return false;
	}

	return true;
}

/**
* @brief 获得最大Actor Thread Num
*
* @return
*/
int NFCTaskModule::GetMaxThreads()
{
	return m_pFramework->GetMaxThreads();
}

int NFCTaskModule::GetBalanceActor(uint64_t balanceId)
{
	if (balanceId == 0)
	{
		return GetRandActor();
	}

	if (m_vecActorPool.empty())
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return -1;
	}

	//数组的0和1 是空的
	uint32_t index = balanceId % (m_vecActorPool.size()-2) + 2;
	if (m_vecActorPool[index])
	{
		return m_vecActorPool[index]->GetAddress().AsInteger();
	}
	else
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return -1;
	}
}

int NFCTaskModule::GetRandActor()
{
	if (m_vecActorPool.empty())
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return -1;
	}

	uint32_t index = mnSuitIndex++;
	index = (index % (m_vecActorPool.size()-2)) + 2;
	if (m_vecActorPool[index])
	{
		return m_vecActorPool[index]->GetAddress().AsInteger();
	}
	else
	{
		NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "error");
		return -1;
	}
}

/**
* @brief 获取所有ActorId
*
* @return
*/
std::vector<int> NFCTaskModule::GetAllActorId() const
{
	std::vector<int> vecActorId;
	for (size_t index = 0; index < m_vecActorPool.size(); index++)
	{
		if (m_vecActorPool[index])
		{
			vecActorId.push_back(m_vecActorPool[index]->GetAddress().AsInteger());
		}
	}
	return vecActorId;
}

/**
* @brief 记录监控Task
*
* @return
*/
void NFCTaskModule::MonitorTask(NFTask* pTask)
{
	mMonitorMutex.Lock();
	if (pTask->m_taskName.empty() == false)
	{
		TaskMonitorData& data = m_taskMonitorMap[pTask->m_taskName];
		data.mAllUseTime = data.mAllUseTime + pTask->m_useTime;
		data.mCount++;
		data.mPerUseTime = data.mAllUseTime / data.mCount;
	}
	mMonitorMutex.Unlock();
}

/**
* @brief 检查超时
*
* @return
*/
void NFCTaskModule::CheckTimeOutTask()
{
	for (size_t i = 0; i < m_vecActorPool.size(); i++)
	{
		if (m_vecActorPool[i])
		{
			m_vecActorPool[i]->CheckTimeoutTask();
		}
	}
}

void NFCTaskModule::OnMainThreadTick()
{
	std::vector<NFTaskActorMessage> listTask;
	const bool ret = m_mQueue.Pop(listTask);
	if (ret)
	{
		const uint64_t start = NFTime::Tick();
		for (auto it = listTask.begin(); it != listTask.end(); ++it)
		{
			NFTaskActorMessage& xMsg = *it;
			if (xMsg.nMsgType != NFTaskActorMessage::ACTOR_MSG_TYPE_COMPONENT)
			{
				auto pTask = static_cast<NFTask*>(xMsg.pData);
				if (pTask)
				{
					const NFTask::TPTaskState state = pTask->MainThreadProcess();
					switch (state)
					{
					case NFTask::TPTASK_STATE_COMPLETED:
						{
							NF_SAFE_DELETE(pTask);
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
							NF_SAFE_DELETE(pTask);
						}
					}
				}
			}
			else
			{
				//error
				auto pTask = static_cast<NFTask*>(xMsg.pData);
				if (pTask)
				{
					NF_SAFE_DELETE(pTask);
				}
				else
				{
					//error
				}
				NFLogError(NF_LOG_ACTOR_PLUGIN, 0, "task actor module error................");
			}
		}

		if (!listTask.empty())
		{
			//NFLogDebug(NF_LOG_ACTOR_PLUGIN, 0, "handle main thread tick task num:{}, use time:{}", listTask.size(),
			//           NFTime::Tick() - start);
		}
	}

	listTask.clear();
}
