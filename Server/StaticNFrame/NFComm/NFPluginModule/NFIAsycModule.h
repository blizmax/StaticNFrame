// -------------------------------------------------------------------------
//    @FileName         :    NFIAsycModule.h
//    @Author           :    Gao.Yi
//    @Date             :    2019-06-12
//    @Module           :    NFIAsycModule
//
//
//                    .::::.
//                  .::::::::.
//                 :::::::::::  FUCK YOU
//             ..:::::::::::'
//           '::::::::::::'
//             .::::::::::
//        '::::::::::::::..
//             ..::::::::::::.
//           ``::::::::::::::::
//            ::::``:::::::::'        .:::.
//           ::::'   ':::::'       .::::::::.
//         .::::'      ::::     .:::::::'::::.
//        .:::'       :::::  .:::::::::' ':::::.
//       .::'        :::::.:::::::::'      ':::::.
//      .::'         ::::::::::::::'         ``::::.
//  ...:::           ::::::::::::'              ``::.
// ```` ':.          ':::::::::'                  ::::..
//                    '.:::::'                    ':'````..
//
// -------------------------------------------------------------------------


#pragma once

#include "NFIModule.h"
#include <vector>
#include "NFITaskModule.h"
#include "NFTask.h"

class NFIAsycModule : public NFIModule
{
public:
	NFIAsycModule()
	{

	}

	virtual ~NFIAsycModule()
	{

	}

	/**
	* @brief ���һ��Actor���
	*
	* @return
	*/
	virtual bool AddActorComponent(NFITaskComponent* pComonnet)
	{
		int actorId = FindModule<NFITaskModule>()->RequireActor();
		if (actorId <= 0)
		{
			return false;
		}

		FindModule<NFITaskModule>()->AddActorComponent(actorId, pComonnet);

		m_vecActorPool.push_back(actorId);
		return true;
	}

	/**
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceActor(uint64_t balanceId)
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

	/**
	* @brief ������һ��actor
	*
	* @return actor����
	*/
	int GetRandActor()
	{
		if (m_vecActorPool.size() <= 0)
		{
			return -1;
		}

		mnSuitIndex++;
		mnSuitIndex = mnSuitIndex % m_vecActorPool.size();

		return m_vecActorPool[mnSuitIndex];
	}

	/**
	* @brief ͨ��ƽ��ID���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddTask(NFTask* pTask)
	{
		if (pTask)
		{
			int actorId = GetBalanceActor(pTask->GetBalanceId());
			if (actorId > 0)
			{
				return FindModule<NFITaskModule>()->AddTask(actorId, pTask);
			}
		}

		return false;
	}

protected:
	/**
	* @brief actor��������
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	size_t mnSuitIndex = 0;
};