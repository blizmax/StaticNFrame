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

class NFIObject;

class _NFExport NFProtobufCommon
{
public:
	static std::string GetFieldsString(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc);
	static bool MessageToJsonString(const google::protobuf::Message& message, std::string& json);
	static bool JsonStringToMessage(const string& json, google::protobuf::Message& message);

	static bool NFObjectToMessage(NFIObject* pObject, google::protobuf::Message& message);
	static bool NFObjectFromMessage(NFIObject* pObject, const google::protobuf::Message& message);

	/*
	** ��message���÷���ȡ���������������,message�������������Ϣ
	*/
	static bool NFObjectAddTableFromMessage(NFIObject* pObject, const std::string&tableName, const google::protobuf::Message& message);

	/*
	** ��message���÷���ȡ���������������,message����������������Ϣ, �����������db_fields������
	*/
	static bool NFObjectAddTableFromMessage(NFIObject* pObject, const google::protobuf::Message& message);

	/*
	** ��message���÷���ȡ������������һ������,message�������������Ϣ
	*/
	static bool NFObjectAddTableRowsFromMessage(NFIObject* pObject, const std::string& tableName, const google::protobuf::Message& message);

	/*
	** ��message���÷���ȡ������������һ�л��������,message����������������Ϣ, �����������db_fields������
	*/
	static bool NFObjectAddTableRowsFromMessage(NFIObject* pObject, const google::protobuf::Message& message);

	static void SetFieldsString(google::protobuf::Message& message, const google::protobuf::FieldDescriptor* pFieldDesc, const std::string& strValue);

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



