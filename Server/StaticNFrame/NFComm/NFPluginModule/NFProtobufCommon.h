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
	** sqlite,mysql��message���÷���ȡ�����ݿ�����
	*/
	static std::string GetDBNameFromMessage(const google::protobuf::Message& message);

	/*
	** sqlite,mysql��message���÷���,��message��field name��Ϊ���ݵ��У������� �����ͳ�ȥ����Ϊ�ַ��� 
	*/
	static std::string GetSqliteColumnFromMessage(const google::protobuf::Message& message, bool firstKey = true);

	/*
	** sqlite��message���÷���,��message��field name����prepare��Ҫ�����,����
	** insert into UserInOutRecord values(?,?,?,?,?,?)
	*/
	static std::string GetSqlitePrepareFromMessage(const google::protobuf::Message& message);
};



