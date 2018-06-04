// -------------------------------------------------------------------------
//    @FileName         :    JyActorMgr.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-19
//    @Email			:    445267987@qq.com
//    @Module           :    
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFXActor.h"

class NFTask;

/**
* @brief actor异步管理基类
*
*/
class NFXActorMgr
{
public:
	/**
	* @brief 构造函数
	*/
	NFXActorMgr()
	{
		nMainThreadRuning = true;
		nIsInit = false;
	}

	/**
	* @brief 析构函数
	*/
	virtual ~NFXActorMgr()
	{

	}

	/**
	* @brief 是否初始化
	*
	* @return 是否初始化
	*/
	virtual bool IsInit()
	{
		return nIsInit;
	}

	/**
	* @brief 初始化actor系统, 配置线程个数
	*
	* @param thread_num	线程数目，至少为1
	* @return < 0 : Failed
	*/
	virtual int InitActor(int thread_num) = 0;

	/**
	* @brief 向系统请求请求一个actor
	*
	* @return 返回actor的唯一索引
	*/
	virtual int RequireActor() = 0;

	/**
	* @brief 释放actor资源
	*
	* @return
	*/
	virtual void ReleaseActor() = 0;

	/**
	* @brief 消息数据处理完后，如果有必要将数据返回给主线程，
	*		 在这个函数里，将消息数据推送给主线程，这个函数在actor线程中执行
	*
	* @param messag	消息数据
	* @param from	发送消息的actor地址
	* @return 是否成功
	*/
	virtual bool HandlerEx(const NFXActorMessage& message, const int from) = 0;

	/**
	* @brief 主线程通过自己保存的actorIndex将发送数据给actor线程
	*
	* @param nActorIndex	actor唯一索引
	* @param pData			要发送的数据
	* @return 是否成功
	*/
	virtual bool SendMsgToActor(const int nActorIndex, void* pData) = 0;

	/**
	* @brief 主线程通过自己保存的actorIndex将发送数据给actor线程
	*
	* @param nActorIndex	actor唯一索引
	* @param strData		要发送的数据
	* @return 是否成功
	*/
	virtual bool SendMsgToActor(const int nActorIndex, const std::string& strData) = 0;
    
	/**
	* @brief 通过actorIndex获得一个actor
	*
	* @param nActorIndex	actor索引地址
	* @return 返回获得的actor, 若没有，为NULL
	*/
	virtual NFXActor* GetActor(const int nActorIndex) = 0;

	/**
	* @brief 向系统申请actorNum个actor, 初始化actor池
	*
	* @param actorNum	actor数目
	* @return 是否成功
	*/
	virtual bool StartActorPool(int actorNum) = 0;

	/**
	* @brief 释放申请的actor数据，关闭actor池
	*
	* @return 是否成功
	*/
	virtual bool CloseActorPool() = 0;

	/**
	* @brief 获得系统还没有处理完的任务数目
	*
	* @return 未完成的任务数目
	*/
	virtual int GetNumQueuedMessages() = 0;

	/**
	* @brief 添加要异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	virtual bool AddTask(NFTask* pTask) = 0;

	/**
	* @brief 添加要异步处理的数据， 数据放在字符串中
	*
	* @param strData 要异步处理的数据
	* @return
	*/
	virtual bool AddStrData(const std::string& strData) = 0;

	/**
	* @brief 主线程处理actor返回的任务
	*
	* @return
	*/
	virtual void OnMainThreadTick() = 0;

	/**
	* @brief 服务器框架需要, 服务器退出的时候，
	*        用来判断异步的任务是否全部完成, 异步任务全部完成后，
	*        服务器才能退出
	*
	* @return
	*/
	virtual bool IsSaveDone()
	{
		if (nIsInit == false) return true;

		if (GetNumQueuedMessages() > 0)
		{
			return false;
		}
		return true;
	}

	/**
	* @brief 设置服务器主线程是否在执行
	*
	* @return
	*/
	void		SetMainThreadRunning(bool running) { nMainThreadRuning = running; }
protected:
	/**
	* @brief 服务器主线程是否在执行，
	*        服务器退出的时候，设置为false，不在有新的任务进入异步系统
	*/
	bool nMainThreadRuning;
	/**
	* @brief actormgr是否完成了初始化
	*/
	bool nIsInit;
};

/**
* @brief actor异步管理模版, ActorType是actor具体的actor类型
*
*/
template<class ActorType>
class NFActorMgr : public NFXActorMgr
{
public:
	/**
	* @brief 构造函数
	*/
	NFActorMgr()
	{
		m_pFramework = nullptr;
		m_pMainActor = nullptr;
	}

