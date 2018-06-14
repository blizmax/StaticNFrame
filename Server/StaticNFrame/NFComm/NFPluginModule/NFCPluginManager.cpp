// -------------------------------------------------------------------------
//    @FileName         :    NFCPluginManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#include "NFCPluginManager.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFCore/NFProfiler.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFCore/NFServerTimeMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"

#include <utility>
#include <thread>
#include <chrono>

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32.lib" )
#endif

NFCPluginManager::NFCPluginManager() : NFIPluginManager()
{
	mnAppID = 0;

	mstrConfigPath = "../";

#ifdef NF_DEBUG_MODE
	mstrConfigName = "Config/Plugin.lua";
#else
	mstrConfigName = "Config/Plugin.lua";
#endif

	//    注册AllServer
	for (int i = 1; i < NF_ST_MAX; i++)
	{
		mServerTypeIdMap[i] = i;
	}

	NFRandomSeed();
}

NFCPluginManager::~NFCPluginManager()
{
}

bool NFCPluginManager::IsLoadAllServer() const
{
	return (GetAppName() == ALL_SERVER);
}

bool NFCPluginManager::Awake()
{
	RegisterStaticPlugin(); //注册静态引擎
	LoadKernelPlugin(); //NFKernelPlugin比较特效，提前加载
	InitSingleton(); //初始化全局单件系统

	//加载引擎配置plugin.xml, 创建引擎，生成module
	LoadPluginConfig();
	for (PluginNameMap::iterator it = mPluginNameMap.begin(); it != mPluginNameMap.end(); ++it)
	{
		LoadStaticPlugin(it->first);
	}

	//调用所有module的Awake函数
	for (auto iter = mPluginInstanceList.begin(); iter != mPluginInstanceList.end(); ++iter)
	{
		(*iter)->Awake();
	}

	return true;
}

inline bool NFCPluginManager::Init()
{
	for (auto iter = mPluginInstanceList.begin(); iter != mPluginInstanceList.end(); ++iter)
	{
		(*iter)->Init();
	}

	return true;
}

bool NFCPluginManager::LoadPluginConfig()
{
	NFPluginConfig* pConfig = NFConfigMgr::Instance()->GetPluginConfig(mstrAppName);
	if (pConfig == nullptr)
	{
		NF_ASSERT_MSG(0, "There are no plugin:" + mstrAppName);
		return false;
	}

	for (size_t i = 0; i < pConfig->mVecPlugins.size(); i++)
	{
		std::string strPluginName = pConfig->mVecPlugins[i];
		mPluginNameMap.emplace(strPluginName, true);
	}

	return true;
}

void NFCPluginManager::RegisteredStaticPlugin(const std::string& strPluginName, const CREATE_PLUGIN_FUNCTION& createFunc)
{
	mPluginFuncMap.emplace(strPluginName, createFunc);
}

void NFCPluginManager::Registered(NFIPlugin* plugin)
{
	if (plugin == nullptr) return;

	std::string strPluginName = plugin->GetPluginName();
	if (!FindPlugin(strPluginName))
	{
		mPluginInstanceMap.insert(PluginInstanceMap::value_type(strPluginName, plugin));
		mPluginInstanceList.push_back(plugin);
		plugin->Install();
	}
	else
	{
		assert(0);
	}
}

void NFCPluginManager::UnRegistered(NFIPlugin* plugin)
{
	if (plugin == nullptr)
	{
		std::cerr << "UnRegistered, plugin == nullptr" << std::endl;
		return;
	}

	PluginInstanceMap::iterator it = mPluginInstanceMap.find(plugin->GetPluginName());
	if (it != mPluginInstanceMap.end())
	{
		if (it->second == nullptr)
		{
			mPluginInstanceMap.erase(it);
			return;
		}

		it->second->Uninstall();
		NFSafeDelete(it->second);
		it->second = nullptr;
		mPluginInstanceMap.erase(it);
	}
}

NFIPlugin* NFCPluginManager::FindPlugin(const std::string& strPluginName)
{
	PluginInstanceMap::iterator it = mPluginInstanceMap.find(strPluginName);
	if (it != mPluginInstanceMap.end())
	{
		return it->second;
	}

	return nullptr;
}

