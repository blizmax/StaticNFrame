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

#include "NFComm/NFPluginModule/NFITaskComponent.h"
#include "NFComm/NFCore/NFTime.h"

NFCTaskModule::NFCTaskModule(NFIPluginManager* p)
{
	m_loopCount = 0;
	m_pPluginManager = p;
	mnSuitIndex = 0;
	nRecvTaskCount = 0;
	nHandleTaskCount = 0;
	srand(static_cast<unsigned>(time(nullptr)));
	//���ȳ�ʼ��
	//NFCTaskModule::InitActorThread(std::thread::hardware_concurrency());
	NFCTaskModule::InitActorThread(10);
}

NFCTaskModule::~NFCTaskModule() = default;

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
	//�ȴ��첽������ϣ�Ȼ�����˳�ϵͳ
	while (GetNumQueuedMessages() > 0 || m_mQueue.Count() > 0)
	{
		OnMainThreadTick();
		NFSLEEP(1);
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
	if (m_loopCount % 1000 == 0)
	{
		for (auto iter = m_taskMonitorMap.begin(); iter != m_taskMonitorMap.end(); iter++)
		{
			NFLogDebug(NF_LOG_ACTOR_PLUGIN, 0, "task:{} use all time:{}, count:{}, per use time:{}", iter->first, iter->second.mAllUseTime, iter->second.mCount, iter->second.mPerUseTime);
		}
	}
	return true;
}

/**
* @brief ��ʼ��actorϵͳ, �����̸߳���
*
* @param thread_num	�߳���Ŀ������Ϊ1
* @return < 0 : Failed
*/
int NFCTaskModule::InitActorThread(int thread_num)
{
	//��������Ӳ���� ȷ����Ҫ���߳���Ŀ
	if (thread_num <= 0) thread_num = 1;

	m_pFramework = new Theron::Framework(thread_num);

	m_pMainActor = new NFTaskActor(*m_pFramework, this);

	return 0;
}

/**
* @brief ��ϵͳ��������һ��actor
*
* @return ����actor��Ψһ����
*/
int NFCTaskModule::RequireActor()
{
	NFTaskActor* pActor(new NFTaskActor(*m_pFramework, this));
	if (pActor)
	{
		m_mActorMap.emplace(pActor->GetAddress().AsInteger(), pActor);
		m_vecActorPool.push_back(pActor->GetAddress().AsInteger());
		return pActor->GetAddress().AsInteger();
	}
	return -1;
}

/**
* @brief ���߳�ͨ���Լ������actorIndex���������ݸ�actor�߳�
*
* @param nActorIndex	actorΨһ����
* @param pData			Ҫ���͵�����
* @return �Ƿ�ɹ�
*/
bool NFCTaskModule::SendMsgToActor(const int nActorIndex, NFTask* pData)
{
	NFTaskActor* pActor = GetActor(nActorIndex);
	if (pActor != nullptr && m_pMainActor != nullptr && m_pFramework != nullptr)
	{
		NFTaskActorMessage xMessage;

		xMessage.nMsgType = NFTaskActorMessage::ACTOR_MSG_TYPE_COMPONENT;
		xMessage.pData = pData;
		xMessage.nFromActor = m_pMainActor->GetAddress().AsInteger();

		return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
	}

	return false;
}

/**
* @brief �ͷ�actor��Դ
*
* @return
*/
void NFCTaskModule::ReleaseActor()
{
	for (auto it = m_mActorMap.begin(); it != m_mActorMap.end(); ++it)
	{
		NF_SAFE_DELETE(it->second);
	}
	m_mActorMap.clear();
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
* @brief ͨ��actorIndex���һ��actor
*
* @param nActorIndex	actor������ַ
* @return ���ػ�õ�actor, ��û�У�ΪNULL
*/
NFTaskActor* NFCTaskModule::GetActor(const int nActorIndex)
{
	const auto iter = m_mActorMap.find(nActorIndex);
	if (iter != m_mActorMap.end())
	{
		return iter->second;
	}
	return nullptr;
}

/**
* @brief ���һ��Actor���
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

	pComonnet->SetTaskActor(pActor);
	pActor->AddComponnet(pComonnet);
	return true;
}

/**
* @brief �������component
*
* @param
* @return
*/
const std::vector<NFITaskComponent*>& NFCTaskModule::GetTaskComponent(int nActorIndex)
{
	static std::vector<NFITaskComponent*> emptyVec;
	NFTaskActor* pActor = GetActor(nActorIndex);
	if (pActor == nullptr)
	{
		return emptyVec;
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

	for (auto it = m_mActorMap.begin(); it != m_mActorMap.end(); ++it)
	{
		count += it->second->GetNumQueuedMessages();
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
		return false;
	}

	if (!SendMsgToActor(nActorId, pTask))
	{
		return false;
	}

	nRecvTaskCount++;
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
		return false;
	}

	nRecvTaskCount++;
	return true;
}

int NFCTaskModule::GetBalanceActor(uint64_t balanceId)
{
	if (balanceId == 0)
	{
		return GetRandActor();
	}
	if (m_vecActorPool.empty())
	{
		return -1;
	}
	mnSuitIndex = balanceId % m_vecActorPool.size();
	return m_vecActorPool[mnSuitIndex];
}

int NFCTaskModule::GetRandActor()
{
	if (m_vecActorPool.empty())
	{
		return -1;
	}

	mnSuitIndex++;
	mnSuitIndex = mnSuitIndex % m_vecActorPool.size();

	return m_vecActorPool[mnSuitIndex];
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
					if (pTask->m_taskName.empty() == false && pTask->m_useTime > 1)
					{
						TaskMonitorData& data = m_taskMonitorMap[pTask->m_taskName];
						data.mAllUseTime = data.mAllUseTime + pTask->m_useTime;
						data.mCount++;
						data.mPerUseTime = data.mAllUseTime / data.mCount;
					}

					const NFTask::TPTaskState state = pTask->MainThreadProcess();
					switch (state)
					{
					case NFTask::TPTASK_STATE_COMPLETED:
						{
							nHandleTaskCount++;
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
							nHandleTaskCount++;
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
