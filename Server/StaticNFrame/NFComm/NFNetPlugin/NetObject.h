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

#include "event2/bufferevent.h"
#include "event2/event.h"
#include "event2/util.h"
#include "event2/buffer.h"
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>
#include <event2/listener.h>
#include <event2/event_compat.h>

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFNetDefine.h"
#include "NFComm/NFCore/NFBuffer.h"

#include <vector>

struct stMsg;
struct bufferevent;
class NFThreadClient;
class NFClient;

/**
* @brief 网络对象，代表一个连接
*/
class NetObject : public NFIModule
{
	friend NFThreadClient;
	friend NFClient;
public:
	/**
	 * @brief	构造函数
	 */
	NetObject();

	/**
	 * @brief	析构函数
	 */
	virtual ~NetObject();
public:
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
	virtual int	Dismantle();

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
	 * @brief	获得唯一ID
	 *
	 * @return
	 */
	uint32_t GetLinkId() const;

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
	virtual void OnHandleMsgPeer(eMsgType type, uint32_t usLink, char* pBuf, uint32_t sz, uint32_t nMsgId, uint64_t nValue) = 0;

	/**
	 * @brief	获得上一次接收数据的时间
	 * @return
	 */
	uint64_t GetLastRecvTime() const;

	/**
	 * @brief	设置上一次接收数据的时间
	 * @return
	 */
	void SetLastRecvTime(uint64_t val);

	/**
	 * @brief	获得上一次ping的时间
	 * @return
	 */
	uint64_t GetLastPingTime() const;

	/**
	 * @brief	设置上一次ping的时间
	 * @return
	 */
	void SetLastPingTime(uint64_t val);

	/**
	 * @brief	获得连接状态
	 * @return
	 */
	eConnectStatus GetStatus() const;

	/**
	 * @brief	设置连状态
	 * @return
	 */
	void SetStatus(eConnectStatus val);

	/**
	 * @brief	是否处于连接状态
	 * @return
	 */
	bool IsConnectOK() const;
protected:
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
	 * @brief	连接状态
	 */
	eConnectStatus m_eStatus;

	/**
	 * @brief	上次接受数据的时间
	 */
	uint64_t m_lastRecvTime;

	/**
	 * @brief	上次断开连接的时间
	 */
	uint64_t m_lastDisconnetTime;

	/**
	 * @brief	上次ping的时间
	 */
	uint64_t m_lastPingTime;
};
