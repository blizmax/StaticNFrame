// -------------------------------------------------------------------------
//    @FileName         :    NFEvppClient.h
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
#include "NFNetDefine.h"
#include "NetEvppObject.h"

#include "evpp/event_loop_thread.h"
#include "evpp/tcp_client.h"
#include "NFEvppServer.h"
#include "NFIClient.h"


 /**
 *@brief   多线程evpp/libevent网络客户端封装类.
 */
class NFEvppClient : public NFIClient
{
	class call_t : NFNonCopyable, public std::enable_shared_from_this<call_t> {
	public:
		call_t(evpp::EventLoop* eventLoop, std::function<void(uint32_t error_code, const std::string& data)> cb, size_t timeout) : eventLoop_(eventLoop),
			cb_(std::move(cb)), timeout_(timeout) {
		}

		void start_timer() {
			if (timeout_ == 0) {
				return;
			}

			auto self = this->shared_from_this();
			timer_ = eventLoop_->RunAfter(timeout_, [this, self]() {
				has_timeout_ = true;
			});

		}

		void callback(uint32_t error_code, const std::string& data) {
			cb_(error_code, data);
		}

		bool has_timeout() const {
			return has_timeout_;
		}

		void cancel() {
			if (timeout_ == 0) {
				return;
			}

			timer_->Cancel();
		}

	private:
		evpp::EventLoop* eventLoop_;
		evpp::InvokeTimerPtr timer_;
		std::function<void(uint32_t error_code, const std::string& data)> cb_;
		size_t timeout_;
		bool has_timeout_ = false;
	};
public:
	/**
	 *@brief  构造函数.
	 */
	NFEvppClient(evpp::EventLoop* eventLoop, uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFEvppClient();

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
	 * @brief	关闭连接
	 *
	 * @return	是否成功
	 */
	virtual void CloseServer();

	/**
	 * @brief	连接服务端
	 *
	 * @return  连接成功与否
	 */
	virtual bool Connect();

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);

	/**
	 * @brief	发送数据 不包含数据头
	 *
	 * @param pData		发送的数据,
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId = 0) override;

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const uint8_t rpcType, const uint64_t rpcReqId, const char* msg, const uint32_t nLen);

	void ProcessMsgLogicThread();

	void SafeExit();
public:
	virtual std::future<NFRpcReqResult> AsyncCall(const std::string& rpc_name, const char* msg, const uint32_t nLen) override;
	virtual void AsyncCall(const std::string& rpc_name, std::function<void(uint32_t error_code, const std::string& data)> cb, uint32_t timeout, const char* msg, const uint32_t nLen) override;
	virtual void RpcCallBack(uint64_t req_id, uint32_t ec, const std::string& data);
	virtual void ClearRpcCache();
private:
	evpp::TCPClient* m_tcpClient;
protected:
	/**
	 * @brief	连接对象
	 */
	NetEvppObject* m_pObject;

	/**
	* @brief 需要消息队列
	*/
	NFQueueVector<MsgFromNetInfo*> mMsgQueue;

	/**
	 * @brief	多线程安全退出
	 */
	std::atomic_bool m_safeExit;

	/**
	 * @brief	rpc
	 */
	std::unordered_map<std::uint64_t, std::shared_ptr<std::promise<NFRpcReqResult>>> future_map_;
	std::unordered_map<std::uint64_t, std::shared_ptr<call_t>> callback_map_;
	std::mutex cb_mtx_;
	uint64_t callback_id_ = 0;
	std::mutex write_mtx_;
	uint64_t fu_id_ = 0;
	uint64_t temp_req_id_ = 0;
};


