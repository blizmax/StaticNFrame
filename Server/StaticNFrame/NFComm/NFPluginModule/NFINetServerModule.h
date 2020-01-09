// -------------------------------------------------------------------------
//    @FileName         :    NFINetServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2017-10-16
//    @Module           :    NFINetServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"
#include "NFComm/NFCore/NFMetaUtils.h"
#include "NFComm/NFPluginModule/NFMsgPackCode.h"
#include "NFComm/NFPluginModule/NFRpcConstVar.h"
#include "NFComm/NFPluginModule/NFRpcUtils.h"

class NFINetServerModule : public NFINetModule
{
public:
	/**
	 * @brief 添加服务器
	 *
	 * @param  eType		服务器类型
	 * @param  nServerID	服务器ID
	 * @param  nMaxClient	服务器最大连接客户端数
	 * @param  nPort		服务器监听端口
	 * @return int			返回0错误
	 */
	virtual uint32_t AddServer(const NF_SERVER_TYPES eType, uint32_t nServerID, uint32_t nMaxClient, uint32_t nPort, bool bWebSocket = false, uint32_t nPacketParseType = 0, bool bForeignNetwork = false, bool bRefuseAttackIp = false) = 0;

	/**
	 * @brief 获得连接的IP
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

	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToServerByPB(uint32_t usLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendByServerID(uint32_t usLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToServerByPB(uint32_t usLinkId, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const std::string& strData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint16_t nMainMsgID, const uint16_t nSubMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID, const uint32_t operateId) = 0;

	/**
	* @brief rpc注册
	*
	* @return
	*/
	template<typename Function>
	void RegisterRpcHandler(NF_SERVER_TYPES eServerType, std::string const& name, const Function& f) 
	{
		register_handler<ExecMode::sync>(eServerType, name, f);
	}

	/**
	* @brief rpc注册
	*
	* @return
	*/
	template<typename Function, typename Self>
	void RegisterRpcHandler(NF_SERVER_TYPES eServerType, std::string const& name, const Function& f, Self* self) 
	{
		register_handler<ExecMode::sync>(eServerType, name, f, self);
	}
	
protected:
	template<ExecMode model, typename Function>
	void register_handler(NF_SERVER_TYPES eServerType, std::string const& name, Function f) {
		return register_nonmember_func<model>(eServerType, name, std::move(f));
	}

	template<ExecMode model, typename Function, typename Self>
	void register_handler(NF_SERVER_TYPES eServerType, std::string const& name, const Function& f, Self* self) {
		return register_member_func<model>(eServerType, name, f, self);
	}

	template<typename F, size_t... I, typename Arg, typename... Args>
	static typename std::result_of<F(uint32_t, Args...)>::type call_helper(
		const F & f, const std::index_sequence<I...>&, std::tuple<Arg, Args...> tup, uint32_t unlinkId) {
		return f(unlinkId, std::move(std::get<I + 1>(tup))...);
	}

