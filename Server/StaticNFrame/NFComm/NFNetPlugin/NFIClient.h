// -------------------------------------------------------------------------
//    @FileName         :    NFIClient.h
//    @Author           :    GaoYi
//    @Date             :    2019-06-04
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                               _(\_/) 
//                             ,((((^`\
//                            ((((  (6 \ 
//                          ,((((( ,    \
//      ,,,_              ,(((((  /"._  ,`,
//     ((((\\ ,...       ,((((   /    `-.-'
//     )))  ;'    `"'"'""((((   (      
//    (((  /            (((      \
//     )) |                      |
//    ((  |        .       '     |
//    ))  \     _ '      `t   ,.')
//    (   |   y;- -,-""'"-.\   \/  
//    )   / ./  ) /         `\  \
//       |./   ( (           / /'
//       ||     \\          //'|
//       ||      \\       _//'||
//       ||       ))     |_/  ||
//       \_\     |_/          ||
//       `'"                  \_\
//                            `'" 
// -------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NetEvppObject.h"

#include "NFEvppServer.h"

/////////////////////////////////////////////////
/**
 *@file   NFIClient.h
 *@brief  网络客户端封装类.
 *
 */
 /////////////////////////////////////////////////

 /**
 *@brief   网络客户端封装类.
 */
class NFIClient : public NFIModule
{
public:
	/**
	 *@brief  构造函数.
	 */
	NFIClient(uint32_t nId, const NFClientFlag& flag):m_usLinkId(nId), m_flag(flag)
	{
		mStatus = eConnectStatus_Disconnect;
		mLastActionTime = 0;
		mPacketParseType = 0;
	}

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFIClient()
	{
		
	}

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
	 * @brief	删除关闭的连接
	 *
	 * @return	是否成功
	 */
	virtual void ExecuteClose() = 0;

	/**
	 * @brief	关闭连接
	 *
	 * @return	是否成功
	 */
	virtual void CloseServer() = 0;

	/**
	 * @brief	获得服务器名字
	 *
	 * @return
	 */
	const string& GetName() const { return m_strName; }

	/**
	 * @brief	返回客户端连接配置
	 *
	 * @return
	 */
	const NFClientFlag& GetFlag() const { return m_flag; }

	/**
	 * @brief	获得唯一ID
	 *
	 * @return
	 */
	uint32_t GetLinkId() const { return m_usLinkId; }

	/**
	* @brief	获得唯一ID
	*
	* @return
	*/
	void SetLinkId(uint32_t linkId) { m_usLinkId = linkId; }

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize) = 0;

	/**
	 * @brief	发送数据 不包含数据头
	 *
	 * @param pData		发送的数据,
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) = 0;

	/**
	 * @brief
	 */
	eConnectStatus GetStatus() const { return mStatus; }

	/**
	 * @brief
	 */
	void SetStatus(eConnectStatus val) { mStatus = val; }

	/**
	 * @brief
	 */
	bool IsNeedRemve() const { return mStatus == eConnectStatus_REMOVE; }

	/**
	 * @brief
	 *
	 * @return uint64_t
	 */
	uint64_t GetLastActionTime() const { return mLastActionTime; }

	/**
	 * @brief
	 *
	 * @param  time
	 * @return void
	 */
	void SetLastActionTime(uint64_t time) { mLastActionTime = time; }

	virtual bool IsWebSocket() const { return m_flag.bWebSocket; }

	void SetPacketParseType(uint32_t type) { mPacketParseType = type; }
	uint32_t GetPacketParseType() const { return mPacketParseType; }
protected:
	/**
	 * @brief	连接配置数据
	 */
	NFClientFlag m_flag;

	/**
	 * @brief	服务器名字
	 */
	std::string m_strName;

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

	/**
	* @brief 解码消息类型
	*/
	uint32_t mPacketParseType;
};
