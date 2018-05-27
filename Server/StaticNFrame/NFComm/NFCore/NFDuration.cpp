// -------------------------------------------------------------------------
//    @FileName         :    NFDuration.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFDuration.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
int64_t NFDuration::kNanosecond = 1LL;
int64_t NFDuration::kMicrosecond = 1000 * kNanosecond;
int64_t NFDuration::kMillisecond = 1000 * kMicrosecond;
int64_t NFDuration::kSecond = 1000 * kMillisecond;
int64_t NFDuration::kMinute = 60 * kSecond;
int64_t NFDuration::kHour = 60 * kMinute;
#endif

