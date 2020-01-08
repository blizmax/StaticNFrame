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
#include "NFComm/NFCore/NFNonCopyable.h"
#include "NFClientUtils.h"
#include "NFConstVar.h"

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
	//rpc
	template<size_t TIMEOUT, typename T = void, typename... Args>
	typename std::enable_if<std::is_void<T>::value>::type Call(const std::string& rpc_name, Args&& ... args) {
		std::future<req_result> future = AsyncCall<FUTURE>(rpc_name, std::forward<Args>(args)...);
		auto status = future.wait_for(std::chrono::milliseconds(TIMEOUT));
		if (status == std::future_status::timeout || status == std::future_status::deferred) {
			throw std::out_of_range("timeout or deferred");
		}

		future.get().as();
	}

	template<typename T = void, typename... Args>
	typename std::enable_if<std::is_void<T>::value>::type Call(const std::string& rpc_name, Args&& ... args) {
		Call<DEFAULT_TIMEOUT, T>(rpc_name, std::forward<Args>(args)...);
	}

	template<size_t TIMEOUT, typename T, typename... Args>
	typename std::enable_if<!std::is_void<T>::value, T>::type Call(const std::string& rpc_name, Args&& ... args) {
		std::future<req_result> future = AsyncCall<FUTURE>(rpc_name, std::forward<Args>(args)...);
		auto status = future.wait_for(std::chrono::milliseconds(TIMEOUT));
		if (status == std::future_status::timeout || status == std::future_status::deferred) {
			throw std::out_of_range("timeout or deferred");
		}

		return future.get().as<T>();
	}

	template<typename T, typename... Args>
	typename std::enable_if<!std::is_void<T>::value, T>::type Call(const std::string& rpc_name, Args&& ... args) {
		return Call<DEFAULT_TIMEOUT, T>(rpc_name, std::forward<Args>(args)...);
	}

	template<CallModel model, typename... Args>
	std::future<req_result> AsyncCall(const std::string& rpc_name, Args&&... args) {
		auto p = std::make_shared<std::promise<req_result>>();
		std::future<req_result> future = p->get_future();

		uint64_t fu_id = 0;
		{
			std::unique_lock<std::mutex> lock(cb_mtx_);
			fu_id_++;
			fu_id = fu_id_;
			future_map_.emplace(fu_id, std::move(p));
		}

		msgpack_codec codec;
		auto ret = codec.pack_args(rpc_name, std::forward<Args>(args)...);
		Send((uint8_t)NFRpcRequestType::req_res, fu_id, ret.data(), ret.size());
		return future;
	}

	template<size_t TIMEOUT = DEFAULT_TIMEOUT, typename... Args>
	void AsyncCall(const NF_SERVER_TYPES eType, const std::string& rpc_name, std::function<void()> cb, Args&& ... args) {
		uint64_t cb_id = 0;
		{
			std::unique_lock<std::mutex> lock(cb_mtx_);
			callback_id_++;
			callback_id_ |= (uint64_t(1) << 63);
			cb_id = callback_id_;
			auto call = std::make_shared<call_t>(m_tcpClient->loop(), std::move(cb), TIMEOUT);
			call->start_timer();
			callback_map_.emplace(cb_id, call);
		}

		msgpack_codec codec;
		auto ret = codec.pack_args(rpc_name, std::forward<Args>(args)...);
		Send((uint8_t)request_type::req_res, cb_id, ret.data(), ret.size());
	}

	void RpcCackBack(uint64_t req_id, uint32_t ec, const std::string& data) {
		temp_req_id_ = req_id;
		auto cb_flag = req_id >> 63;
		if (cb_flag) {
			std::shared_ptr<call_t> cl = nullptr;
			{
				std::unique_lock<std::mutex> lock(cb_mtx_);
				cl = std::move(callback_map_[req_id]);
			}

			assert(cl);
			if (!cl->has_timeout()) {
				cl->cancel();
				cl->callback(ec, data);
			}
			else {
				cl->callback((uint32_t)NFRpcErrorCode::TIMEOUT, data);
			}

			std::unique_lock<std::mutex> lock(cb_mtx_);
			callback_map_.erase(req_id);
		}
		else {
			std::unique_lock<std::mutex> lock(cb_mtx_);
			auto& f = future_map_[req_id];
			if (ec) {
				//LOG<<ec.message();
				if (!f) {
					//std::cout << "invalid req_id" << std::endl;
					return;
				}
			}

			assert(f);
			f->set_value(req_result{ data });
			future_map_.erase(req_id);
		}
	}

	void ClearRpcCache() {
		{
			std::unique_lock<std::mutex> lock(cb_mtx_);
			callback_map_.clear();
			future_map_.clear();
		}
	}
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

	//rpc
	std::unordered_map<std::uint64_t, std::shared_ptr<std::promise<req_result>>> future_map_;
	std::unordered_map<std::uint64_t, std::shared_ptr<call_t>> callback_map_;
	std::mutex cb_mtx_;
	uint64_t callback_id_ = 0;
	std::mutex write_mtx_;
	uint64_t fu_id_ = 0;
	uint64_t temp_req_id_ = 0;
};


