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
	* @brief 添加一个Actor组件
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
	* @brief 通过任务的动态均衡id，获得actor
	*		 为了防止数据库错乱，防止同时对数据库表中的一条数据，读取写入，
	*		 使用动态均衡id, 使得在某个时候只有一条线程对表中的一条数据，读取或写入
	* @param balanceId 动态均衡id
	* @return	一个actor索引
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
	* @brief 随机获得一个actor
	*
	* @return actor索引
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
	* @brief 通过平衡ID添加要异步处理的task
	*
	* @param pTask 要异步处理的task
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
	* @brief actor索引数组
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief 用来平衡随机获得actor
	*/
	size_t mnSuitIndex = 0;
};