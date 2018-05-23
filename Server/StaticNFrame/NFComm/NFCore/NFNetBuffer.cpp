// -------------------------------------------------------------------------
//    @FileName         :    NFNetBuffer.cpp
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#include "NFNetBuffer.h"

const char NFNetBuffer::kCRLF[] = "\r\n";

const size_t NFNetBuffer::kCheapPrependSize = 8;
const size_t NFNetBuffer::kInitialSize = 1024;