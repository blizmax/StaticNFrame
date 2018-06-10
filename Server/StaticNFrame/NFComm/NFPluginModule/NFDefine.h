// -------------------------------------------------------------------------
//    @FileName         :    NFDefine.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include <functional>
#include <stdint.h>
#include <string.h>

class NFCData;

using DATA_NODE_EVENT_FUNCTOR = std::function<int(const uint64_t, const std::string&, const NFCData&, const NFCData&)>;
using DATA_NODE_EVENT_FUNCTOR_PTR = NF_SHARE_PTR<DATA_NODE_EVENT_FUNCTOR>;

