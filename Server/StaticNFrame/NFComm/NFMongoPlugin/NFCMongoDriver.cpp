
#include "NFCMongoDriver.h"


#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

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

bool NFCMongoDriver::CreateCollection(const std::string& collectionName, const std::string& key)
{
	bson_error_t error;
	bson_t keys;
	mongoc_index_opt_t opt;

	if (m_pDatabase == nullptr) return false;

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

	mongoc_index_opt_init(&opt);

	bson_init(&keys);
	bson_append_int32(&keys, key.c_str(), -1, 1);

	opt.unique = true;

	bool ret = mongoc_collection_ensure_index(collection, &keys, &opt, &error);
	if (ret == false)
	{
		NFLogError("Create Collection Index Failed, collectionName:{}, key:{}", collectionName, key);
		return HandleMongocError(error);
	}
	bson_destroy(&keys);

	return true;
}

bool NFCMongoDriver::Update(const std::string& json)
{
	return true;
}

