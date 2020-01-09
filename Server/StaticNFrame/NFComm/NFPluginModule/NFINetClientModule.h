// -------------------------------------------------------------------------
//    @FileName         :    NFINetClientModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFINetModule.h"
#include "NFComm/NFPluginModule/NFMsgPackCode.h"
#include "NFComm/NFPluginModule/NFRpcConstVar.h"
#include "NFComm/NFPluginModule/NFRpcUtils.h"

class NFINetClientModule : public NFINetModule
{
public:
	/**
	 * @brief 初始化线程池
	 */
	virtual void InitThreadPool(uint32_t threadCount) = 0;
	/**
	 * @brief 连接服务器ip和port，返回代表这里连接的唯一ID，可以连接同种服务器类型多次
	 * 内部通过数组来实现快速访问
	 *
	 * @param  eServerType  服务器类型
	 * @param  strIp		连接的监听IP
	 * @param  nPort		连接的端口
	 * @return uint32_t		返回客户端的唯一ID，返回0表示错误
	 */
	virtual uint32_t AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, uint32_t nPort, uint32_t packetParsetype = 0, bool bWebSocket = false) = 0;

	virtual void CloseServer(const uint32_t unLinkId) = 0;

	virtual void DisconnectServer(const uint32_t unLinkId) = 0;

	virtual void CloseServerByServerType(NF_SERVER_TYPES eServerType) = 0;

	virtual void CloseAllServer() = 0;

	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	///////////////////////////////////////////////////
	virtual void SendByServerID(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToServerByPB(const uint32_t unLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	//rpc
	template<size_t TIMEOUT, typename T = void, typename... Args>
	typename std::enable_if<std::is_void<T>::value>::type Call(uint32_t unLinkId, const std::string& rpc_name, Args&& ... args)
	{
		std::future<NFRpcReqResult> future = AsyncCall<FUTURE>(rpc_name, std::forward<Args>(args)...);
		auto status = future.wait_for(std::chrono::milliseconds(TIMEOUT));
		if (status == std::future_status::timeout || status == std::future_status::deferred) {
			throw std::out_of_range("timeout or deferred");
		}

		future.get().as();
	}

	template<typename T = void, typename... Args>
	typename std::enable_if<std::is_void<T>::value>::type Call(uint32_t unLinkId, const std::string& rpc_name, Args&& ... args)
	{
		Call<DEFAULT_TIMEOUT, T>(unLinkId, rpc_name, std::forward<Args>(args)...);
	}

	template<size_t TIMEOUT, typename T, typename... Args>
	typename std::enable_if<!std::is_void<T>::value, T>::type Call(uint32_t unLinkId, const std::string& rpc_name, Args&& ... args)
	{
		std::future<NFRpcReqResult> future = AsyncCall<FUTURE>(unLinkId, rpc_name, std::forward<Args>(args)...);
		auto status = future.wait_for(std::chrono::milliseconds(TIMEOUT));
		if (status == std::future_status::timeout || status == std::future_status::deferred) {
			throw std::out_of_range("timeout or deferred");
		}

		return future.get().as<T>();
	}

	template<typename T, typename... Args>
	typename std::enable_if<!std::is_void<T>::value, T>::type Call(uint32_t unLinkId, const std::string& rpc_name, Args&& ... args)
	{
		return Call<DEFAULT_TIMEOUT, T>(unLinkId, rpc_name, std::forward<Args>(args)...);
	}

	template<CallModel model, typename... Args>
	std::future<NFRpcReqResult> AsyncCall(uint32_t unLinkId, const std::string& rpc_name, Args&&... args)
	{
		msgpack_codec codec;
		auto ret = codec.pack_args(rpc_name, std::forward<Args>(args)...);
		return AsyncCall(unLinkId, rpc_name, ret.data(), ret.size());
	}

	template<size_t TIMEOUT = DEFAULT_TIMEOUT, typename... Args>
	void AsyncCall(uint32_t unLinkId, const std::string& rpc_name, std::function<void(uint32_t error_code, const std::string& data)> cb, Args&& ... args)
	{
		msgpack_codec codec;
		auto ret = codec.pack_args(rpc_name, std::forward<Args>(args)...);
		AsyncCall(unLinkId, rpc_name, cb, TIMEOUT, ret.data(), ret.size());
	}

	virtual std::future<NFRpcReqResult> AsyncCall(uint32_t unLinkId, const std::string& rpc_name, const char* msg, const uint32_t nLen) = 0;
	virtual void AsyncCall(uint32_t unLinkId, const std::string& rpc_name, std::function<void(uint32_t error_code, const std::string& data)> cb, uint32_t timeout, const char* msg, const uint32_t nLen) = 0;
};