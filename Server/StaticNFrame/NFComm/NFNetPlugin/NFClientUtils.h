#pragma once

#pragma once
#include <iostream>
#include <string>
#include <deque>
#include <future>
#include "NFCodec.h"
#include "NFConstVar.h"

inline bool has_error(std::string result) {
	if (result.empty()) {
		return true;
	}

	msgpack_codec codec;
	auto tp = codec.unpack<std::tuple<int>>(result.data(), result.size());

	return std::get<0>(tp) != 0;
}

template<typename T>
inline T get_result(std::string result) {
	rpc_service::msgpack_codec codec;
	auto tp = codec.unpack<std::tuple<int, T>>(result.data(), result.size());
	return std::get<1>(tp);
}


inline std::string get_error_msg(std::string result) {
	msgpack_codec codec;
	auto tp = codec.unpack<std::tuple<int, std::string>>(result.data(), result.size());
	return std::get<1>(tp);
}

template<typename T>
inline T as(std::string result) {
	if (has_error(result)) {
		throw std::logic_error(get_error_msg(result));
	}

	return get_result<T>(result);
}

class req_result {
public:
	req_result() = default;
	req_result(std::string data) : data_(data.data(), data.length()) {}
	bool success() const {
		return !has_error(data_);
	}

	template<typename T>
	T as() {
		if (has_error(data_)) {
			throw std::logic_error(get_error_msg(data_));
		}

		return get_result<T>(data_);
	}

	void as() {
		if (has_error(data_)) {
			throw std::logic_error(get_error_msg(data_));
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