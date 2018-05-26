// -------------------------------------------------------------------------
//    @FileName         :    NFDuration.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFDuration.h"

const int64_t NFDuration::kNanosecond = 1LL;
const int64_t NFDuration::kMicrosecond = 1000 * kNanosecond;
const int64_t NFDuration::kMillisecond = 1000 * kMicrosecond;
const int64_t NFDuration::kSecond = 1000 * kMillisecond;
const int64_t NFDuration::kMinute = 60 * kSecond;
const int64_t NFDuration::kHour = 60 * kMinute;

