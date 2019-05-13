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
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFPluginModule/NFDataNode.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFDateTime.hpp"

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
	LoadClassNode();
	LoadDBTable();
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

bool NFCConfigModule::LoadDBTable()
{
	NFLuaRef ref = GetGlobal("dbTableName");
	if (!ref.isValid())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "lua can't find dbTableName");
		assert(0);
	}

	if (!ref.isTable())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "dbTableName is not table in the lua");
		assert(0);
	}

	for (auto it = ref.begin(); it != ref.end(); ++it)
	{
		std::string dbTableName = it.key<std::string>();
		NFLuaRef dbTableNameRef = it.value();

		NFDBTable* pDBTable = NF_NEW NFDBTable();
		pDBTable->mTableName = dbTableName;

		for (auto colit = dbTableNameRef.begin(); colit != dbTableNameRef.end(); ++colit)
		{
			NFLuaRef colRef = colit.value();
			NFDBTableCol tableCol;
			GetLuaTableValue(colRef, "colName", tableCol.mColName);
			GetLuaTableValue(colRef, "colType", tableCol.mColType);
			GetLuaTableValue(colRef, "colTypeNum", tableCol.mColTypeNum);
			GetLuaTableValue(colRef, "colTypeLength", tableCol.mColTypeLength);
			GetLuaTableValue(colRef, "primaryKey", tableCol.mPrimaryKey);
			GetLuaTableValue(colRef, "index", tableCol.mIndex);
			GetLuaTableValue(colRef, "autoincrement", tableCol.mAutoIncrement);

			if (tableCol.mPrimaryKey)
			{
				pDBTable->mPrimaryKeyCol = tableCol.mColName;
			}
			pDBTable->mDBTableColMap.emplace(tableCol.mColName, tableCol);
		}

		for (auto classiter = mClassObjectConfig.begin(); classiter != mClassObjectConfig.end(); classiter++)
		{
			for (auto nodeiter = classiter->second->mClassNodeArray.begin(); nodeiter != classiter->second->mClassNodeArray.end(); nodeiter++)
			{
				for (size_t i = 0; i < nodeiter->mVecTableName.size(); i++)
				{
					if (dbTableName == nodeiter->mVecTableName[i])
					{
						if (pDBTable->mDBTableColMap.find(nodeiter->mNodeName) == pDBTable->mDBTableColMap.end())
						{
							NFDBTableCol tableCol;
							tableCol.mColName = nodeiter->mNodeName;
							tableCol.mPrimaryKey = false;
							tableCol.mIndex = false;
							tableCol.mAutoIncrement = false;

							tableCol.mColTypeNum = nodeiter->mNodeType;
							if (nodeiter->mNodeType == NF_DT_BOOLEAN)
							{
								tableCol.mColType = "real";
								
							}
							else if (nodeiter->mNodeType == NF_DT_INT)
							{
								tableCol.mColType = "int";
							}
							else if (nodeiter->mNodeType == NF_DT_DOUBLE)
							{
								tableCol.mColType = "double";
							}
							else if (nodeiter->mNodeType == NF_DT_STRING)
							{
								tableCol.mColType = "varchar";
								tableCol.mColTypeLength = 45;
							}
							else
							{
								tableCol.mColType = "varchar";
								tableCol.mColTypeLength = 512;
							}
							pDBTable->mDBTableColMap.emplace(tableCol.mColName, tableCol);
						}
					}
				}
			}
		}
	
		mDBTableColConfig.emplace(dbTableName, pDBTable);
	}
	return true;
}

