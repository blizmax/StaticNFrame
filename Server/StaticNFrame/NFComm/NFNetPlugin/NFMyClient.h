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

class NFMyClient
{
public:
	/**
	 * @brief 构造函数
	 */
	NFMyClient(uint32_t linkId);

	/**
	 * @brief 析构函数
	 */
	virtual ~NFMyClient();

	/**
	 * @brief 连接
	 *
	 * @param  ip
	 * @param  port
	 * @return bool 
	 */
	virtual bool Connect(const std::string& ip, int port);
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
};
