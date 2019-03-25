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

	/*
	** sqlite,mysql从message利用反射,将message的field name做为数据的列，将列名 和类型出去来作为字符串 
	*/
	static std::string GetSqliteColumnFromMessage(const google::protobuf::Message& message, bool firstKey = true);

	/*
	** sqlite从message利用反射,将message的field name生成prepare需要的语句,比如
	** insert into UserInOutRecord values(?,?,?,?,?,?)
	*/
	static std::string GetSqlitePrepareFromMessage(const google::protobuf::Message& message);
};



