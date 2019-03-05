// -------------------------------------------------------------------------
//    @FileName         :    NFProtobufCommon.h
//    @Author           :    GaoYi
//    @Date             :    2019/3/2
//    @Email			:    445267987@qq.com
//    @Module           :    NFProtobufCommon
//
// -------------------------------------------------------------------------
#pragma once

#include "NFMessageDefine/NFMsgDefine.h"

class NFProtobufCommon
{
public:
	static bool MessageToJsonString(const google::protobuf::Message& message, std::string& json);
	static bool JsonStringToMessage(const string& json, google::protobuf::Message& message);
};



