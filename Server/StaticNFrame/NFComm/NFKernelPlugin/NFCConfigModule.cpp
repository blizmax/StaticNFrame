// -------------------------------------------------------------------------
//    @FileName         :    NFCConfigModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2018-11-3
//    @Module           :    NFKernelPlugin
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCConfigModule.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"

#define DEFINE_LUA_STRING_LOAD_PLUGIN			"LoadPlugin"
#define DEFINE_LUA_STRING_SERVER_PLUGINS		"ServerPlugins"
#define DEFINE_LUA_STRING_SERVER_TYPE			"ServerType"
#define DEFINE_LUA_STRING_SERVER				"ServerList"

#define DEFINE_LUA_STRING_SERVER_NAME			"ServerName"
#define DEFINE_LUA_STRING_SERVER_ID				"ServerId"
#define DEFINE_LUA_STRING_SERVER_GAME_ID		"GameId"
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
#define DEFINE_LUA_STRING_SERVER_INNER_PROT		"ServerInnerPort"

#define DEFINE_LUA_STRING_LOG_INFO				"LogInfo"			//log配置

NFCConfigModule::NFCConfigModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mLogLevel = NLL_TRACE_NORMAL;
	mLogFlushLevel = NLL_TRACE_NORMAL;
	//比较特殊，必须在这里加载配置，不然plugginmanager::Awake会出问题， 引擎配置没有数据
	LoadConfig();
	NFConfigMgr::Instance()->Init(this);
}

NFCConfigModule::~NFCConfigModule()
{
	for (auto it = mPluginConfig.begin(); it != mPluginConfig.end(); it++)
	{
		NFPluginConfig* pConfig = it->second;
		if (pConfig)
		{
			NF_SAFE_DELETE(pConfig);
		}
	}
	mPluginConfig.clear();

	for (auto it = mServerConfig.begin(); it != mServerConfig.end(); it++)
	{
		NFServerConfig* pConfig = it->second;
		if (pConfig)
		{
			NF_SAFE_DELETE(pConfig);
		}
	}
	mServerConfig.clear();
}

bool NFCConfigModule::LoadConfig()
{
	TryAddPackagePath(m_pPluginManager->GetConfigPath()); //Add Search Path to Lua
	std::list<std::string> fileList;
	NFFileUtility::GetFiles(m_pPluginManager->GetConfigPath(), fileList, true, "*.lua");

	for (auto it = fileList.begin(); it != fileList.end(); it++)
	{
		if (TryLoadScriptFile(*it) == false)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "Load {} Failed!", *it);
			assert(0);
		}
	}

	LoadLogConfig();
	LoadPluginConfig();
	LoadServerConfig();

	return true;
}

bool NFCConfigModule::LoadLogConfig()
{
	mLogInfoConfig.clear();
	mLogLevel = 0;
	mLogFlushLevel = 0;

	GetValue(DEFINE_LUA_STRING_LOG_LEVEL, mLogLevel);
	GetValue(DEFINE_LUA_STRING_LOG_FLUSH_LEVEL, mLogFlushLevel);

	NFLuaRef logInfoRef = GetGlobal(DEFINE_LUA_STRING_LOG_INFO);
	if (!logInfoRef.isValid())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "log.lua can't find ({})", DEFINE_LUA_STRING_LOG_INFO);
		assert(0);
	}

	if (!logInfoRef.isTable())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "{} is not table in the log.lua", DEFINE_LUA_STRING_LOG_INFO);
		assert(0);
	}

	/* lua 是从1开始的 */
	for (int i = 1; i <= logInfoRef.len(); i++)
	{
		NFLuaRef logRef = logInfoRef[i];
		if (!logRef.isTable())
		{
			NFLogError(NF_LOG_LOAD_CONFIG, 0, "logInfo some wrong in the log.lua");
			assert(0);
		}

		LogInfoConfig logConfig;

		GetLuaTableValue(logRef, "logid", logConfig.mLogId);
		GetLuaTableValue(logRef, "display", logConfig.mDisplay);
		GetLuaTableValue(logRef, "level", logConfig.mLevel);
		GetLuaTableValue(logRef, "logname", logConfig.mLogName);

		NFLuaRef guidRef;
		GetLuaTableValue(logRef, "guid", guidRef);

		if (guidRef.isTable())
		{
			for (size_t j = 0; j < guidRef.len(); j++)
			{
				NFLuaRef guidLuaRef = guidRef[j];
				uint64_t guid = guidLuaRef.toValue<uint64_t>();
				if (guid != 0)
				{
					logConfig.mVecGuid.push_back(guid);
				}
			}
		}
		else
		{
			uint64_t guid = guidRef.toValue<uint64_t>();
			if (guid != 0)
			{
				logConfig.mVecGuid.push_back(guid);
			}
		}

		mLogInfoConfig.push_back(logConfig);
	}

	return true;
}

