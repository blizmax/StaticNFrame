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
#include "NFComm/NFPluginModule/NFCObject.h"

NFCConfigModule::NFCConfigModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
	mGlobalConfig = NF_NEW NFCObject(0, m_pPluginManager);

	mGlobalConfig->SetNodeUInt32(DEFINE_LUA_STRING_LOG_LEVEL, NLL_TRACE_NORMAL);
	mGlobalConfig->SetNodeUInt32(DEFINE_LUA_STRING_LOG_FLUSH_LEVEL, NLL_TRACE_NORMAL);

	std::vector<int> dataColType;
	dataColType.push_back(NF_DT_INT);
	dataColType.push_back(NF_DT_BOOLEAN);
	dataColType.push_back(NF_DT_INT);
	dataColType.push_back(NF_DT_STRING);
	dataColType.push_back(NF_DT_ARRAY);
	mGlobalConfig->AddTable(0, DEFINE_LUA_STRING_LOG_INFO, dataColType, 0);

	//比较特殊，必须在这里加载配置，不然plugginmanager::Awake会出问题， 引擎配置没有数据
	LoadConfig();
	NFConfigMgr::Instance()->Init(this);
}

NFCConfigModule::~NFCConfigModule()
{
	NFConfigMgr::Instance()->UnInit();
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
	mGlobalConfig->SetNodeUInt32(DEFINE_LUA_STRING_LOG_LEVEL, 0);
	mGlobalConfig->SetNodeUInt32(DEFINE_LUA_STRING_LOG_FLUSH_LEVEL, 0);
	mGlobalConfig->ClearTable(DEFINE_LUA_STRING_LOG_INFO);

	uint32_t logLevel = 0;
	uint32_t logFlushLevel = 0;
	GetValue(DEFINE_LUA_STRING_LOG_LEVEL, logLevel);
	GetValue(DEFINE_LUA_STRING_LOG_FLUSH_LEVEL, logFlushLevel);

	mGlobalConfig->SetNodeUInt32(DEFINE_LUA_STRING_LOG_LEVEL, logLevel);
	mGlobalConfig->SetNodeUInt32(DEFINE_LUA_STRING_LOG_FLUSH_LEVEL, logFlushLevel);

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

		uint32_t logId = 0;
		bool display = false;
		uint32_t level = 0;
		std::string logName;

		GetLuaTableValue(logRef, "logid", logId);
		GetLuaTableValue(logRef, "display", display);
		GetLuaTableValue(logRef, "level", level);
		GetLuaTableValue(logRef, "logname", logName);

		int curRow = mGlobalConfig->AddTableRow(DEFINE_LUA_STRING_LOG_INFO);

		mGlobalConfig->SetTableUInt32(DEFINE_LUA_STRING_LOG_INFO, curRow, LOG_INFO_LOG_ID, logId);
		mGlobalConfig->SetTableBool(DEFINE_LUA_STRING_LOG_INFO, curRow, LOG_INFO_DISPLAY, display);
		mGlobalConfig->SetTableUInt32(DEFINE_LUA_STRING_LOG_INFO, curRow, LOG_INFO_LEVEL, level);
		mGlobalConfig->SetTableString(DEFINE_LUA_STRING_LOG_INFO, curRow, LOG_INFO_LOG_NAME, logName);

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
					mGlobalConfig->AddTableArrayItem(DEFINE_LUA_STRING_LOG_INFO, curRow, LOG_INFO_LOG_GUID, NFCData(NF_DT_INT, guid));
				}
			}
		}
		else
		{
			uint64_t guid = guidRef.toValue<uint64_t>();
			if (guid != 0)
			{
				mGlobalConfig->AddTableArrayItem(DEFINE_LUA_STRING_LOG_INFO, curRow, LOG_INFO_LOG_GUID, NFCData(NF_DT_INT, guid));
			}
		}
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

		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_NAME, pConfig->mServerName);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_GAME_NAME, pConfig->mGameName);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_IP, pConfig->mServerIp);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_PORT, pConfig->mServerPort);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_MAX_CONNECT_NUM, pConfig->mMaxConnectNum);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_WORK_THREAD_NUM, pConfig->mWorkThreadNum);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SECURITY, pConfig->mSecurity);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_WebSocket, pConfig->mWebSocket);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_HttpPort, pConfig->mHttpPort);

		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_MONGO_IP, pConfig->mMongoIp);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_MONGO_PORT, pConfig->mMongoPort);
		GetLuaTableValue(serverConfigRef, DEFINE_LUA_STRING_SERVER_MONGO_DBNAME, pConfig->mMongoDbName);

		GetLuaTableValue(serverConfigRef, "MysqlIp", pConfig->mMysqlIp);
		GetLuaTableValue(serverConfigRef, "MysqlPort", pConfig->mMysqlPort);
		GetLuaTableValue(serverConfigRef, "MysqlDbName", pConfig->mMysqlDbName);
		GetLuaTableValue(serverConfigRef, "MysqlUser", pConfig->mMysqlUser);
		GetLuaTableValue(serverConfigRef, "MysqlPassword", pConfig->mMysqlPassword);

		GetLuaTableValue(serverConfigRef, "NosqlIp", pConfig->mNosqlIp);
		GetLuaTableValue(serverConfigRef, "NosqlPort", pConfig->mNosqlPort);
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