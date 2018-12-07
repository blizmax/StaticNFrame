
#include "NFCMongoDriver.h"


#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

NFCMongoDriver::NFCMongoDriver(const int nReconnectTime, const int nReconnectCount)
{
	m_pClient = nullptr;
	m_pDatabase = nullptr;
	m_ip = "localhost";
	m_port = 27017;
	mfCheckReconnect = 0.0f;
	mnReconnectTime = nReconnectTime;
	mnReconnectCount = nReconnectCount;
	mongoc_init();
}

NFCMongoDriver::~NFCMongoDriver()
{
	CloseConnection();

	mongoc_cleanup();
}

bool NFCMongoDriver::Connect(const std::string& ip, uint32_t port, const std::string& dbname)
{
	m_ip = ip;
	m_port = port;
	m_dbname = dbname;

	return Connect();
}

bool NFCMongoDriver::Connect()
{
	bson_error_t error;
	std::string str = NF_FORMAT("mongodb://{}:{}", m_ip, m_port);
	m_uri = mongoc_uri_new_with_error(str.c_str(), &error);
	if (m_uri == nullptr)
	{
		NFLogError("Failed to parse URI:{}", str);
		return HandleMongocError(error);
	}

	m_pClient = mongoc_client_new_from_uri(m_uri);
	if (m_pClient == nullptr)
	{
		NFLogError("Mongo Client Connect Error:{}", str);
		return false;
	}

	mongoc_client_set_appname(m_pClient, "StaticNFrame");

	m_pDatabase = mongoc_client_get_database(m_pClient, m_dbname.c_str());
	if (m_pDatabase == nullptr)
	{
		NFLogError("Mongo Client Get DbBase Error, ip:{}, dbName:{}", str, m_dbname);
		return false;
	}
	return true;
}

/**
* @brief 循环执行
*
* @return bool
*/
bool NFCMongoDriver::Execute()
{
	if (IsNeedReconnect() && CanReconnect())
	{
		Reconnect();
	}

	return true;
}

bool NFCMongoDriver::Ping()
{
	bson_t *command;
	bson_t	reply;
	bson_error_t error;
	bool retval;

	command = BCON_NEW("ping", BCON_INT32(1));

	retval = mongoc_client_command_simple(m_pClient, "admin", command, NULL, &reply, &error);

	if (!retval) {
		NFLogError("Mongo Ping Error:{}", error.message)
		return false;
	}

	bson_destroy(&reply);
	bson_destroy(command);

	return true;
}

void NFCMongoDriver::CloseConnection()
{
	for (mongoc_collection_t* pCollection = m_collectionMap.First(); nullptr != pCollection; pCollection = m_collectionMap.Next())
	{
		mongoc_collection_destroy(pCollection);
	}
	m_collectionMap.ClearAll();

	if (m_pClient)
	{
		mongoc_client_destroy(m_pClient);
	}

	if (m_pDatabase)
	{
		mongoc_database_destroy(m_pDatabase);
	}

	if (m_uri)
	{
		mongoc_uri_destroy(m_uri);
	}

	m_pClient = nullptr;
	m_pDatabase = nullptr;
	m_uri = nullptr;
}

bool NFCMongoDriver::Enable()
{
	return !IsNeedReconnect();
}

bool NFCMongoDriver::IsNeedReconnect()
{
	//没有配置表
	if (m_ip.length() < 1 || m_dbname.length() < 1)
	{
		return false;
	}

	if (NULL == m_pClient || m_pDatabase == NULL)
	{
		return true;
	}

	if (!Ping())
	{
		CloseConnection();
		return true;
	}

	return false;
}


bool NFCMongoDriver::CanReconnect()
{
	mfCheckReconnect += 0.1f;

	//30分钟检查断线重连
	if (mfCheckReconnect < mnReconnectTime)
	{
		return false;
	}

	if (mnReconnectCount == 0)
	{
		return false;
	}

	mfCheckReconnect = 0.0f;

	return true;
}

bool NFCMongoDriver::Reconnect()
{
	CloseConnection();
	Connect();

	if (mnReconnectCount > 0)
	{
		mnReconnectCount--;
	}

	return true;
}

bool NFCMongoDriver::HandleMongocError(const bson_error_t& error)
{
	if (!error.domain || !error.code || !error.message[0])
	{
		return true;
	}

	NFLogError("mongoc error:{}", error.message);
	return false;
}

bool NFCMongoDriver::IsExistCollection(const std::string& collectionName)
{
	if (m_pDatabase == nullptr) return false;

	bson_error_t error;
	bool ret = mongoc_database_has_collection(m_pDatabase, collectionName.c_str(), &error);
	HandleMongocError(error);
	return ret;
}

mongoc_collection_t* NFCMongoDriver::GetCollection(const std::string& collectionName)
{
	auto pCollection = m_collectionMap.GetElement(collectionName);
	if (pCollection == nullptr)
	{
		pCollection = mongoc_database_get_collection(m_pDatabase, collectionName.c_str());
		if (pCollection)
		{
			m_collectionMap.AddElement(collectionName, pCollection);
		}
	}
	return pCollection;
}

