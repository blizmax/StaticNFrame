// -------------------------------------------------------------------------
//    @FileName         :    NetObject.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <algorithm>
#include <stdint.h>

#include "NFLibEvent.h"

#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFNetDefine.h"
#include "NFComm/NFCore/NFBuffer.h"

#include <vector>
#include <atomic>

struct stMsg;
struct bufferevent;
class NFThreadClient;
class NFClient;
class NFServer;

/**
* @brief 网络对象，代表一个连接
*/
class NetObject
{
public:
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
public:
	/**
	 * @brief	构造函数
	 */
	NetObject();

	/**
	 * @brief	析构函数
	 */
	virtual ~NetObject();

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
	 *@brief  设置接收回调.
	 */
	void SetRecvCB(const NET_RECEIVE_FUNCTOR& recvcb)
	{
		mRecvCB = recvcb;
	}

	/**
	 *@brief  设置连接事件回调.
	 */
	void SetEventCB(const NET_EVENT_FUNCTOR& eventcb)
	{
		mEventCB = eventcb;
	}

	/**
	* @brief 如果是websocket的话，判断是否handleshark
	*
	* @return bool
	*/
	bool IsHandleShark() const;

	/**
	* @brief 如果是websocket的话，需要设计handleshark
	*
	* @return
	*/
	void SetHandleShark(bool b);

	/**
	* @brief 如果是websocket的话
	*
	* @return bool
	*/
	bool IsWebSocket() const;

	/**
	* @brief 如果是websocket的话
	*
	* @return 
	*/
	void SetWebSocket(bool b);

	/**
	 * @brief
	 *
	 * @return std::string
	 */
	std::string GetStrIp() const;

	/**
	 * @brief
	 *
	 * @param  val
	 * @return void
	 */
	void SetStrIp(std::string val);

	/**
	 * @brief
	 *
	 * @return uint32_t
	 */
	uint32_t GetPort() const;

	/**
	 * @brief
	 *
	 * @param  val
	 * @return void
	 */
	void SetPort(uint32_t val);

	/**
	 * @brief	接收数据
	 *
	 * @return	是否成功
	 */
	virtual bool OnRecvData(bufferevent* pBufEv);

	/**
	 * @brief	取出数据，并进行处理
	 *
	 * @return	< 0, 表示网络数据有问题, = 0, 表示还有数据继续处理，> 0表示数据处理完毕
	 */
	virtual int Dismantle();

	/**
	 * @brief	保存socket，并对socket做属性处理, 非阻塞
	 *
	 * @return
	 */
	virtual void SetSocketId(SOCKET nSocket);

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	/**
	 * @brief	获得读写数据结构体
	 *
	 * @return
	 */
	struct bufferevent* GetBev() const;

	/**
	 * @brief	获得读写数据结构体
	 *
	 * @return
	 */
	void SetBev(struct bufferevent* bev);

	/**
	 * @brief	获得唯一ID
	 *
	 * @return
	 */
	uint32_t GetLinkId() const;

	/**
	* @brief	获得唯一ID
	*
	* @return
	*/
	void SetLinkId(uint32_t linkId);

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
	* @brief	对解析出来的数据进行处理
	*
	* @param type    数据类型，主要是为了和多线程统一处理, 主要有接受数据处理，连接成功处理，断开连接处理
	* @param usLink  本客户端的唯一id
	* @param strMsg  websocket 字符串数据 这里的字符串不带0结尾，不要使用c_str函数
	* @return
	*/
	virtual void OnHandleMsgPeer(eMsgType type, uint32_t usLink, const std::string& strMsg);

	/**
	 * @brief
	 *
	 * @return bool
	 */
	bool GetNeedRemove() const;

	/**
	 * @brief
	 *
	 * @param  val
	 * @return void
	 */
	void SetNeedRemove(bool val);

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
	 * @brief 关闭对象禁止对象的读写功能
	 *
	 * @return void
	 */
	virtual void CloseObject();

	/**
	* @brief 处理shark info
	*
	* @return void
	*/
	virtual int HandleSharkInfo();

	/**
	* @brief 处理shark info
	*
	* @return void
	*/
	virtual std::string HandleSharkReturn();
protected:
	/**
	 * @brief	处理接受数据的回调
	 */
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	 * @brief	网络事件回调
	 */
	NET_EVENT_FUNCTOR mEventCB;

	/**
	 * @brief	libevent代表连接读写事件对象
	 */
	struct bufferevent* m_pBev;

	/**
	 * @brief	代表这个链接的socket
	 */
	SOCKET m_nSocketId;

	/**
	 * @brief	代表客户端连接的唯一ID
	 */
	uint32_t m_usLinkId;

	/**
	 * @brief	用来保存网络发过来的临时数据
	 */
	NFBuffer m_buffer;

	/**
	 * @brief	连接代表的对方的IP
	 */
	std::string m_strIp;

	/**
	 * @brief	连接代表的对方的端口
	 */
	uint32_t m_port;

	/**
	 * @brief 是否需要删除, 这个链接不在起作用，将在下一次循环中被删除
	 */
	bool mNeedRemove;

	/**
	* @brief 如果是websocket，需要先握手
	*/
	bool mHandleShark;

	/**
	* @brief websocket
	*/
	bool mWebSocket;

	/**
	* @brief websocket header map
	*/
	std::map<std::string, std::string> mHeaderMap;

	std::string mCacheFrame;

	std::string mParseString;

	uint32_t mWSFrameType;
};

