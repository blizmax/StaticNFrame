// -------------------------------------------------------------------------
//    @FileName         :    NFRpcUtils.h
//    @Author           :    GaoYi
//    @Date             :    2020/1/08
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                    _ooOoo_
//                   o8888888o
//                   88" . "88
//                   (| -_- |)
//                    O\ = /O
//                ____/`---'\____
//              .   ' \\| |// `.
//               / \\||| : |||// \
//             / _||||| -:- |||||- \
//               | | \\\ - /// | |
//            | \_| ''\---/'' | |
//              \ .-\__ `-` ___/-. /
//           ___`. .' /--.--\ `. . __
//        ."" '< `.___\_<|>_/___.' >'"".
//       | | : `- \`.;`\ _ /`;.`/ - ` : | |
//         \ \ `-. \_ __\ /__ _/ .-` / /
// ======`-.____`-.___\_____/___.-`____.-'======
//                    `=---='
//
// .............................................
//          ·ð×æ±£ÓÓ             ÓÀÎÞBUG
//
// -------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <future>
#include "NFMsgPackCode.h"
#include "NFRpcConstVar.h"

inline bool RpcHasError(std::string result) {
	if (result.empty()) {
		return true;
	}

	msgpack_codec codec;
	auto tp = codec.unpack<std::tuple<int>>(result.data(), result.size());

	return std::get<0>(tp) != 0;
}

template<typename T>
inline T NFRpcGetResult(std::string result) {
	msgpack_codec codec;
	auto tp = codec.unpack<std::tuple<int, T>>(result.data(), result.size());
	return std::get<1>(tp);
}


inline std::string NFRpcGetErrorMsg(std::string result) {
	msgpack_codec codec;
	auto tp = codec.unpack<std::tuple<int, std::string>>(result.data(), result.size());
	return std::get<1>(tp);
}

template<typename T>
inline T as(std::string result) {
	if (RpcHasError(result)) {
		throw std::logic_error(NFRpcGetErrorMsg(result));
	}

	return NFRpcGetResult<T>(result);
}

class NFRpcReqResult {
public:
	NFRpcReqResult() = default;
	NFRpcReqResult(std::string data) : data_(data.data(), data.length()) {}
	bool success() const {
		return !RpcHasError(data_);
	}

	template<typename T>
	T as() {
		if (RpcHasError(data_)) {
			throw std::logic_error(NFRpcGetErrorMsg(data_));
		}

		return NFRpcGetResult<T>(data_);
	}

	void as() {
		if (RpcHasError(data_)) {
			throw std::logic_error(NFRpcGetErrorMsg(data_));
		}
	}

private:
	std::string data_;
};

enum class CallModel {
	future,
	callback
};
const constexpr auto FUTURE = CallModel::future;

const constexpr size_t DEFAULT_TIMEOUT = 5000; //milliseconds
