// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestActorModule.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include <NFComm/NFPluginModule/NFIMysqlModule.h>
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"

#include "mongoc/mongoc.h"


NFCTestActorModule::NFCTestActorModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCTestActorModule::~NFCTestActorModule()
{
}

bool NFCTestActorModule::Init()
{
	return true;
}

bool NFCTestActorModule::AfterInit()
{
	mongoc_uri_t *uri = nullptr;
	mongoc_client_t *client;
	mongoc_database_t *database;
	mongoc_collection_t *collection;
	bson_t *command, reply, *insert;
	bson_error_t error;
	char *str;
	bool retval;

	std::string uri_string = "mongodb://14.17.104.12:28900";
	mongoc_init();
	uri = mongoc_uri_new_with_error(uri_string.c_str(), &error);
	if (!uri) {
		NFLogError("failed to parse URI:{}\n, error message:{}\n", uri_string, error.message);
		return true;
	}

	client = mongoc_client_new_from_uri(uri);
	if (!client)
	{
		return true;
	}

	database = mongoc_client_get_database(client, "ttr-1");
	if (!database)
	{
		return true;
	}
	collection = mongoc_client_get_collection(client, "ttr_1", "test");
	if (!collection)
	{
		return true;
	}

	command = BCON_NEW("ping", BCON_INT32(1));

	retval = mongoc_client_command_simple(client, "ttr_1", command, NULL, &reply, &error);

	if (!retval)
	{
		NFLogError("{}", error.message);
	}

	str = bson_as_json(&reply, NULL);
	NFLogInfo("{}", str);

	insert = BCON_NEW("uid", BCON_UTF8("world"));
	if (!mongoc_collection_insert_one(collection, insert, NULL, NULL, &error))
	{
		NFLogError("{}", error.message);
	}
	return true;
}

bool NFCTestActorModule::Execute()
{
	return true;
}

bool NFCTestActorModule::BeforeShut()
{
	return true;
}

bool NFCTestActorModule::Shut()
{
	return true;
}