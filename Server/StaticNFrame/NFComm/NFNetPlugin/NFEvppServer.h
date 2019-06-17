// -------------------------------------------------------------------------
//    @FileName         :    NFEvppServer.h
//    @Author           :    GaoYi
//    @Date             :    2019/06/01
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
#include "NetEvppObject.h"
#include "NFIServer.h"

#include <evpp/tcp_server.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>
#include "NFComm/NFCore/NFQueue.hpp"

struct MsgFromNetInfo
{
	MsgFromNetInfo(const evpp::TCPConnPtr TCPConPtr) : mTCPConPtr(TCPConPtr)
	{
		nType = eMsgType_Num;
	}

	eMsgType nType;
	evpp::TCPConnPtr mTCPConPtr;
	std::string strMsg;
};

class NFCNetServerModule;

class NFEvppServer : public NFIServer
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
	 * @brief 添加网络对象
	 *
	 * @param  fd
	 * @param  sa
	 * @return bool
	 */
	NetEvppObject* AddNetObject(const evpp::TCPConnPtr& conn);

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
	 * @brief 获得连接IP
	 *
	 * @param  usLinkId
	 * @return std::string
	 */
	virtual std::string GetLinkIp(uint32_t usLinkId) override;

	/**
	* @brief 关闭连接
	*
	* @param  usLinkId
	* @return
	*/
	virtual void CloseLinkId(uint32_t usLinkId) override;

	/**
	 * @brief 获得一个可用的ID
	 *
	 * @return uint32_t
	 */
	virtual uint32_t GetFreeUnLinkId();

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool SendAll(const void* pData, uint32_t unSize) override;

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(uint32_t usLinkId, const void* pData, uint32_t unSize) override;

	NetEvppObject* GetNetObject(uint32_t uslinkId);
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

	/**
	 * @brief 主线程处理消息队列
	 */
	virtual void ProcessMsgLogicThread();
private:
	evpp::EventLoopThread* m_eventLoop;
	evpp::TCPServer* m_tcpServer;
private:
	/**
	* @brief 链接对象数组
	*/
	std::unordered_map<uint32_t, NetEvppObject*> mNetObjectArray;

	/**
	* @brief 需要删除的连接对象
	*/
	std::vector<uint32_t> mvRemoveObject;

	/**
	* @brief 需要消息队列
	*/
	NFQueueVector<MsgFromNetInfo*> mMsgQueue;

	/**
	* @brief 退出的时候用来确认
	*/
	atomic_bool mExit;
};
