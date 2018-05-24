// -------------------------------------------------------------------------
//    @FileName         :    NFClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-24
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFCore/NFBuffer.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NetObject.h"

/////////////////////////////////////////////////
/**
 *@file   NFClient.h
 *@brief  单线程libevent网络客户端封装类.
 *
 */
 /////////////////////////////////////////////////

/**
*@brief  单线程libevent网络客户端封装类.
*/
class NFClient : public NetObject
{
public:
	/**
	 *@brief  构造函数.
	 */
	NFClient();

	/**
	 *@brief  构造函数.
	 */
	NFClient(uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFClient();

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
	 * @brief libevent读数据回调
	 *
	 * @param pEv   libevent读写数据类
	 * @param pArg  传入的参数
	 * @return
	 */
	static void conn_recvcb(struct bufferevent* pEv, void* pArg);

	/**
	 * @brief libevent连接事件回调
	 *
	 * @param pEv		libevent读写数据类
	 * @param events	事件
	 * @param pArg		传入的参数
	 * @return
	 */
	static void conn_eventcb(struct bufferevent* pEv, short events, void* pArg);

	/**
	 * @brief libevent写数据回调
	 *
	 * @param pEv   libevent读写数据类
	 * @param pArg  传入的参数
	 * @return
	 */
	static void conn_writecb(struct bufferevent* pEv, void* pArg);

	/**
	 * @brief log回调
	 *
	 * @param severity
	 * @param msg		要打印的消息
	 * @return
	 */
	static void log_cb(int severity, const char* msg);

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
	 * @brief	连接服务端
	 *
	 * @return  连接成功与否
	 */
	virtual bool Connect();

	/**
	 * @brief	关闭客户端连接, 与Conenct对应
	 * @return
	 */
	virtual void Close();

	/**
	 * @brief	获得libevent主数据结构
	 * @return
	 */
	event_base* GetMainBase() const;

	/**
	 * @brief	获得服务器名字
	 *
	 * @return
	 */
	const string& GetName() const;

	/**
	 * @brief	返回客户端连接配置
	 *
	 * @return
	 */
	const NFClientFlag& GetFlag() const;

	/**
	 * @brief	断线重连
	 * @return	是否成功
	 */
	virtual bool Reconnect();

	/**
	 * @brief	检查连接
	 * @return
	 */
	virtual void CheckConnect();
protected:
	/**
	 * @brief	libevent的react数据结构
	 */
	event_base* m_pMainBase;

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
