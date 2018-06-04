// -------------------------------------------------------------------------
//    @FileName         :    NFServer.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"

struct evconnlistener;
struct event_base;
class NetObject;

class NFServer : public NFIModule
{
public:
	/**
	 * @brief 构造函数
	 */
	NFServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag);

	/**
	* @brief 析构函数
	*/
	virtual ~NFServer();


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
	 * @brief 添加网络对象
	 *
	 * @param  fd
	 * @param  sa
	 * @return bool 
	 */
	bool AddNetObject(SOCKET fd, sockaddr* sa);

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize);

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

	/**
	 * @brief 获得一个可用的ID
	 *
	 * @return uint32_t
	 */
	virtual uint32_t GetFreeUnLinkId();

	/**
	* @brief 获得当前网络对象个数
	*
	* @return uint32_t
	*/
	virtual uint32_t GetNetObjectCount() const;

	/**
	* @brief 获得当前最大链接个数
	*
	* @return uint32_t
	*/
	virtual uint32_t GetMaxConnectNum() const;

	/**
	 * @brief
	 *
	 * @return event_base*
	 */
	virtual event_base* GetEventBase() const;
protected:
	/**
	 * @brief
	 *
	 * @return void 
	 */
	virtual void ExecuteClose();

	/**
	 * @brief 网络接受数据包回调
	 *
	 * @param  unLinkId
	 * @param  playerId
	 * @param  nMsgId
	 * @param  msg
	 * @param  nLen
	 * @return void 
	 */
	virtual void OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	/**
	 * @brief 网络事件，连接和断开连接处理
	 *
	 * @param  nEvent
	 * @param  unLinkId
	 * @return void 
	 */
	virtual void OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId);
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
	* @brief libevent
	*/
	struct event_base* mBase;

	/**
	* @brief libevent监听器
	*/
	struct evconnlistener* mListener;

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
	* @brief 链接对象数组
	*/
	std::vector<NetObject*> mNetObjectArray;

	/**
	* @brief 链接对象真实数目
	*/
	uint32_t mNetObjectCount;

	/**
	* @brief 需要删除的连接对象
	*/
	std::vector<uint32_t> mvRemoveObject;
};

