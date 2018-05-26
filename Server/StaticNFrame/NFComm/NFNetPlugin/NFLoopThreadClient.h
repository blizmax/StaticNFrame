// -------------------------------------------------------------------------
//    @FileName         :    NFThreadClient.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NetObject.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "NetThreadObject.h"
#include "NFEventLoopThread.h"

/**
*@brief  多线程libevent网络客户端封装类.
*		 m_thread主要使用在主线程钟
*		 m_threadNetMsgs用于在两个线程中传输数据 
*		 event_base* m_pMainBase创建于主线程，使用在client线程，销毁在主线程
*        struct bufferevent* m_pBev创建于client线程, 读在client线程，写在主线程，没有看到同步？
*        mRecvCB, mEventCB均使用与主线程
*		 SOCKET m_nSocketId用在client线程
*        NFBuffer m_buffer用在client线程
*        eConnectStatus m_eStatus写主要在client线程，连接时写了一次，这个地方没有问题，断开连接时写了一次，可能会导致问题，
*        Send时会有读判断， 但没有同步措施，有可能在client线程已经断开连接了，但是这里还在写? 不知道libevent里会不会有判断
*        Reconnect没有同步eStatus，也可能有问题
* 问题1：m_eStatus在两个线程里使用，大部分时间没有问题， 但连接断开时，可能会有问题
* 问题2：Close函数在主线程中运行有问题，在不止client线程在做什么的情况下，贸然调用	SetStatus(eConnectStatus_Disconnect);
	m_buffer.Clear(); 可能导致未定义问题，或崩溃. 应该先break循环，关闭线程，然后在调用
*/
class NFLoopThreadClient : public NetThreadObject
{
public:
	/**
	 *@brief  构造函数.
	 */
	NFLoopThreadClient(uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFLoopThreadClient();

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
	virtual void CheckConnect();

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
	 * @brief	关闭连接, 如果在主线调用这个函数，应该先break网络线程，
	 * 然后m_thread.StopThread()关闭网络线程
	 * 这样就不会有线程冲突, 然后在调用
	 *
	 * @return
	 */
	virtual int Close();

	/**
	 * @brief	开始连接
	 * @return
	 */
	virtual bool Connect();

	/**
	 * @brief	断线重连
	 *
	 * @return
	 */
	virtual bool Reconnect();

	/**
	 * @brief	处理连接成功
	 *
	 * @param nSocket	系统分配的socket
	 * @return
	 */
	virtual void OnHandleConnect(SOCKET nSocket);

	/**
	 * @brief	处理断开连接
	 *
	 * @return
	 */
	virtual void OnHandleDisConnect();

	/**
	 * @brief	客户端线程运行函数
	 * @return
	 */
	int OnClientThread();
private:
	/**
	 * @brief	线程管理类
	 */
	std::unique_ptr<NFEventLoopThread> m_loopThread;

	/**
	 * @brief	多线程消息队列
	 */
	NFQueueVector<NFThreadNetMsg*> m_threadNetMsgs;

	/**
	* @brief	处理接受数据的回调
	*/
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	* @brief	网络事件回调
	*/
	NET_EVENT_FUNCTOR mEventCB;

	/**
	* @brief	连接配置数据
	*/
	NFClientFlag m_flag;

	/**
	* @brief	服务器名字
	*/
	std::string m_strName;
};

