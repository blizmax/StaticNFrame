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
class NFClient : public NFIModule
{
public:
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
	 * @brief	删除关闭的连接
	 *
	 * @return	是否成功
	 */
	virtual void ExecuteClose();

	/**
	 * @brief	连接服务端
	 *
	 * @return  连接成功与否
	 */
	virtual bool Connect();

	/**
	 * @brief	关闭客户端连接, 与Conenct对应
	 * 程序运行时，只能在OnExectue里调用，
	 * 退出时，在析构函数调用
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
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	/**
	 * @brief
	 */
	eConnectStatus GetStatus() const;

	/**
	 * @brief
	 */
	void SetStatus(eConnectStatus val);

	/**
	 * @brief
	 */
	bool IsNeedRemve() const;

	/**
	 * @brief
	 *
	 * @return uint64_t
	 */
	uint64_t GetLastActionTime() const;

	/**
	 * @brief
	 *
	 * @param  time
	 * @return void
	 */
	void SetLastActionTime(uint64_t time);

	virtual bool IsWebSocket() const { return m_flag.bWebSocket; }
protected:
	/**
	 * @brief	libevent的react数据结构
	 */
	event_base* m_pMainBase;

	/**
	 * @brief	连接配置数据
	 */
	NFClientFlag m_flag;

	/**
	 * @brief	服务器名字
	 */
	std::string m_strName;

	/**
	 * @brief	连接对象
	 */
	NetObject* m_pObject;

	/**
	 * @brief	处理接受数据的回调
	 */
	NET_RECEIVE_FUNCTOR mRecvCB;

	/**
	 * @brief	网络事件回调
	 */
	NET_EVENT_FUNCTOR mEventCB;

	/**
	 * @brief	代表客户端连接的唯一ID
	 */
	uint32_t m_usLinkId;

	/**
	 * @brief 连接状态
	 */
	eConnectStatus mStatus;

	/**
	 * @brief 上一次活动时间
	 */
	uint64_t mLastActionTime;
};

