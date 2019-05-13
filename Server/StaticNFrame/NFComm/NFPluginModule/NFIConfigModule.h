// -------------------------------------------------------------------------
//    @FileName         :    NFIConfigModule.h
//    @Author           :    GaoYi
//    @Date             :    2018-12-1
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFIModule.h"

#include "NFIPluginManager.h"
#include "NFServerDefine.h"
#include "NFIObject.h"

#include <unordered_map>
#include <vector>

#define DEFINE_LUA_STRING_LOAD_PLUGIN			"LoadPlugin"
#define DEFINE_LUA_STRING_SERVER_PLUGINS		"ServerPlugins"
#define DEFINE_LUA_STRING_SERVER_TYPE			"ServerType"
#define DEFINE_LUA_STRING_SERVER				"ServerList"

#define DEFINE_LUA_STRING_SERVER_NAME			"ServerName"
#define DEFINE_LUA_STRING_SERVER_ID				"ServerId"
#define DEFINE_LUA_STRING_SERVER_GAME_NAME		"GameName"
#define DEFINE_LUA_STRING_SERVER_MONGO_IP		"MongoIp"
#define DEFINE_LUA_STRING_SERVER_MONGO_PORT		"MongoPort"
#define DEFINE_LUA_STRING_SERVER_MONGO_DBNAME	"MongonName"
#define DEFINE_LUA_STRING_SERVER_ZONE_ID		"ZoneId"
#define DEFINE_LUA_STRING_SERVER_IP				"ServerIp"
#define DEFINE_LUA_STRING_SERVER_PORT			"ServerPort"
#define DEFINE_LUA_STRING_MAX_CONNECT_NUM		"MaxConnectNum"
#define DEFINE_LUA_STRING_WORK_THREAD_NUM		"WorkThreadNum"
#define DEFINE_LUA_STRING_SECURITY				"Security"
#define DEFINE_LUA_STRING_LOG_LEVEL				"LogLevel"					//log等级配置
#define DEFINE_LUA_STRING_LOG_FLUSH_LEVEL		"LogFlushLevel"				//log刷新等级配置
#define DEFINE_LUA_STRING_WebSocket				"WebSocket"
#define DEFINE_LUA_STRING_HttpPort				"HttpPort"

#define DEFINE_LUA_STRING_LOG_INFO				"LogInfo"			//log配置

enum EnumLogInfoLuaString
{
	LOG_INFO_LOG_ID = 0,
	LOG_INFO_DISPLAY = 1,
	LOG_INFO_LEVEL = 2,
	LOG_INFO_LOG_NAME = 3,
	LOG_INFO_LOG_GUID = 4,
};

struct NFPluginConfig
{
	NFPluginConfig()
	{
		mServerType = NF_ST_NONE;
	}

public:
	std::vector<std::string> mVecPlugins;
	uint32_t mServerType;
};

struct NFServerConfig
{
public:
	NFServerConfig()
	{
		mServerType = NF_ST_NONE;
		mServerId = 0;
		mMongoPort = 0;
		mServerPort = 0;
		mMaxConnectNum = 100;
		mWorkThreadNum = 1;
		mSecurity = false;
		mWebSocket = false;
		mHttpPort = 0;
		mMysqlPort = 0;
		mNosqlPort = 0;
	}

public:
	std::string mServerName;	//服务器名字
	uint32_t mServerType;	//服务器类型
	uint32_t mServerId;		//服务器唯一ID
	std::string mMongoIp;	//mongodb 地址
	uint32_t	mMongoPort;	//mongodb port
	std::string mMongoDbName; //mongo dbname数据库名字
	std::string mMysqlIp;
	uint32_t	mMysqlPort;
	std::string mMysqlDbName;
	std::string mMysqlUser;
	std::string mMysqlPassword;
	std::string mNosqlIp;
	uint32_t	mNosqlPort;
	std::string mServerIp;  //服务器监听IP
	uint32_t mServerPort;	//服务器监听端口
	uint32_t mHttpPort;		//服务器http监听ip
	uint32_t mMaxConnectNum;	//最大连接数目
	uint32_t mWorkThreadNum;	//线程数目
	bool mSecurity;				//是否加密
	bool mWebSocket;			//是否开启websocket功能
};

struct NFClassNode
{
	NFClassNode()
	{
		mNodeType = 0;
		mSave = false;
		mPrivate = false;
		mPublic = false;
	}
	std::string mNodeName;
	uint32_t mNodeType;
	bool mSave;
	bool mPublic;
	bool mPrivate;
	int8_t mFeature;
	std::string mDBTable;
	std::string mDesc;
	std::vector<std::string> mVecTableName;
};

struct NFClassObject
{
	std::string mClassName;
	std::unordered_map<std::string, NFClassNode> mClassNodeMap;
	std::vector<NFClassNode> mClassNodeArray;
};

struct NFDBTableCol
{
	NFDBTableCol()
	{
		mPrimaryKey = false;
		mIndex = false;
		mAutoIncrement = false;
		mColTypeLength = 0;
	}
	std::string mColName;
	std::string mColType;
	uint32_t mColTypeNum;
	uint32_t mColTypeLength;	//如果列类型是字符串,这里是字符串长度
	bool mPrimaryKey;
	bool mIndex;
	bool mAutoIncrement;
};

struct NFDBTable
{
	std::string mTableName;
	std::string mPrimaryKeyCol;
	std::unordered_map<std::string, NFDBTableCol> mDBTableColMap;
};

class NFIConfigModule : public NFIModule
{
public:
	virtual NFPluginConfig* GetPluginConfig(const std::string& pluginName) = 0;
	virtual NFServerConfig* GetServerConfig(uint32_t serverId) = 0;
	virtual std::vector<NFServerConfig*> GetServerConfigFromServerType(uint32_t serverType) = 0;

	virtual NFIObject* GetGlobalConfig() const = 0;
	virtual NFClassObject* GetClassObject(const std::string& className) const = 0;
	virtual void ProductFile() = 0;
};