bool NFCPluginManager::Execute()
{
	bool bRet = true;
	uint64_t startTime = NFGetTime();
	uint64_t endTime = 0;

	NFServerTimeMgr::GetSingletonPtr()->Update(NFGetTime());

	BEGIN_PROFILE("Loop");

		PluginInstanceMap::iterator it = mPluginInstanceMap.begin();
		for (; it != mPluginInstanceMap.end(); ++it)
		{
			bool tembRet = it->second->Execute();
			bRet = bRet && tembRet;
		}

		for (auto iter = mModuleAloneMultiMap.begin(); iter != mModuleAloneMultiMap.end(); ++iter)
		{
			bool tembRet = iter->second->Execute();
			bRet = bRet && tembRet;
		}

		END_PROFILE();

	//采用固定帧率
	endTime = NFGetTime();
	uint32_t cost = static_cast<uint32_t>(endTime > startTime ? (endTime - startTime) : 0);
	uint32_t sleepTime = mFrameTime > cost ? (mFrameTime - cost) : 0;
	if (sleepTime > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
	}
	else
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return bRet;
}

inline int NFCPluginManager::GetAppID() const
{
	return mnAppID;
}

int NFCPluginManager::GetAppID(int serverType) const
{
	if (IsLoadAllServer())
	{
		auto it = mServerTypeIdMap.find(serverType);
		if (it != mServerTypeIdMap.end())
		{
			return it->second;
		}
	}
	return mnAppID;
}

inline void NFCPluginManager::SetAppID(const int nAppID)
{
	mnAppID = nAppID;
}

void NFCPluginManager::SetAppID(int serverType, int appID)
{
	mServerTypeIdMap[serverType] = appID;
}

inline const std::string& NFCPluginManager::GetConfigPath() const
{
	return mstrConfigPath;
}

inline void NFCPluginManager::SetConfigPath(const std::string& strPath)
{
	mstrConfigPath = strPath;
}

void NFCPluginManager::SetConfigName(const std::string& strFileName)
{
	if (strFileName.empty())
	{
		return;
	}

#ifdef NF_DEBUG_MODE
	mstrConfigName = "Config/" + strFileName;
#else
	mstrConfigName = "Config/" + strFileName;
#endif
}

const std::string& NFCPluginManager::GetConfigName() const
{
	return mstrConfigName;
}

const std::string& NFCPluginManager::GetAppName() const
{
	return mstrAppName;
}

void NFCPluginManager::SetAppName(const std::string& strAppName)
{
	if (!mstrAppName.empty())
	{
		return;
	}

	mstrAppName = strAppName;
}

const std::string& NFCPluginManager::GetLogConfigName() const
{
	return mstrLogConfigName;
}

void NFCPluginManager::SetLogConfigName(const std::string& strName)
{
	mstrLogConfigName = strName;
}

void NFCPluginManager::AddModule(const std::string& strModuleName, NFIModule* pModule)
{
	mModuleInstanceMap.insert(ModuleInstanceMap::value_type(strModuleName, pModule));
}

void NFCPluginManager::RemoveModule(const std::string& strModuleName)
{
	ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strModuleName);
	if (it != mModuleInstanceMap.end())
	{
		mModuleInstanceMap.erase(it);
	}
}

NFIModule* NFCPluginManager::FindModule(const std::string& strModuleName)
{
	std::string strSubModuleName = strModuleName;

#if NF_PLATFORM == NF_PLATFORM_WIN
	std::size_t position = strSubModuleName.find(' ');
	if (string::npos != position)
	{
		strSubModuleName = strSubModuleName.substr(position + 1, strSubModuleName.length());
	}
#else
	for (int i = 0; i < (int)strSubModuleName.length(); i++)
	{
		std::string s = strSubModuleName.substr(0, i + 1);
		int n = atof(s.c_str());
		if ((int)strSubModuleName.length() == i + 1 + n)
		{
			strSubModuleName = strSubModuleName.substr(i + 1, strSubModuleName.length());
			break;
		}
	}
#endif

	ModuleInstanceMap::iterator it = mModuleInstanceMap.find(strSubModuleName);
	if (it != mModuleInstanceMap.end())
	{
		return it->second;
	}

	return nullptr;
}

bool NFCPluginManager::AfterInit()
{
	for (auto iter = mPluginInstanceList.begin(); iter != mPluginInstanceList.end(); ++iter)
	{
		(*iter)->AfterInit();
	}

	return true;
}

bool NFCPluginManager::CheckConfig()
{
	PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
	for (; itCheckInstance != mPluginInstanceMap.end(); ++itCheckInstance)
	{
		itCheckInstance->second->CheckConfig();
	}

	for (auto it = mModuleAloneMultiMap.begin(); it != mModuleAloneMultiMap.end(); ++it)
	{
		it->second->CheckConfig();
	}
	return true;
}

