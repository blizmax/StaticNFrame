// -------------------------------------------------------------------------
//    @FileName         :    NFRouter.h
//    @Author           :    GaoYi
//    @Date             :    2020/01/7
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

#include <string.h>
#include <unordered_map>
#include <memory>
#include <functional>


class NFRouter {
public:
	template<ExecMode model, typename Function>
	void register_handler(std::string const& name, Function f) {
		return register_nonmember_func<model>(name, std::move(f));
	}

	template<ExecMode model, typename Function, typename Self>
	void register_handler(std::string const& name, const Function& f, Self* self) {
		return register_member_func<model>(name, f, self);
	}

	void remove_handler(std::string const& name) { this->map_invokers_.erase(name); }

	template<typename T>
	void route(const char* data, std::size_t size, std::weak_ptr<T> conn) {
		auto conn_sp = conn.lock();
		if (!conn_sp) {
			return;
		}

		auto req_id = conn_sp->request_id();
		std::string result;
		try {
			msgpack_codec codec;
			auto p = codec.unpack<std::tuple<std::string>>(data, size);
			auto& func_name = std::get<0>(p);
			auto it = map_invokers_.find(func_name);
			if (it == map_invokers_.end()) {
				result = codec.pack_args_str(result_code::FAIL, "unknown function: " + func_name);
				conn_sp->response(req_id, std::move(result));
				return;
			}

			ExecMode model;
			it->second(conn, data, size, result, model);
			if (model == ExecMode::sync) {
				if (result.size() >= MAX_BUF_LEN) {
					result = codec.pack_args_str(result_code::FAIL, "the response result is out of range: more than 10M " + func_name);
				}
				conn_sp->response(req_id, std::move(result));
			}
		}
		catch (const std::exception & ex) {
			msgpack_codec codec;
			result = codec.pack_args_str(result_code::FAIL, ex.what());
			conn_sp->response(req_id, std::move(result));
		}
	}

	NFRouter() = default;

private:
	NFRouter(const NFRouter&) = delete;
	NFRouter(NFRouter&&) = delete;

	template<typename F, size_t... I, typename Arg, typename... Args>
	static typename std::result_of<F(Args...)>::type call_helper(
		const F & f, const std::index_sequence<I...>&, std::tuple<Arg, Args...> tup) {
		return f(std::move(std::get<I + 1>(tup))...);
	}

	template<typename F, typename Arg, typename... Args>
	static
		typename std::enable_if<std::is_void<typename std::result_of<F(Args...)>::type>::value>::type
		call(const F & f, std::string & result, std::tuple<Arg, Args...> tp) {
		call_helper(f, std::make_index_sequence<sizeof...(Args)>{}, std::move(tp));
		result = msgpack_codec::pack_args_str(result_code::OK);
	}

	template<typename F, typename Arg, typename... Args>
	static
		typename std::enable_if<!std::is_void<typename std::result_of<F(Args...)>::type>::value>::type
		call(const F & f, std::string & result, std::tuple<Arg, Args...> tp) {
		auto r = call_helper(f, std::make_index_sequence<sizeof...(Args)>{}, std::move(tp));
		msgpack_codec codec;
		result = msgpack_codec::pack_args_str(result_code::OK, r);
	}

	template<typename F, typename Self, size_t... Indexes, typename Arg, typename... Args>
	static typename std::result_of<F(Self Args...)>::type call_member_helper(
		const F & f, Self * self, const std::index_sequence<Indexes...>&,
		std::tuple<Arg, Args...> tup) {
		return (*self.*f)(std::move(std::get<Indexes + 1>(tup))...);
	}

	template<typename F, typename Self, typename Arg, typename... Args>
	static typename std::enable_if<
		std::is_void<typename std::result_of<F(Self, Args...)>::type>::value>::type
		call_member(const F & f, Self * self, std::string & result,
			std::tuple<Arg, Args...> tp) {
		call_member_helper(f, self, typename std::make_index_sequence<sizeof...(Args)>{}, std::move(tp));
		result = msgpack_codec::pack_args_str(result_code::OK);
	}

	template<typename F, typename Self, typename Arg, typename... Args>
	static typename std::enable_if<
		!std::is_void<typename std::result_of<F(Self, Args...)>::type>::value>::type
		call_member(const F & f, Self * self, std::string & result,
			std::tuple<Arg, Args...> tp) {
		auto r =
			call_member_helper(f, self, typename std::make_index_sequence<sizeof...(Args)>{}, std::move(tp));
		result = msgpack_codec::pack_args_str(result_code::OK, r);
	}

	template<typename Function, ExecMode mode = ExecMode::sync>
	struct invoker {
		template<ExecMode model>
		static inline void apply(const Function& func, std::weak_ptr<connection> conn, const char* data, size_t size,
			std::string& result, ExecMode& exe_model) {
			using args_tuple = typename function_traits<Function>::args_tuple_2nd;
			exe_model = ExecMode::sync;
			msgpack_codec codec;
			try {
				auto tp = codec.unpack<args_tuple>(data, size);
				call(func, conn, result, std::move(tp));
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
		static inline void apply_member(const Function& func, Self* self,
			const char* data, size_t size, std::string& result,
			ExecMode& exe_model) {
			using args_tuple = typename function_traits<Function>::args_tuple_2nd;
			exe_model = ExecMode::sync;
			msgpack_codec codec;
			try {
				auto tp = codec.unpack<args_tuple>(data, size);
				call_member(func, self, result, std::move(tp));
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
	void register_nonmember_func(std::string const& name, Function f) {
		this->map_invokers_[name] = { std::bind(&invoker<Function>::template apply<model>, std::move(f), std::placeholders::_1,
												std::placeholders::_2, std::placeholders::_3,
												std::placeholders::_4, std::placeholders::_5) };
	}

	template<ExecMode model, typename Function, typename Self>
	void register_member_func(const std::string& name, const Function& f, Self* self) {
		this->map_invokers_[name] = { std::bind(&invoker<Function>::template apply_member<model, Self>,
												f, self, std::placeholders::_1, std::placeholders::_2,
												std::placeholders::_3, std::placeholders::_4,
												std::placeholders::_5) };
	}

	std::unordered_map<std::string,
		std::function<void(const char*, size_t, std::string&, ExecMode& model)>>
		map_invokers_;
};


