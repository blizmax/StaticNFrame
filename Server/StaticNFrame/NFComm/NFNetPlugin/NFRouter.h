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
#include "NFComm/NFPluginModule/NFMsgPackCode.h"
#include "NFComm/NFPluginModule/NFRpcConstVar.h"
#include "NFComm/NFPluginModule/NFRpcUtils.h"
#include "NetEvppObject.h"

enum class ExecMode { sync, async };
const constexpr ExecMode Async = ExecMode::async;

class NFRouter {
public:
	void route(NetEvppObject* pObject, uint64_t req_id, char* data, std::size_t size) {
		std::string result;
		try {
			msgpack_codec codec;
			auto p = codec.unpack<std::tuple<std::string>>(data, size);
			auto& func_name = std::get<0>(p);
			auto it = map_invokers_.find(func_name);
			if (it == map_invokers_.end()) {
				result = codec.pack_args_str(NFRpcResultCode::FAIL, "unknown function: " + func_name);
				pObject->Send((uint8_t)NFRpcRequestType::req_res, req_id, result.data(), result.length());
				return;
			}

			ExecMode model;
			it->second(pObject->GetLinkId(), data, size, result, model);
			if (model == ExecMode::sync) {
				if (result.size() >= MAX_BUF_LEN) {
					result = codec.pack_args_str(NFRpcResultCode::FAIL, "the response result is out of range: more than 10M " + func_name);
				}
				pObject->Send((uint8_t)NFRpcRequestType::req_res, req_id, result.data(), result.length());
			}
		}
		catch (const std::exception & ex) {
			msgpack_codec codec;
			result = codec.pack_args_str(NFRpcResultCode::FAIL, ex.what());
			pObject->Send((uint8_t)NFRpcRequestType::req_res, req_id, result.data(), result.length());
		}
	}
};


