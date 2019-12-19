// -------------------------------------------------------------------------
//    @FileName         :    NFIServer.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/03
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//
//                    .::::.
//                  .::::::::.
//                 :::::::::::  FUCK YOU
//             ..:::::::::::'
//           '::::::::::::'
//             .::::::::::
//        '::::::::::::::..
//             ..::::::::::::.
//           ``::::::::::::::::
//            ::::``:::::::::'        .:::.
//           ::::'   ':::::'       .::::::::.
//         .::::'      ::::     .:::::::'::::.
//        .:::'       :::::  .:::::::::' ':::::.
//       .::'        :::::.:::::::::'      ':::::.
//      .::'         ::::::::::::::'         ``::::.
//  ...:::           ::::::::::::'              ``::.
// ```` ':.          ':::::::::'                  ::::..
//                    '.:::::'                    ':'````..
//
// -------------------------------------------------------------------------
#pragma once


#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"
#include "NFComm/NFCore/NFMutex.h"
#include "NFComm/NFCore/NFLock.h"


class NFIServer : public NFIModule
{
	friend class NFCNetServerModule;
public:
	/**
 * @brief 构造函数
 */
	NFIServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag) : mFlag(flag), mServerType(serverType), mServerId(serverId), mNetObjectCount(0)
	{
		mWebSocket = flag.bWebSocket;
		mPacketParseType = flag.mPacketParseType;
		mNetObjectMaxIndex = 0;
		assert(serverType > NF_ST_NONE && serverType < NF_ST_MAX);
	}

	/**
	* @brief 析构函数
	*/
	virtual ~NFIServer()
	{
		
	}
	/**
	 *@brief  设置接收回调.
	 */
	template <typename BaseType>
	void SetRecvCB(BaseType* pBaseType, void (BaseType::*handleRecieve)(const uint32_t unLinkId, const uint64_t valueId, const uint32_t opreateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen))
	{
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
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
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize) = 0;

	/**
 * @brief	发送数据 不包含数据头
 *
 * @param pData		发送的数据,
 * @param unSize	数据的大小
 * @return
 */
	virtual bool Send(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	/**
	 * @brief 获得连接IP
	 *
	 * @param  usLinkId
	 * @return std::string
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) = 0;

	/**
	* @brief 关闭连接
	*
	* @param  usLinkId
	* @return
	*/
	virtual void CloseLinkId(uint32_t usLinkId) = 0;

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool SendAll(const void* pData, uint32_t unSize) = 0;

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool SendAll(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	/**
	* @brief	获得唯一ID
	*
	* @return
	*/
	virtual uint32_t GetServerId() const { return mServerId; }

	/**
	* @brief	获得服务器类型
	*
	* @return
	*/
	virtual uint32_t GetServerType() const { return mServerType; }

	/**
	* @brief	是否是web服务器
	*
	* @return
	*/
	virtual bool IsWebSocket() const { return mWebSocket; }

	/**
	* @brief 获得当前网络对象个数
	*
	* @return uint32_t
	*/
	virtual uint32_t GetNetObjectCount() const { return mNetObjectCount; }

	/**
	* @brief 获得当前最大链接个数
	*
	* @return uint32_t
	*/
	virtual uint32_t GetMaxConnectNum() const { return mFlag.nMaxConnectNum; }
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
	* @brief 链接对象真实数目
	*/
	uint32_t mNetObjectCount;

	/**
	* @brief 当前链接对象最大索引
	*/
	uint32_t mNetObjectMaxIndex;

	/**
	* @brief 是否是websocket
	*/
	bool mWebSocket;

	/**
	* @brief 解码消息类型
	*/
	uint32_t mPacketParseType;

	/**
	* @brief 解码消息类型, 攻击服务器IP收集
	*/
	std::map<std::string, uint32_t> mAttackIp;

	/**
	* @brief 解码消息类型, 攻击服务器IP收集
	*/
	NFMutex mAttackIpLock;
};


