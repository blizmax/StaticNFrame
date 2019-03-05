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

class NFCTaskModule : public NFITaskModule
{
public:
	explicit NFCTaskModule(NFIPluginManager* p);
	virtual ~NFCTaskModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;

	virtual bool Execute() override;
public:
	/**
	* @brief ��ʼ��actorϵͳ, �����̸߳���
	*
	* @param thread_num	�߳���Ŀ������Ϊ1
	* @return < 0 : Failed
	*/
	virtual int InitActorThread(int thread_num);

	/**
	* @brief ��ϵͳ��������һ��actor
	*
	* @return ����actor��Ψһ����
	*/
	virtual int RequireActor();

	/**
	* @brief ���߳�ͨ���Լ������actorIndex���������ݸ�actor�߳�
	*
	* @param nActorIndex	actorΨһ����
	* @param pData			Ҫ���͵�����
	* @return �Ƿ�ɹ�
	*/
	virtual bool SendMsgToActor(const int nActorIndex, NFTask* pData);

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
	virtual NFTaskActor* GetActor(const int nActorIndex);

	/**
	* @brief ���ϵͳ��û�д������������Ŀ
	*
	* @return δ��ɵ�������Ŀ
	*/
	virtual int GetNumQueuedMessages();

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
	* @param messag	��Ϣ����
	* @param from	������Ϣ��actor��ַ
	* @return �Ƿ�ɹ�
	*/
	virtual bool HandlerEx(const NFTaskActorMessage& message, const int from);

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
	bool AddTask(NFTask* pTask);

	/**
	* @brief ���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddTask(int actorId, NFTask* pTask);

	/**
	* @brief ���̴߳���actor���ص�����
	*
	* @return
	*/
	void OnMainThreadTick();
protected:
	Theron::Framework* m_pFramework;
	NFTaskActor* m_pMainActor;
	std::map<int, NFTaskActor*> m_mActorMap;

protected:
	/**
	* @brief actor��������
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	int mnSuitIndex;

	/**
	* @brief ���յ�������Ŀ
	*
	*/
	int nRecvTaskCount;

	/**
	* @brief �����������Ŀ
	* @return
	*/
	int nHandleTaskCount;

	/**
	* @brief ���ص���Ϣ���У��̰߳�ȫ,
	* actor�߳̽����ݷ�����У� ���̴߳Ӷ�����ȡ���ݴ���
	*/
	NFQueueVector<NFTaskActorMessage> m_mQueue;
};
