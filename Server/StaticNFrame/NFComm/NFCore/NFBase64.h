// -------------------------------------------------------------------------
//    @FileName         :    NFBase64.h
//    @Author           :    GaoYi
//    @Date             :    2017-08-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once


#include <string>

#include "NFPlatform.h"

class _NFExport NFBase64
{
public:
    NFBase64() {}
    virtual ~NFBase64() {}

    static bool Encode(const std::string& src, std::string* dst);

    static bool Decode(const std::string& src, std::string* dst);

private:
    // 根据在Base64编码表中的序号求得某个字符
    static inline char Base2Chr(unsigned char n);

    // 求得某个字符在Base64编码表中的序号
    static inline unsigned char Chr2Base(char c);

    inline static int Base64EncodeLen(int n);
    inline static int Base64DecodeLen(int n);
};
