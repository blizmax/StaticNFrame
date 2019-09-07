#pragma once

#include "NFIModule.h"

#include <vector>

class NFTaskActorMessage;
class NFTask;
class NFITaskComponent;

class NFITaskModule : public NFIModule
{
public:
	/**
	* @brief 初始化actor系统, 配置线程个数
	*
	* @param thread_num	线程数目，至少为1
	* @return < 0 : Failed
	*/
	virtual int InitActorThread(int thread_num) = 0;

	/**
	* @brief 消息数据处理完后，如果有必要将数据返回给主线程，
	*		 在这个函数里，将消息数据推送给主线程，这个函数在actor线程中执行
	*
	* @param message	消息数据
	* @param from	发送消息的actor地址
	* @return 是否成功
	*/
	virtual bool HandlerEx(const NFTaskActorMessage& message, int from) = 0;

	/**
	* @brief 添加要异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	virtual bool AddTask(NFTask* pTask) = 0;

	/**
	* @brief 添加要异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	virtual bool AddTask(int actorId, NFTask* pTask) = 0;

	/**
	* @brief 向系统请求请求一个actor
	*
	* @return 返回actor的唯一索引
	*/
	virtual int RequireActor() = 0;

	/**
	* @brief 获得系统还没有处理完的任务数目
	*
	* @return 未完成的任务数目
	*/
	virtual int GetNumQueuedMessages() = 0;

	/**
	* @brief 添加一个Actor组件
	*
	* @return
	*/
	virtual bool AddActorComponent(int nActorIndex, NFITaskComponent* pComonnet) = 0;

	/**
	* @brief 获得所有component
	*
	* @param
	* @return
	*/
	virtual const std::vector<NFITaskComponent*>& GetTaskComponent(int nActorIndex) = 0;

	/**
	* @brief 获得最大Actor Thread Num
	*
	* @return
	*/
	virtual int GetMaxThreads() = 0;
};
