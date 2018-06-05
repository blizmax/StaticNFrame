#pragma once

#define THERON_USE_STD_THREADS 1

#include <map>
#include <Theron/Theron.h>

class NFXActorMgr;

/**
* @brief actor消息数据
*
*/
class NFXActorMessage
{
public:
	/**
	* @brief 消息类型
	*
	*/
	enum MessageType
	{
		/**
		* @brief 要处理的消息
		*
		*/
		ACTOR_MSG_TYPE_COMPONENT,
		/**
		* @brief 已经被处理的消息，等待返回主线程处理
		*
		*/
		ACTOR_MSG_TYPE_END_FUNC,
	};

	/**
	* @brief 构造函数
	*
	*/
	NFXActorMessage()
	{
		nFromActor = 0;
		pData = nullptr;
		nMsgType = ACTOR_MSG_TYPE_COMPONENT;
	}
public:
	/**
	* @brief 消息类型
	*
	*/
	int nMsgType;

	/**
	* @brief 发送消息的actor索引
	*
	*/
	int nFromActor;

	/**
	* @brief 消息数据
	*
	*/
	void* pData;

	/**
	* @brief 字符串携带消息数据
	*
	*/
	std::string strData;
};

/**
* @brief actor基类
*
*/
class NFXActor : public Theron::Actor
{
public:
	/**
	* @brief 构造函数
	*        必须调用RegisterHandler，注册异步过程中用来处理的函数
	*/
	NFXActor(Theron::Framework& framework, NFXActorMgr* pActorMgr) : Theron::Actor(framework)
	{
		RegisterHandler(this, &NFXActor::DefaultHandler);
		m_pActorMgr = pActorMgr;
	}

	/**
	* @brief 析构函数
	*
	*/
	virtual ~NFXActor() {}

	/**
	* @brief 向另外一个actor发送消息
	*
	* @param address 接受消息的actor的地址
	* @param message 发现的消息
	* @return 返回发送是否成功
	*/
	virtual bool SendMsg(const Theron::Address address, const NFXActorMessage& message)
	{
		return Send(message, address);
	}
protected:
	/**
	* @brief 处理已经被处理过的消息
	*
	* @param message 要处理的消息
	* @param from	 发送消息的actor地址
	* @return 返回发送是否成功
	*/
	virtual void HandlerEx(const NFXActorMessage& message, const Theron::Address from) {};

	/**
	* @brief 处理发送的数据
	*
	* @param message 要处理的消息
	* @param from	 发送消息的actor地址
	* @return
	*/
	virtual void Handler(const NFXActorMessage& message, const Theron::Address from) {};
private:
	/**
	* @brief 异步处理的过程
	*
	* @param message 要处理的消息
	* @param from	 发送消息的actor地址
	* @return
	*/
	void DefaultHandler(const NFXActorMessage& message, const Theron::Address from)
	{
		if (message.nMsgType == NFXActorMessage::ACTOR_MSG_TYPE_COMPONENT)
		{
			Handler(message, from);
		}
		else
		{
			HandlerEx(message, from);
		}
	}
protected:
	/**
	* @brief actor管理基类
	*/
	NFXActorMgr* m_pActorMgr;
};