	template<typename F, typename Arg, typename... Args>
	static
		typename std::enable_if<std::is_void<typename std::result_of<F(uint32_t, Args...)>::type>::value>::type
		call(const F & f, uint32_t unlinkId, std::string & result, std::tuple<Arg, Args...> tp) {
		call_helper(f, std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), unlinkId);
		result = msgpack_codec::pack_args_str(result_code::OK);
	}

	template<typename F, typename Arg, typename... Args>
	static
		typename std::enable_if<!std::is_void<typename std::result_of<F(uint32_t, Args...)>::type>::value>::type
		call(const F & f, uint32_t unlinkId, std::string & result, std::tuple<Arg, Args...> tp) {
		auto r = call_helper(f, std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), unlinkId);
		msgpack_codec codec;
		result = msgpack_codec::pack_args_str(result_code::OK, r);
	}

	template<typename F, typename Self, size_t... Indexes, typename Arg, typename... Args>
	static typename std::result_of<F(Self, uint32_t, Args...)>::type call_member_helper(
		const F & f, Self * self, const std::index_sequence<Indexes...>&,
		std::tuple<Arg, Args...> tup, uint32_t unlinkId = 0) {
		return (*self.*f)(unlinkId, std::move(std::get<Indexes + 1>(tup))...);
	}

	template<typename F, typename Self, typename Arg, typename... Args>
	static typename std::enable_if<
		std::is_void<typename std::result_of<F(Self, uint32_t, Args...)>::type>::value>::type
		call_member(const F & f, Self * self, uint32_t unlinkId, std::string & result,
			std::tuple<Arg, Args...> tp) {
		call_member_helper(f, self, typename std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), unlinkId);
		result = msgpack_codec::pack_args_str(result_code::OK);
	}

	template<typename F, typename Self, typename Arg, typename... Args>
	static typename std::enable_if<
		!std::is_void<typename std::result_of<F(Self, uint32_t, Args...)>::type>::value>::type
		call_member(const F & f, Self * self, uint32_t unlinkId, std::string & result,
			std::tuple<Arg, Args...> tp) {
		auto r =
			call_member_helper(f, self, typename std::make_index_sequence<sizeof...(Args)>{}, std::move(tp), unlinkId);
		result = msgpack_codec::pack_args_str(result_code::OK, r);
	}

	template<typename Function, ExecMode mode = ExecMode::sync>
	struct invoker {
		template<ExecMode model>
		static inline void apply(const Function& func, uint32_t unlinkId, const char* data, size_t size,
			std::string& result, ExecMode& exe_model) {
			using args_tuple = typename function_traits<Function>::args_tuple_2nd;
			exe_model = ExecMode::sync;
			msgpack_codec codec;
			try {
				auto tp = codec.unpack<args_tuple>(data, size);
				call(func, unlinkId, result, std::move(tp));
				exe_model = model;
			}
			catch (std::invalid_argument & e) {
				result = codec.pack_args_str(result_code::FAIL, e.what());
			}
			catch (const std::exception & e) {
				result = codec.pack_args_str(result_code::FAIL, e.what());
			}
		}

		template<ExecMode model, typename Self>
		static inline void apply_member(const Function& func, Self* self, uint32_t unlinkId,
			const char* data, size_t size, std::string& result,
			ExecMode& exe_model) {
			using args_tuple = typename function_traits<Function>::args_tuple_2nd;
			exe_model = ExecMode::sync;
			msgpack_codec codec;
			try {
				auto tp = codec.unpack<args_tuple>(data, size);
				call_member(func, self, unlinkId, result, std::move(tp));
				exe_model = model;
			}
			catch (std::invalid_argument & e) {
				result = codec.pack_args_str(result_code::FAIL, e.what());
			}
			catch (const std::exception & e) {
				result = codec.pack_args_str(result_code::FAIL, e.what());
			}
		}
	};

	template<ExecMode model, typename Function>
	void register_nonmember_func(NF_SERVER_TYPES eServerType, const std::string& name, Function f) {
		map_invokers_[name] = { std::bind(&invoker<Function>::template apply<model>, std::move(f), std::placeholders::_1,
											   std::placeholders::_2, std::placeholders::_3,
											   std::placeholders::_4, std::placeholders::_5) };
		//RegisterRpcMemberFunc(eServerType, name, cb);
	}

	template<ExecMode model, typename Function, typename Self>
	void register_member_func(NF_SERVER_TYPES eServerType, const std::string& name, const Function& f, Self* self) {
		map_invokers_[name] = { std::bind(&invoker<Function>::template apply_member<model, Self>,
											   f, self, std::placeholders::_1, std::placeholders::_2,
											   std::placeholders::_3, std::placeholders::_4,
											   std::placeholders::_5) };
		//RegisterRpcMemberFunc(eServerType, name, cb);
	}

	virtual void RegisterRpcMemberFunc(NF_SERVER_TYPES eServerType, const std::string& name, const std::function<void(uint32_t, const char*, size_t, std::string&, ExecMode& model)>& cb) = 0;

	std::unordered_map<std::string,
		std::function<void(uint32_t, const char*, size_t, std::string&, ExecMode& model)>>
		map_invokers_;
};