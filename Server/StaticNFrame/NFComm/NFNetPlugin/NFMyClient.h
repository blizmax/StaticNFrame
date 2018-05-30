// -------------------------------------------------------------------------
//    @FileName         :    NFMyClient.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/28
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFSocket.h"
#include "NFPoll.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFCore/NFBuffer.h>
#include "NFClient.h"

struct NFClientFlag;

class NFMyClient : public NFIModule
{
public:
	/**
	 * @brief 构造函数
	 */
	NFMyClient(uint32_t nId, const NFClientFlag& flag);

	/**
	 * @brief 析构函数
	 */
	virtual ~NFMyClient();

		/**
	 *@brief  设置接收回调.
	 */
	template <typename BaseType>
	void SetRecvCB(BaseType* pBaseType, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	}

	/**
	 *@brief  设置连接事件回调.
	 */
	template <typename BaseType>
	void SetEventCB(BaseType* pBaseType, void (BaseType::*handleEvent)(const eMsgType nEvent, const uint32_t unLinkId))
	{
		mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2);
	}

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
	 * @brief 连接
	 *
	 * @param  ip
	 * @param  port
	 * @return bool 
	 */
	virtual bool Connect(const std::string& ip, int port);

	/**
	 * @brief 发送数据
	 *
	 * @param  buf
	 * @param  len
	 * @return bool 
	 */
	virtual bool Send(const char* buf, uint32_t len);

	/**
	 * @brief 处理接受发送
	 *
	 * @param  timeout
	 * @return bool 
	 */
	virtual bool Dispatch(uint32_t timeout);

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
	virtual void OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue);

	/**
	 * @brief	处理连接成功
	 *
	 * @param nSocket	系统分配的socket
	 * @return
	 */
	virtual void OnHandleConnect();

	/**
	 * @brief	处理断开连接
	 *
	 * @return
	 */
	virtual void OnHandleDisConnect();

	/**
	 * @brief	接收数据
	 *
	 * @return	是否成功
	 */
	virtual bool OnRecvData();

	/**
	 * @brief	取出数据，并进行处理
	 *
	 * @return	< 0, 表示网络数据有问题, = 0, 表示还有数据继续处理，> 0表示数据处理完毕
	 */
	virtual int Dismantle();

	/**
	 * @brief	获得唯一ID
	 *
	 * @return
	 */
	virtual uint32_t GetLinkId() const;
protected:
	/**
	 * @brief	代表这个链接的socket
	 */
	SOCKET m_nSocketId;

	/**
	 * @brief	代表客户端连接的唯一ID
	 */
	uint32_t m_usLinkId;

	/**
	 * @brief 事件结构，主要存取网络事件标志，以及读，写数据事件的处理
	 */
	EventData* m_pEventData;

	/**
	 * @brief	处理接受数据的回调
	 */
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	 * @brief	网络事件回调
	 */
	NET_EVENT_FUNCTOR mEventCB;

	/**
	 * @brief	用来保存网络发过来的临时数据
	 */
	NFBuffer m_buffer;

	/**
	 * @brief	连接配置数据
	 */
	NFClientFlag m_flag;
};
