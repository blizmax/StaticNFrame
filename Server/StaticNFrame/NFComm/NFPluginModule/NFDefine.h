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
class DATA_TABLE_EVENT_DATA
{
public:
    DATA_TABLE_EVENT_DATA()
        : nOpType(0), nRow(-1), nCol(-1)
    {
    }

	uint8_t nOpType;
    int16_t nRow;
    int16_t nCol;
    std::string strName;
};

using DATA_NODE_EVENT_FUNCTOR = std::function<int(const uint64_t, const std::string&, const NFCData&, const NFCData&)>;
using DATA_NODE_EVENT_FUNCTOR_PTR = NF_SHARE_PTR<DATA_NODE_EVENT_FUNCTOR>;

using DATA_TABLE_EVENT_FUNCTOR = std::function<int(const uint64_t, const DATA_TABLE_EVENT_DATA&, const NFCData&, const NFCData&)>;
using DATA_TABLE_EVENT_FUNCTOR_PTR = NF_SHARE_PTR<DATA_TABLE_EVENT_FUNCTOR>;