	/**
	* @brief 析构函数
	*/
	virtual ~NFActorMgr()
	{
		NFActorMgr<ActorType>::ReleaseActor();
	}
public:
	/**
	* @brief 初始化actor系统, 配置线程个数
	*
	* @param thread_num	线程数目，至少为1
	* @return < 0 : Failed
	*/
	virtual int InitActor(int thread_num) override
	{
		//根据物理硬件， 确定需要的线程数目
		if (thread_num <= 0) thread_num = 1;

		m_pFramework = new Theron::Framework(thread_num);
		
		m_pMainActor = new ActorType(*m_pFramework, this);

		return 0;
	}

	/**
	* @brief 向系统请求请求一个actor
	*
	* @return 返回actor的唯一索引
	*/
	virtual int RequireActor() override
	{
		NFXActor* pActor(new ActorType(*m_pFramework, this));
		if (pActor)
		{
			m_mActorMap.emplace(pActor->GetAddress().AsInteger(), pActor);
			return pActor->GetAddress().AsInteger();
		}
		return -1;
	}

	/**
	* @brief 主线程通过自己保存的actorIndex将发送数据给actor线程
	*
	* @param nActorIndex	actor唯一索引
	* @param pData			要发送的数据
	* @return 是否成功
	*/
	virtual bool SendMsgToActor(const int nActorIndex, void* pData) override
	{
		NFXActor* pActor = GetActor(nActorIndex);
		if (pActor != nullptr && m_pMainActor != nullptr && m_pFramework != nullptr)
		{
			NFXActorMessage xMessage;

			xMessage.nMsgType = NFXActorMessage::ACTOR_MSG_TYPE_COMPONENT;
			xMessage.pData = pData;
			xMessage.nFromActor = m_pMainActor->GetAddress().AsInteger();

			return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
		}

		return false;
	}

	/**
	* @brief 主线程通过自己保存的actorIndex将发送数据给actor线程
	*
	* @param nActorIndex	actor唯一索引
	* @param strData		要发送的数据
	* @return 是否成功
	*/
	virtual bool SendMsgToActor(const int nActorIndex, const std::string& strData) override
	{
		NFXActor* pActor = GetActor(nActorIndex);
		if (pActor != nullptr && m_pMainActor != nullptr && m_pFramework != nullptr)
		{
			NFXActorMessage xMessage;

			xMessage.nMsgType = NFXActorMessage::ACTOR_MSG_TYPE_COMPONENT;
			xMessage.strData = strData;
			xMessage.nFromActor = m_pMainActor->GetAddress().AsInteger();

			return m_pFramework->Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
		}

		return false;
	}
    
	/**
	* @brief 释放actor资源
	*
	* @return
	*/
	virtual void ReleaseActor() override
	{
		for (auto it = m_mActorMap.begin(); it != m_mActorMap.end(); it++)
		{
			NFSafeDelete(it->second);
		}
		m_mActorMap.clear();
		if (m_pMainActor)
		{
			NFSafeDelete(m_pMainActor);
		}
		m_pMainActor = nullptr;
		if (m_pFramework)
		{
			NFSafeDelete(m_pFramework);
		}
		m_pFramework = nullptr;
	}

	/**
	* @brief 通过actorIndex获得一个actor
	*
	* @param nActorIndex	actor索引地址
	* @return 返回获得的actor, 若没有，为NULL
	*/
	virtual NFXActor* GetActor(const int nActorIndex) override
	{
		auto iter = m_mActorMap.find(nActorIndex);
		if (iter != m_mActorMap.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	/**
	* @brief 获得系统还没有处理完的任务数目
	*
	* @return 未完成的任务数目
	*/
	virtual int GetNumQueuedMessages() override
	{
		if (nIsInit == false) return 0;

		int count = 0;
		if (m_pMainActor)
		{
			count += m_pMainActor->GetNumQueuedMessages();
		}

		for (auto it = m_mActorMap.begin(); it != m_mActorMap.end(); it++)
		{
			count += it->second->GetNumQueuedMessages();
		}

		return count;
	}

	/**
	* @brief 添加要异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	virtual bool AddTask(NFTask* pTask) override
	{
		return true;
	}

	/**
	* @brief 添加要异步处理的数据， 数据放在字符串中
	*
	* @param strData 要异步处理的数据
	* @return
	*/
	virtual bool AddStrData(const std::string& strData) override
	{
		return true;
	}

	/**
	* @brief 消息数据处理完后，如果有必要将数据返回给主线程，
	*		 在这个函数里，将消息数据推送给主线程，这个函数在actor线程中执行
	*
	* @param messag	消息数据
	* @param from	发送消息的actor地址
	* @return 是否成功
	*/
	virtual bool HandlerEx(const NFXActorMessage& message, const int from) override
	{
		return true;
	}

	/**
	* @brief 主线程处理actor返回的任务
	*
	* @return
	*/
	virtual void OnMainThreadTick() override
	{

	}
protected:
	Theron::Framework*			m_pFramework;
	NFXActor*					m_pMainActor;
	std::map<int, NFXActor*>	m_mActorMap;
};