bool NFCConfigModule::LoadClassNode()
{
	NFLuaRef ref = GetGlobal("className");
	if (!ref.isValid())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "lua can't find className");
		assert(0);
	}

	if (!ref.isTable())
	{
		NFLogError(NF_LOG_LOAD_CONFIG, 0, "className is not table in the lua");
		assert(0);
	}

	for (auto it = ref.begin(); it != ref.end(); ++it)
	{
		std::string className = it.key<std::string>();
		NFLuaRef classNameRef = it.value();

		NFClassObject *pClassObject = NF_NEW NFClassObject();
		pClassObject->mClassName = className;

		for (auto classit = classNameRef.begin(); classit != classNameRef.end(); ++classit)
		{
			NFLuaRef nodeRef = classit.value();

			NFClassNode classNode;
			std::string dbTable;

			GetLuaTableValue(nodeRef, "nodeName", classNode.mNodeName);
			GetLuaTableValue(nodeRef, "nodeType", classNode.mNodeType);
			GetLuaTableValue(nodeRef, "save", classNode.mSave);
			GetLuaTableValue(nodeRef, "public", classNode.mPublic);
			GetLuaTableValue(nodeRef, "private", classNode.mPrivate);
			GetLuaTableValue(nodeRef, "dbTable", classNode.mDBTable);
			GetLuaTableValue(nodeRef, "desc", classNode.mDesc);

			if (classNode.mSave)
			{
				NFBitValue<int8_t>::SetBitValue(classNode.mFeature, NFDataNode::PF_SAVE);
			}
			if (classNode.mPublic)
			{
				NFBitValue<int8_t>::SetBitValue(classNode.mFeature, NFDataNode::PF_PUBLIC);
			}
			if (classNode.mPrivate)
			{
				NFBitValue<int8_t>::SetBitValue(classNode.mFeature, NFDataNode::PF_PRIVATE);
			}
			
			NFStringUtility::Split(classNode.mVecTableName, classNode.mDBTable, ",");
			pClassObject->mClassNodeMap.emplace(classNode.mNodeName, classNode);
			pClassObject->mClassNodeArray.push_back(classNode);
		}
		mClassObjectConfig.emplace(pClassObject->mClassName, pClassObject);
	}
	return true;
}

void NFCConfigModule::CreateSqlFile()
{
	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "create db sql file...........");

	std::stringstream sqlFile;
	sqlFile << "/*" << std::endl;
	sqlFile << "---------------------mysql-----------------------" << std::endl;
	sqlFile << "Date:" << NFDateTime::Now().GetDbTimeString() << std::endl;
	sqlFile << "*/" << std::endl;
	sqlFile << std::endl;

	for (auto iter = mDBTableColConfig.begin(); iter != mDBTableColConfig.end(); iter++)
	{
		NFDBTable* pDBTable = iter->second;
		sqlFile << "/*-- ----------------------------" << std::endl;
		sqlFile << "-- Table structure for " << pDBTable->mTableName << std::endl;
		sqlFile << "-- ----------------------------*/" << std::endl;

		NFDBTableCol* pPrimaryKey = nullptr;
		if (pDBTable->mDBTableColMap.find(pDBTable->mPrimaryKeyCol) != pDBTable->mDBTableColMap.end())
		{
			pPrimaryKey = &pDBTable->mDBTableColMap[pDBTable->mPrimaryKeyCol];
		}

		if (pPrimaryKey == nullptr)
		{
			NFLogError(NF_LOG_SYSTEMLOG, 0, "db table:{} not find the primary col:{}", pDBTable->mTableName, pDBTable->mPrimaryKeyCol);
			continue;
		}

		sqlFile << "CREATE TABLE IF NOT EXISTS `" << pDBTable->mTableName << "` (\n";
		sqlFile << "`" << pPrimaryKey->mColName << "`";
		if (pPrimaryKey->mColType != "varchar")
		{
			sqlFile << " " << pPrimaryKey->mColType << " not null";
		}
		else
		{
			sqlFile << " " << pPrimaryKey->mColType << "(" << pPrimaryKey->mColTypeLength << ") CHARACTER SET latin1 not null";
		}

		if (pPrimaryKey->mAutoIncrement)
		{
			sqlFile << " AUTO_INCREMENT,\n";
		}

		sqlFile << " PRIMARY KEY(`" << pPrimaryKey->mColName << "`)\n";

		sqlFile << "\n) ENGINE=InnoDB DEFAULT CHARSET=utf8;" << std::endl;
		for (auto colIter = pDBTable->mDBTableColMap.begin(); colIter != pDBTable->mDBTableColMap.end(); colIter++)
		{
			NFDBTableCol& tableCol = colIter->second;

			if (tableCol.mColName == pPrimaryKey->mColName)
			{
				continue;
			}

			sqlFile << "ALTER TABLE `" << pDBTable->mTableName << "` ADD COLUMN " << tableCol.mColName;
			
			if (tableCol.mColType != "varchar")
			{
				if (tableCol.mAutoIncrement == false)
				{
					sqlFile << " " << tableCol.mColType << " default 0";
				}
				else
				{
					sqlFile << " " << tableCol.mColType << " not null";
				}
			}
			else
			{
				if (tableCol.mAutoIncrement == false)
				{
					sqlFile << " " << tableCol.mColType << "(" << tableCol.mColTypeLength << ") CHARACTER SET latin1 DEFAULT ''";
				}
				else
				{
					sqlFile << " " << tableCol.mColType << "(" << tableCol.mColTypeLength << ") CHARACTER SET latin1 not null";
				}
				
			}

			sqlFile << ";";
			sqlFile << std::endl;
		}
	}

	NFFileUtility::WriteFile("../../../Sql/mysql_create_table.sql", sqlFile.str().c_str(), sqlFile.str().length());
}

