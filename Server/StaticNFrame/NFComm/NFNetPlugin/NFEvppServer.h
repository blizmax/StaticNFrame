// -------------------------------------------------------------------------
//    @FileName         :    NFEvppServer.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once


#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"

#include <evpp/tcp_server.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>

class NFCNetServerModule;

class NFEvppServer : public NFIModule
{
	friend NFCNetServerModule;
public:
	/**
	 * @brief 构造函数
	 */
	NFEvppServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag);

	/**
	* @brief 析构函数
	*/
	virtual ~NFEvppServer();

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
	* @brief	获得唯一ID
	*
	* @return
	*/
	uint32_t GetServerId() const;

	/**
	* @brief	获得服务器类型
	*
	* @return
	*/
	uint32_t GetServerType() const;

	/**
	* @brief	是否是web服务器
	*
	* @return
	*/
	bool IsWebSocket() const;

	/**
	* @brief	初始化
	*
	* @return 是否成功
	*/
	virtual bool Init() override;

	/**
	* @brief	关闭客户端
	*
	* @return  是否成功
	*/
	virtual bool Shut() override;

	/**
	 * @brief 释放数据
	 *
	 * @return bool
	 */
	virtual bool Finalize() override;

	/**
	* @brief	服务器每帧执行
	*
	* @return	是否成功
	*/
	virtual bool Execute() override;
private:
	evpp::EventLoop* m_eventLoop;
	evpp::TCPServer* m_tcpServer;
private:
	/**
	 * @brief	处理接受数据的回调
	 */
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	 * @brief	网络事件回调
	 */
	NET_EVENT_FUNCTOR mEventCB;

	/**
	* @brief 服务器配置数据
	*/
	NFServerFlag mFlag;

	/**
	* @brief 服务器类型
	*/
	NF_SERVER_TYPES mServerType;

	/**
	* @brief 服务器Id，一般一个应用程序一个服务器类型，一个服务器ID
	*/
	uint32_t mServerId;

	/**
	* @brief 是否是websocket
	*/
	bool mWebSocket;

	/**
	* @brief 解码消息类型
	*/
	uint32_t mPacketParseType;
};
