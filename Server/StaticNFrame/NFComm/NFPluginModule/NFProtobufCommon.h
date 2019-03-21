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
#include "NFComm/NFCore/NFPlatform.h"

class _NFExport NFProtobufCommon
{
public:
	static std::string GetFieldsString(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc);
	static bool MessageToJsonString(const google::protobuf::Message& message, std::string& json);
	static bool JsonStringToMessage(const string& json, google::protobuf::Message& message);

	/*
	** sqlite,mysql从message利用反射取出数据库名字
	*/
	static std::string GetDBNameFromMessage(const google::protobuf::Message& message);
};