bool NFCConfigModule::LoadPluginConfig()
{
	NFLuaRef pluginRef = GetGlobal(DEFINE_LUA_STRING_LOAD_PLUGIN);
	if (!pluginRef.isValid())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "Plugin.lua can't find ({})", DEFINE_LUA_STRING_LOAD_PLUGIN);
		assert(0);
	}

	if (!pluginRef.isTable())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "{} is not table in the plugin.lua", DEFINE_LUA_STRING_LOAD_PLUGIN);
		assert(0);
	}

	for (auto it = pluginRef.begin(); it != pluginRef.end(); ++it)
	{
		std::string serverPluginName = it.key<std::string>();
		NFLuaRef serverPluginRef = it.value();
		NFLuaRef serverPluginListRef;
		if (!GetLuaTableValue(serverPluginRef, DEFINE_LUA_STRING_SERVER_PLUGINS, serverPluginListRef))
		{
			NFLogError(NF_LOG_LOAD_CONFIG, 0, "{} can't find int server:{} int the table {}  in the plugin.lua", DEFINE_LUA_STRING_SERVER_PLUGINS, serverPluginName, DEFINE_LUA_STRING_LOAD_PLUGIN);
			assert(0);
		}

		NFPluginConfig* pConfig = NF_NEW NFPluginConfig();;
		if (serverPluginListRef.isTable())
		{
			for (int i = 1; i <= serverPluginListRef.len(); i++)
			{
				std::string plugin;
				if (GetLuaTableValue(serverPluginListRef, i, plugin))
				{
					pConfig->mVecPlugins.push_back(plugin);
				}
			}
		}

		if (!GetLuaTableValue(serverPluginRef, DEFINE_LUA_STRING_SERVER_TYPE, pConfig->mServerType))
		{
			NFLogError(NF_LOG_LOAD_CONFIG, 0, "{} can't find int server:{} int the table {}  in the plugin.lua", DEFINE_LUA_STRING_SERVER_PLUGINS, serverPluginName, DEFINE_LUA_STRING_LOAD_PLUGIN);
			assert(0);
		}

		if (pConfig->mServerType >= NF_ST_MAX)
		{
			NFLogError(NF_LOG_LOAD_CONFIG, 0, "{} can't find int server:{} int the table {}  in the plugin.lua", DEFINE_LUA_STRING_SERVER_PLUGINS, serverPluginName, DEFINE_LUA_STRING_LOAD_PLUGIN);
			assert(0);
		}
		mPluginConfig.emplace(serverPluginName, pConfig);
	}

	return true;
}

bool NFCConfigModule::LoadServerConfig()
{
	TryRunGlobalScriptFunc("InitServer");

	NFLuaRef serverRef = GetGlobal(DEFINE_LUA_STRING_SERVER);
	if (!serverRef.isValid())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "lua file can't find the ({})", DEFINE_LUA_STRING_SERVER);
		assert(0);
	}

	for (auto it = serverRef.begin(); it != serverRef.end(); ++it)
	{
		NFLuaRef serverConfigRef = it.value();
		NFServerConfig* pConfig = NF_NEW NFServerConfig();

		if (!GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_ID, pConfig->mServerId))
		{
			NFLogError(NF_LOG_LOAD_CONFIG, 0, "must be config the ServerId........");
			assert(0);
		}

		if (!GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_TYPE, pConfig->mServerType))
		{
			NFLogError(NF_LOG_LOAD_CONFIG, 0, "must be config the ServerType........");
			assert(0);
		}

		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_GAME_ID, pConfig->mGameId);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_NAME, pConfig->mServerName);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_GAME_NAME, pConfig->mGameName);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_MONGO_IP, pConfig->mMongoIp);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_MONGO_PORT, pConfig->mMongoPort);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_MONGO_DBNAME, pConfig->mMongoDbName);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_IP, pConfig->mServerIp);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_PORT, pConfig->mServerPort);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_MAX_CONNECT_NUM, pConfig->mMaxConnectNum);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_WORK_THREAD_NUM, pConfig->mWorkThreadNum);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SECURITY, pConfig->mSecurity);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_INNER_PROT, pConfig->mServerInnerPort);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_WebSocket, pConfig->mWebSocket);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_HttpPort, pConfig->mHttpPort);
		mServerConfig.emplace(pConfig->mServerId, pConfig);
	}

	return true;
}

bool NFCConfigModule::Awake()
{
	return true;
}


bool NFCConfigModule::Init()
{
	return true;
}

bool NFCConfigModule::AfterInit()
{
	return true;
}

bool NFCConfigModule::BeforeShut()
{
	return true;
}

bool NFCConfigModule::Shut()
{
	return true;
}

bool NFCConfigModule::Execute()
{
	return true;
}

bool  NFCConfigModule::OnReloadPlugin()
{
	TryAddPackagePath(m_pPluginManager->GetConfigPath()); //Add Search Path to Lua
	std::list<std::string> fileList;
	NFFileUtility::GetFiles(m_pPluginManager->GetConfigPath(), fileList, true, "*.lua");

	for (auto it = fileList.begin(); it != fileList.end(); it++)
	{
		if (TryLoadScriptFile(*it) == false)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "Load {} Failed!", *it);
			return false;
		}
	}

	LoadLogConfig();

	return true;
}

NFPluginConfig* NFCConfigModule::GetPluginConfig(const std::string& pluginName)
{
	auto it = mPluginConfig.find(pluginName);
	if (it != mPluginConfig.end())
	{
		return it->second;
	}
	return nullptr;
}

NFServerConfig* NFCConfigModule::GetServerConfig(uint32_t serverId)
{
	auto it = mServerConfig.find(serverId);
	if (it != mServerConfig.end())
	{
		return it->second;
	}
	return nullptr;
}

std::vector<NFServerConfig*> NFCConfigModule::GetServerConfigFromServerType(uint32_t serverType)
{
	std::vector<NFServerConfig*> vec;
	for (auto it = mServerConfig.begin(); it != mServerConfig.end(); it++)
	{
		NFServerConfig* pConfig = it->second;
		if (pConfig)
		{
			if (pConfig->mServerType == serverType)
			{
				vec.push_back(pConfig);
			}
		}
	}
	return vec;
}