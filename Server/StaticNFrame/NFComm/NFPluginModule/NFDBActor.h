#pragma once

#include "NFXActor.h"

class NFXActorMgr;
class CDB;
class NFTask;

/**
* @brief 异步写DB的actor
*
*/
class NFDBActor : public NFXActor
{
public:
	/**
	* @brief 构造函数
	*/
	NFDBActor(Theron::Framework& framework, NFXActorMgr* pActorMgr);

	/**
	* @brief 析构函数
	*/
	virtual ~NFDBActor();

	/**
	* @brief 初始化DB系统，将通过stDBInfo配置连接数据库
	*
	* @param fo 数据库配置
	* @return true连接数据库成功, false失败
	*/
	bool Init();

	/**
	* @brief 在actor线程处理消息
	*
	* @param message 要处理的消息数据
	* @param from    发消息的actor的地址
	* @return
	*/
	virtual void Handler(const NFXActorMessage& message, const Theron::Address from);

	/**
	* @brief 将被处理过的消息返回给主线程
	*
	* @param message 被处理过的消息
	* @param from    发消息的actor的地址
	* @return
	*/
	virtual void HandlerEx(const NFXActorMessage& message, const Theron::Address from);
public:
	/**
	* @brief 处理task前的处理
	*
	* @param pTask 要处理的任务
	* @return
	*/
	void onProcessTaskStart(NFTask* pTask);
	/**
	* @brief task处理后的处理
	*
	* @param pTask 要处理的任务
	* @return
	*/
	void onProcessTaskEnd(NFTask* pTask);
	/**
	* @brief 异步处理任务系统
	*
	* @param pTask 要处理的任务
	* @return
	*/
	void processTask(NFTask* pTask);
private:
	/**
	* @brief 数据库是否初始化
	*/
	bool m_bInit;
};

