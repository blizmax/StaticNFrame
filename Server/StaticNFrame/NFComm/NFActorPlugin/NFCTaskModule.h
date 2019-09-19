// -------------------------------------------------------------------------
//    @FileName         :    NFCTaskModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFActorPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFTaskActor.h"
#include "NFComm/NFCore/NFMutex.h"
#include "NFComm/NFCore/NFLock.h"
#include "NFComm/NFCore/NFAtomic.h"
#include <map>

class NFCTaskModule final : public NFITaskModule
{
public:
	explicit NFCTaskModule(NFIPluginManager* p);
	virtual ~NFCTaskModule();

	bool Awake() override;

	bool Init() override;

	bool AfterInit() override;

	bool BeforeShut() override;

	bool Shut() override;

	bool Finalize() override;

	bool Execute() override;
public:
	/**
	* @brief ��ʼ��actorϵͳ, �����̸߳���
	*
	* @param thread_num	�߳���Ŀ������Ϊ1
	* @return < 0 : Failed
	*/
	virtual int InitActorThread(int thread_num, int yieldstrategy = 0);

	/**
	* @brief ��ϵͳ��������һ��actor
	*
	* @return ����actor��Ψһ����
	*/
	virtual int RequireActor() override;

	/**
	* @brief ���һ��Actor���
	*
	* @return 
	*/
	virtual bool AddActorComponent(int nActorIndex, NFITaskComponent* pComonnet) override;

	/**
	* @brief �������component
	*
	* @param
	* @return
	*/
	virtual NFITaskComponent* GetTaskComponent(int nActorIndex) override;

	/**
	* @brief ���߳�ͨ���Լ������actorIndex���������ݸ�actor�߳�
	*
	* @param nActorIndex	actorΨһ����
	* @param pData			Ҫ���͵�����
	* @return �Ƿ�ɹ�
	*/
	virtual bool SendMsgToActor(int nActorIndex, NFTask* pData);

	/**
	* @brief ���߳�ͨ���Լ������actorIndex���������ݸ�actor�߳�
	*
	* @param nActorIndex	actorΨһ����
	* @param pData			Ҫ���͵�����
	* @return �Ƿ�ɹ�
	*/
	virtual bool SendMsgToActor(NFTaskActor* pActor, NFTask* pData);

	/**
	* @brief �ͷ�actor��Դ
	*
	* @return
	*/
	virtual void ReleaseActor();

	/**
	* @brief ͨ��actorIndex���һ��actor
	*
	* @param nActorIndex	actor������ַ
	* @return ���ػ�õ�actor, ��û�У�ΪNULL
	*/
	virtual NFTaskActor* GetActor(int nActorIndex);

	/**
	* @brief ���ϵͳ��û�д������������Ŀ
	*
	* @return δ��ɵ�������Ŀ
	*/
	virtual int GetNumQueuedMessages() override;

	/**
	* @brief �ͷ������actor���ݣ��ر�actor��
	*
	* @return �Ƿ�ɹ�
	*/
	virtual bool CloseActorPool();

	/**
	* @brief ��Ϣ���ݴ����������б�Ҫ�����ݷ��ظ����̣߳�
	*		 ��������������Ϣ�������͸����̣߳����������actor�߳���ִ��
	*
	* @param message	��Ϣ����
	* @param from	������Ϣ��actor��ַ
	* @return �Ƿ�ɹ�
	*/
	virtual bool HandlerEx(const NFTaskActorMessage& message, int from) override;

	/**
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceActor(uint64_t balanceId);

	/**
	* @brief ������һ��actor
	*
	* @return actor����
	*/
	int GetRandActor();

	/**
	* @brief ���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	virtual bool AddTask(NFTask* pTask) override;

	/**
	* @brief ���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	virtual bool AddTask(int actorId, NFTask* pTask) override;

	/**
	* @brief ���̴߳���actor���ص�����
	*
	* @return
	*/
	void OnMainThreadTick();

	/**
	* @brief ������Actor Thread Num
	*
	* @return
	*/
	virtual int GetMaxThreads() override;

	/**
	* @brief ��¼���Task
	*
	* @return
	*/
	virtual void MonitorTask(NFTask* pTask);

	/**
	* @brief ��ȡ����ActorId
	*
	* @return
	*/
	virtual std::vector<int> GetAllActorId() const override;
protected:
	Theron::Framework* m_pFramework;
	NFTaskActor* m_pMainActor;
protected:
	/**
	* @brief actor��������
	*/
	std::vector<NFTaskActor*> m_vecActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	std::atomic<uint32_t> mnSuitIndex;

	/**
	* @brief ���ص���Ϣ���У��̰߳�ȫ,
	* actor�߳̽����ݷ�����У� ���̴߳Ӷ�����ȡ���ݴ���
	*/
	NFQueueVector<NFTaskActorMessage> m_mQueue;

	class TaskMonitorData
	{
	public:
		TaskMonitorData()
		{
			mAllUseTime = 0;
			mPerUseTime = 0;
			mCount = 0;
		}
		uint64_t mAllUseTime;
		uint64_t mPerUseTime;
		uint64_t mCount;
	};
	
	uint32_t m_loopCount;

	//task ��� static� ��Ϊ taskmodule �����кܶ��
	std::map<std::string, TaskMonitorData> m_taskMonitorMap;

	/**
	* @brief actor����������
	*/
	mutable NFMutex mMonitorMutex;

};
