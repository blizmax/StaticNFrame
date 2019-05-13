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
#define DEFINE_LUA_STRING_LOG_LEVEL				"LogLevel"					//log�ȼ�����
#define DEFINE_LUA_STRING_LOG_FLUSH_LEVEL		"LogFlushLevel"				//logˢ�µȼ�����
#define DEFINE_LUA_STRING_WebSocket				"WebSocket"
#define DEFINE_LUA_STRING_HttpPort				"HttpPort"

#define DEFINE_LUA_STRING_LOG_INFO				"LogInfo"			//log����

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
	std::string mServerName;	//����������
	uint32_t mServerType;	//����������
	uint32_t mServerId;		//������ΨһID
	std::string mMongoIp;	//mongodb ��ַ
	uint32_t	mMongoPort;	//mongodb port
	std::string mMongoDbName; //mongo dbname���ݿ�����
	std::string mMysqlIp;
	uint32_t	mMysqlPort;
	std::string mMysqlDbName;
	std::string mMysqlUser;
	std::string mMysqlPassword;
	std::string mNosqlIp;
	uint32_t	mNosqlPort;
	std::string mServerIp;  //����������IP
	uint32_t mServerPort;	//�����������˿�
	uint32_t mHttpPort;		//������http����ip
	uint32_t mMaxConnectNum;	//���������Ŀ
	uint32_t mWorkThreadNum;	//�߳���Ŀ
	bool mSecurity;				//�Ƿ����
	bool mWebSocket;			//�Ƿ���websocket����
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
	uint32_t mColTypeLength;	//������������ַ���,�������ַ�������
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