void NFCConfigModule::ProductFile()
{
	CreateHeaderFile();
	CreateSqlFile();
}

void NFCConfigModule::CreateHeaderFile()
{
	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "create class header file...........");
	std::stringstream classHeaderFile;
	classHeaderFile << "#pragma once" << std::endl;
	classHeaderFile << std::endl;

	std::stringstream classNodeFile;
	classNodeFile << "#pragma once" << std::endl;
	classNodeFile << std::endl;
	
	classHeaderFile << "#define NF_NODE_STRING_CLASS_NAME \t\t\t\t\t\"ClassName\"" << std::endl;

	for (auto iter = mClassObjectConfig.begin(); iter != mClassObjectConfig.end(); iter++)
	{
		std::string className = iter->first;
		NFStringUtility::ToUpper(className);
		classHeaderFile << "#define NF_NODE_STRING_CLASS_NAME_" << className << "\t\t\t\t\t\"" << iter->first << "\"" << std::endl;

		classNodeFile << "////////////////////////////// " << iter->first << " node name /////////////////////////////" << std::endl;
		
		for (auto nodeIter = iter->second->mClassNodeArray.begin(); nodeIter != iter->second->mClassNodeArray.end(); nodeIter++)
		{
			std::string nodeName = nodeIter->mNodeName;
			NFStringUtility::ToUpper(nodeName);

			std::stringstream classNodeLine;
			if (nodeIter->mNodeType == NF_DT_BOOLEAN)
			{
				classNodeLine << "#define NF_" << className << "_NODE_BOOL_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_INT)
			{
				classNodeLine << "#define NF_" << className << "_NODE_INT_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_DOUBLE)
			{
				classNodeLine << "#define NF_" << className << "_NODE_DOUBLE_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_STRING)
			{
				classNodeLine << "#define NF_" << className << "_NODE_STRING_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_ARRAY)
			{
				classNodeLine << "#define NF_" << className << "_NODE_ARRAY_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_LIST)
			{
				classNodeLine << "#define NF_" << className << "_NODE_LIST_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_MAPSTRING)
			{
				classNodeLine << "#define NF_" << className << "_NODE_MAPSTRING_" << nodeName;
			}
			else if (nodeIter->mNodeType == NF_DT_MAPINT)
			{
				classNodeLine << "#define NF_" << className << "_NODE_MAPINT_" << nodeName;
			}

			while (classNodeLine.str().length() <= 60)
			{
				classNodeLine << " ";
			}

			classNodeLine << "\"" << nodeIter->mNodeName << "\"";

			while (classNodeLine.str().length() <= 100)
			{
				classNodeLine << " ";
			}

			classNodeLine << "//" << nodeIter->mDesc << std::endl;

			classNodeFile << classNodeLine.str();
		}
	}

	NFFileUtility::WriteFile("../../../Server/NFMessageDefine/NFNodeClassName.h", classHeaderFile.str().c_str(), classHeaderFile.str().length());
	NFFileUtility::WriteFile("../../../Server/NFMessageDefine/NFNodeClass.h", classNodeFile.str().c_str(), classNodeFile.str().length());
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

NFClassObject* NFCConfigModule::GetClassObject(const std::string& className) const
{
	auto iter = mClassObjectConfig.find(className);
	if (iter != mClassObjectConfig.end())
	{
		return iter->second;
	}
	return nullptr;
}