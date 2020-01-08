#pragma once
#include <cstdint>

enum class NFRpcResultCode : std::int16_t {
	OK = 0,
	FAIL = 1,
};

enum class NFRpcErrorCode {
	OK,
	UNKNOWN,
	FAIL,
	TIMEOUT,
	CANCEL,
	BADCONNECTION,
};

enum class NFRpcRequestType : uint8_t {
	req_one = 0,
	req_res,
	sub_pub
};

static const size_t MAX_BUF_LEN = 1048576 * 10;
static const size_t HEAD_LEN = 13;
static const size_t INIT_BUF_SIZE = 2 * 1024;
