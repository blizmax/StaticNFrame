// -------------------------------------------------------------------------
//    @FileName         :    NFRpcConstVar.h
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
#include <cstdint>

enum class NFRpcResultCode {
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

enum class NFRpcRequestType {
	req_none = 0,
	req_res,
	sub_pub
};

enum class ExecMode { sync, async };
const constexpr ExecMode Async = ExecMode::async;

static const size_t MAX_BUF_LEN = 1048576 * 10;
static const size_t HEAD_LEN = 13;
static const size_t INIT_BUF_SIZE = 2 * 1024;