bool NFCPluginManager::ReadyExecute()
{
	PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
	for (; itCheckInstance != mPluginInstanceMap.end(); ++itCheckInstance)
	{
		itCheckInstance->second->ReadyExecute();
	}

	for (auto it = mModuleAloneMultiMap.begin(); it != mModuleAloneMultiMap.end(); ++it)
	{
		it->second->ReadyExecute();
	}
	return true;
}

bool NFCPluginManager::BeforeShut()
{
	PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
	for (; itBeforeInstance != mPluginInstanceMap.end(); ++itBeforeInstance)
	{
		itBeforeInstance->second->BeforeShut();
	}

	for (auto it = mModuleAloneMultiMap.begin(); it != mModuleAloneMultiMap.end(); ++it)
	{
		it->second->BeforeShut();
	}

	return true;
}

bool NFCPluginManager::Shut()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); ++itInstance)
	{
		itInstance->second->Shut();
	}

	for (auto it = mModuleAloneMultiMap.begin(); it != mModuleAloneMultiMap.end(); ++it)
	{
		it->second->Shut();
	}

	return true;
}

bool NFCPluginManager::Finalize()
{
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); ++itInstance)
	{
		itInstance->second->Finalize();
	}

	for (auto it = mModuleAloneMultiMap.begin(); it != mModuleAloneMultiMap.end(); ++it)
	{
		it->second->Finalize();
	}

	//先析构掉独立的module
	for (auto it = mModuleAloneMultiMap.begin(); it != mModuleAloneMultiMap.end(); ++it)
	{
		NFSafeDelete(it->second);
		it->second = nullptr;
	}

	mModuleAloneMultiMap.clear();

	//std::function必须在module析构前，清理掉
	mModuleAloneFuncMap.clear();

	////////////////////////////////////////////////

	for (auto it = mPluginNameMap.begin(); it != mPluginNameMap.end(); ++it)
	{
		if (it->first != "NFKernelPlugin")
		{
			UnLoadStaticPlugin(it->first);
		}
	}

	UnLoadStaticPlugin("NFKernelPlugin");

	mPluginInstanceMap.clear();
	mPluginInstanceList.clear();
	mModuleInstanceMap.clear();
	mPluginNameMap.clear();

	//最后释放单件系统
	ReleaseSingletion();



	return true;
}

bool NFCPluginManager::LoadStaticPlugin(const std::string& strPluginName)
{
	auto it = mPluginFuncMap.find(strPluginName);
	if (it == mPluginFuncMap.end())
	{
		std::cout << stderr << " Load Static Plugin [" << strPluginName
			<< "] Failed, The Plugin Not Registered, Please Registed like this 'REGISTER_STATIC_PLUGIN(this, "
			<< strPluginName << ")' in the NFCLoadStaticPlugin.cpp" << std::endl;
		assert(0);
		return false;
	}

	if (FindPlugin(strPluginName)) return true;

	NFIPlugin* pPlugin = it->second(this);
	if (pPlugin)
	{
		Registered(pPlugin);
	}
	return true;
}

bool NFCPluginManager::UnLoadStaticPlugin(const std::string& strPluginDLLName)
{
	UnRegistered(FindPlugin(strPluginDLLName));
	return true;
}

void NFCPluginManager::RegisterAloneModule(const std::string& strModuleName, const CREATE_ALONE_MODULE& createFunc)
{
	mModuleAloneFuncMap.emplace(strModuleName, createFunc);
}

NFIModule* NFCPluginManager::CreateAloneModule(const std::string& strModuleName)
{
	std::string strSubModuleName = strModuleName;

#if NF_PLATFORM == NF_PLATFORM_WIN
	std::size_t position = strSubModuleName.find(' ');
	if (string::npos != position)
	{
		strSubModuleName = strSubModuleName.substr(position + 1, strSubModuleName.length());
	}
#else
	for (int i = 0; i < (int)strSubModuleName.length(); i++)
	{
		std::string s = strSubModuleName.substr(0, i + 1);
		int n = atof(s.c_str());
		if ((int)strSubModuleName.length() == i + 1 + n)
		{
			strSubModuleName = strSubModuleName.substr(i + 1, strSubModuleName.length());
			break;
		}
	}
#endif

	auto it = mModuleAloneFuncMap.find(strSubModuleName);
	if (it != mModuleAloneFuncMap.end())
	{
		NFIModule* pModule = it->second(this);
		if (pModule)
		{
			mModuleAloneMultiMap.emplace(strSubModuleName, pModule);
		}
		return pModule;
	}
	return nullptr;
}