bool NFCMongoDriver::DropCollection(const std::string& collectionName)
{
	mongoc_collection_t *collection = GetCollection(collectionName);
	if (collection == nullptr)
	{
		NFLogError("DropCollection Failed, collection:{} not exist!", collectionName);
		return false;
	}

	bson_error_t error;
	bool ret = mongoc_collection_drop(collection, &error);
	if (ret == false)
	{
		NFLogError("Drop Collection Failed, collectionName:{}", collectionName);
		return HandleMongocError(error);
	}
	return ret;
}

std::string NFCMongoDriver::FindKey(const std::string& collectionName, int64_t key)
{
	std::string result;
	mongoc_collection_t *collection = GetCollection(collectionName);
	if (!collection)
	{
		NFLogError("Collection Name Not Exist:{}", collectionName);
		return result;
	}

	mongoc_cursor_t *cursor;
	const bson_t *doc;

	bson_error_t error;
	bson_t *query = bson_new();
	BSON_APPEND_INT64(query, "_id", key);

	char *str;
	bson_t opts;
	bson_init(&opts);
	BSON_APPEND_INT32(&opts, "limit", 1);
	BSON_APPEND_BOOL(&opts, "singleBatch", true);

	cursor = mongoc_collection_find_with_opts(collection, query, &opts, nullptr);
	if (mongoc_cursor_next(cursor, &doc) == false)
	{
		if (mongoc_cursor_error(cursor, &error))
		{
			HandleMongocError(error);
		}
	}
	else
	{
		str = bson_as_canonical_extended_json(doc, NULL);
		result = str;
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	bson_destroy(&opts);
	return result;
}

std::string NFCMongoDriver::FindOne(const std::string& collectionName, const std::string& json_query)
{
	std::string result;
	mongoc_collection_t *collection = GetCollection(collectionName);
	if (!collection)
	{
		NFLogError("Collection Name Not Exist:{}", collectionName);
		return result;
	}

	mongoc_cursor_t *cursor;
	const bson_t *doc;

	bson_error_t error;
	bson_t *query = bson_new_from_json((const uint8_t*)json_query.data(), json_query.length(), &error);
	if (HandleMongocError(error) == false)
	{
		bson_destroy(query);
		NFLogError("bson_new_from_json error:{}", json_query);
		return result;
	}
	
	char *str;
	bson_t opts;
	bson_init(&opts);
	BSON_APPEND_INT32(&opts, "limit", 1);
	BSON_APPEND_BOOL(&opts, "singleBatch", true);

	cursor = mongoc_collection_find_with_opts(collection, query, &opts, nullptr);
	if (mongoc_cursor_next(cursor, &doc) == false)
	{
		if (mongoc_cursor_error(cursor, &error)) 
		{
			HandleMongocError(error);
		}
	}
	else
	{
		str = bson_as_canonical_extended_json(doc, NULL);
		result = str;
		bson_free(str);
	}
	
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	bson_destroy(&opts);
	return result;
}

std::vector<std::string> NFCMongoDriver::FindMany(const std::string& collectionName, const std::string& json_query)
{
	std::vector<std::string> result;
	mongoc_collection_t *collection = GetCollection(collectionName);
	if (!collection)
	{
		NFLogError("Collection Name Not Exist:{}", collectionName);
		return result;
	}

	mongoc_cursor_t *cursor;
	const bson_t *doc;

	bson_error_t error;
	bson_t *query = bson_new_from_json((const uint8_t*)json_query.data(), json_query.length(), &error);
	if (HandleMongocError(error) == false)
	{
		if (query) bson_destroy(query);
		NFLogError("bson_new_from_json error:{}", json_query);
		return result;
	}

	char *str;
	bson_t opts;
	bson_init(&opts);
	BSON_APPEND_INT32(&opts, "limit", 1);
	BSON_APPEND_BOOL(&opts, "singleBatch", true);

	cursor = mongoc_collection_find_with_opts(collection, query, &opts, nullptr);
	while (mongoc_cursor_next(cursor, &doc))
	{
		str = bson_as_canonical_extended_json(doc, NULL);
		result.push_back(std::string(str));
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	bson_destroy(&opts);
	return result;
}

std::vector<std::string> NFCMongoDriver::FindAll(const std::string& collectionName)
{
	std::vector<std::string> result;
	mongoc_collection_t *collection = GetCollection(collectionName);
	if (!collection)
	{
		NFLogError("Collection Name Not Exist:{}", collectionName);
		return result;
	}

	mongoc_cursor_t *cursor;
	const bson_t *doc;

	bson_t *query = bson_new();
	char *str;

	cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
	while (mongoc_cursor_next(cursor, &doc))
	{
		str = bson_as_canonical_extended_json(doc, NULL);
		result.push_back(std::string(str));
		bson_free(str);
	}

	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	return result;
}

bool NFCMongoDriver::CreateCollection(const std::string& collectionName, const std::string& primay_key)
{
	if (m_pDatabase == nullptr) return false;

	bson_error_t error;
	bson_t keys;
	mongoc_index_opt_t opt;

	mongoc_collection_t *collection = GetCollection(collectionName);
	if (!collection)
	{
		collection = mongoc_database_create_collection(m_pDatabase, collectionName.c_str(), nullptr, &error);
		if (collection == nullptr)
		{
			NFLogError("Create Collection Failed, collectionName:{}", collectionName);
			return HandleMongocError(error);
		}

		m_collectionMap.AddElement(collectionName, collection);
	}

	if (primay_key.empty())
	{
		return true;
	}

	mongoc_index_opt_init(&opt);

	bson_init(&keys);
	bson_append_int32(&keys, primay_key.c_str(), -1, 1);

	opt.unique = true;

	bool ret = mongoc_collection_ensure_index(collection, &keys, &opt, &error);
	if (ret == false)
	{
		NFLogError("Create Collection Index Failed, collectionName:{}, key:{}", collectionName, primay_key);

		HandleMongocError(error);

		bson_destroy(&keys);
		return false;
	}

	if (collectionName != PRIMARY_TABLE)
	{
		InsertPrimaryKey(collectionName, primay_key);
	}

	bson_destroy(&keys);
	return true;
}

void NFCMongoDriver::FindAllPrimaryKey()
{
	std::vector<std::string> resultVec = FindAll(PRIMARY_TABLE);
	for (int i = 0; i < resultVec.size(); i++)
	{
		const std::string& result = resultVec[i];
		NFMsg::mongo_primary_key primary_key;
		NFServerCommon::JsonStringToMessage(result, primary_key);

		m_collectionPrimaryKeyMap.emplace(primary_key._id(), primary_key.primary());
	}
}

bool NFCMongoDriver::InsertPrimaryKey(const std::string& collectionName, const std::string& primaryKey)
{
	bson_t *doc = bson_new();
	BSON_APPEND_UTF8(doc, PRIMARY_TABLE_KEY, collectionName.c_str());
	BSON_APPEND_UTF8(doc, PRIMARY_TABLE_COL, primaryKey.c_str());
	bool ret = InsertOne(PRIMARY_TABLE, doc);
	bson_destroy(doc);

	m_collectionPrimaryKeyMap.emplace(collectionName, primaryKey);
	return ret;
}

bool NFCMongoDriver::InsertOne(const std::string& collectionName, bson_t *doc)
{
	if (doc == nullptr) return false;

	bson_error_t error;
	mongoc_collection_t *collection = GetCollection(collectionName);
	if (!collection)
	{
		NFLogError("Collection Name Not Exist:{}", collectionName);
		return false;
	}

	bool ret = mongoc_collection_insert_one(collection, doc, NULL, NULL, &error);
	if (ret == false)
	{
		HandleMongocError(error);
		return false;
	}

	return true;
}

bool NFCMongoDriver::InsertOne(const std::string& collectionName, const google::protobuf::Message& message)
{
	std::string jsonStr;
	if (NFServerCommon::MessageToJsonString(message, jsonStr) == false)
	{
		return false;
	}

	return InsertOne(collectionName, jsonStr);
}

bool NFCMongoDriver::InsertOne(const std::string& collectionName, const std::string& json_query)
{
	bson_error_t error;
	if (collectionName != PRIMARY_TABLE)
	{
		std::string primary_key = m_collectionPrimaryKeyMap[collectionName];
		if (primary_key.length() > 0 && primary_key != PRIMARY_TABLE_KEY)
		{
			rapidjson::Document document;
			document.Parse(json_query.c_str());
			
			if (document.IsObject() && !document.HasMember(PRIMARY_TABLE_KEY))
			{
				if (document.HasMember(primary_key.c_str()) == false)
				{
					NFLogError("json hast not primary key:{}", primary_key);
					return false;
				}

				rapidjson::Value& value = document[primary_key.c_str()];
				document.AddMember(rapidjson::Value::StringRefType(PRIMARY_TABLE_KEY), rapidjson::Value().CopyFrom(value, document.GetAllocator()), document.GetAllocator());

				rapidjson::StringBuffer buffer;
				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				document.Accept(writer);

				bson_t *doc = bson_new_from_json((const uint8_t*)buffer.GetString(), buffer.GetSize(), &error);
				if (HandleMongocError(error) == false || doc == nullptr)
				{
					if (doc) bson_destroy(doc);
					NFLogError("bson_new_from_json error:{}", json_query);
					return false;
				}

				bool ret = InsertOne(collectionName, doc);
				bson_destroy(doc);
				return ret;
			}
		}
	}
	
	bson_t *doc = bson_new_from_json((const uint8_t*)json_query.data(), json_query.length(), &error);
	if (HandleMongocError(error) == false || doc == nullptr)
	{
		if (doc) bson_destroy(doc);
		NFLogError("bson_new_from_json error:{}", json_query);
		return false;
	}

	bool ret = InsertOne(collectionName, doc);
	bson_destroy(doc);
	return ret;
}

bool NFCMongoDriver::Update(const std::string& json)
{
	return true;
}

