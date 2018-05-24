#pragma once

#include "NetObject.h"
#include "NFClient.h"
#include <thread>
#include "NFComm/NFCore/NFThread.hpp"
#include "NFComm/NFCore/NFQueue.hpp"

/**
*@brief  多线程libevent网络客户端封装类.
*/
class NFThreadClient : public NFClient
{
public:
	/**
	 *@brief  构造函数.
	 */
	NFThreadClient(uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFThreadClient();

	/**
	 * @brief	初始化
	 *
	 * @return 是否成功
	 */
	virtual bool Init() override;

	/**
	 * @brief	关闭客户端释放数据
	 *
	 * @return  是否成功
	 */
	virtual bool Shut() override;

	/**
	 * @brief	服务器每帧执行
	 *
	 * @return	是否成功
	 */
	virtual bool Execute() override;

	/**
	 * @brief	对解析出来的数据进行处理
	 *
	 * @param type    数据类型，主要是为了和多线程统一处理, 主要有接受数据处理，连接成功处理，断开连接处理
	 * @param usLink  本客户端的唯一id
	 * @param pBuf    数据指针
	 * @param sz      数据大小
	 * @param nMsgId  分析出来的消息id
	 * @param nValue  消息头携带的值，可能是玩家ID，也可能是对方客户端连接的唯一id
	 * @return
	 */
	virtual void OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue) override;

	/**
	 * @brief	处理多线程网络数据, 从多线程队列取出数据并处理
	 *
	 * @return	
	 */
	virtual void ProcessMsgLogicThread();

	/**
	 * @brief	检查连接
	 * @return
	 */
	virtual void CheckConnect() override;

	/**
	 * @brief	开启线程处理网络
	 * @return
	 */
	void StartThread();

	/**
	 * @brief	发送链接成功的消息给自己, 因为是多线程， 为避免过多的锁， 
	 * 采用发送消息的机制，通知主线程网络连接状况
	 * @return
	 */
	void SendConnected();

	/**
	 * @brief	发送链接断开的消息给自己, 因为是多线程， 为避免过多的锁， 
	 * 采用发送消息的机制，通知主线程网络连接状况
	 * @return
	 */
	void SendDisconnect();

	/**
	 * @brief	关闭连接
	 *
	 * @return
	 */
	virtual void Close() override;

	/**
	 * @brief	开始连接
	 * @return
	 */
	virtual bool Connect() override;

	/**
	 * @brief	断线重连
	 *
	 * @return
	 */
	virtual bool Reconnect() override;

	/**
	 * @brief	处理连接成功
	 *
	 * @param nSocket	系统分配的socket
	 * @return
	 */
	virtual void OnHandleConnect(SOCKET nSocket) override;

	/**
	 * @brief	处理断开连接
	 *
	 * @return
	 */
	virtual void OnHandleDisConnect() override;

	/**
	 * @brief	客户端线程运行函数
	 * @return
	 */
	void OnClientThread();
private:
	/**
	 * @brief	线程管理类
	 */
	NFThread m_thread;

	/**
	 * @brief	多线程消息队列
	 */
	NFQueueVector<NFThreadNetMsg*> m_threadNetMsgs;
};
