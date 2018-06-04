#pragma once

#include "NFComm/NFCore/NFQueue.hpp"
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFActorMgr.h"
#include "NFDBActor.h"

#include <vector>


class NFTask;

/**
* @brief 异步处理DB管理类
*/
class NFDBActorMgr : public NFActorMgr<NFDBActor>, public NFSingleton<NFDBActorMgr>
{
public:
	/**
	* @brief 构造函数
	*/
	NFDBActorMgr();

	/**
	* @brief 析构函数
	*/
	virtual ~NFDBActorMgr();

	/**
	* @brief 初始化DB以及actor系统
	*
	* @param dbInfo		db配置
	* @param actor_num	actor个数
	* @return
	*/
	virtual int Init(int actor_num = 10);

	/**
	 * @brief 释放资源
	 *
	 * @return int 
	 */
	virtual int UnInit();

	/**
	* @brief 向系统申请actorNum个actor, 初始化actor池
	*
	* @param actorNum	actor数目
	* @return 是否成功
	*/
	virtual bool StartActorPool(int actorNum);

	/**
	* @brief 释放申请的actor数据，关闭actor池
	*
	* @return 是否成功
	*/
	virtual bool CloseActorPool();

	/**
	* @brief 消息数据处理完后，如果有必要将数据返回给主线程，
	*		 在这个函数里，将消息数据推送给主线程，这个函数在actor线程中执行
	*
	* @param messag	消息数据
	* @param from	发送消息的actor地址
	* @return 是否成功
	*/
	virtual bool HandlerEx(const NFXActorMessage& message, const int from);

	/**
	* @brief 通过任务的动态均衡id，获得actor
	*		 为了防止数据库错乱，防止同时对数据库表中的一条数据，读取写入，
	*		 使用动态均衡id, 使得在某个时候只有一条线程对表中的一条数据，读取或写入
	* @param balanceId 动态均衡id
	* @return	一个actor索引
	*/
	int			GetBalanceActor(uint64_t balanceId);

	/**
	* @brief 随机获得一个actor
	*
	* @return actor索引
	*/
	int			GetRandActor();

	/**
	* @brief 添加要异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	bool		AddTask(NFTask* pTask);

	/**
	* @brief 主线程处理actor返回的任务
	*
	* @return
	*/
	void		OnMainThreadTick();
protected:
	/**
	* @brief actor索引数组
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief 用来平衡随机获得actor
	*/
	int mnSuitIndex;

	/**
	* @brief 接收的任务数目
	*
	*/
	int nRecvTaskCount;

	/**
	* @brief 处理的任务数目
	* @return
	*/
	int nHandleTaskCount;

	/**
	* @brief 返回的消息队列，线程安全, 
	* actor线程将数据放入队列， 主线程从队列里取数据处理
	*/
	NFQueue<NFXActorMessage>		m_mQueue;
};