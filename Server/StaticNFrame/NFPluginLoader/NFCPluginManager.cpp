// -------------------------------------------------------------------------
//    @FileName         :    NFCPluginManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#include "NFCPluginManager.h"
#include "common/RapidXML/rapidxml.hpp"
#include "common/RapidXML/rapidxml_iterators.hpp"
#include "common/RapidXML/rapidxml_print.hpp"
#include "common/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFProfiler.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include "NFComm/NFCore/NFCpu.h"

#include <utility>
#include <thread>
#include <chrono>

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32.lib" )
#endif

NFCPluginManager::NFCPluginManager() : NFIPluginManager()
{
	mCurFrameCount = 0;
	mnAppID = 0;

	mstrConfigPath = "../Config";

	mstrConfigName = "Plugin.lua";

	mstrLogPath = "logs";

	mnInitTime = NFGetTime();
	mnNowTime = mnInitTime;
	mIsDaemon = false;

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

bool NFCPluginManager::IsDaemon() const
{
	return mIsDaemon;
}

void NFCPluginManager::SetDaemon()
{
	mIsDaemon = true;
}

bool NFCPluginManager::IsLoadAllServer() const
{
	return (GetAppName() == ALL_SERVER);
}

bool NFCPluginManager::Awake()
{
	mSystemInfo.Init();
#ifndef NF_DYNAMIC_PLUGIN
	RegisterStaticPlugin(); //注册静态引擎
	LoadKernelPlugin(); //NFKernelPlugin比较特殊，提前加载

	InitSingleton();

	NFLogInfo("NFPluginLoader Awake................");
	//加载引擎配置plugin.xml, 创建引擎，生成module
	LoadPluginConfig();
	for (PluginNameMap::iterator it = mPluginNameMap.begin(); it != mPluginNameMap.end(); ++it)
	{
		LoadStaticPlugin(it->first);
	}
#else
	LoadKernelPlugin(); //NFKernelPlugin比较特殊，提前加载
	InitSingleton();

	//加载引擎配置plugin.xml, 创建引擎，生成module
	LoadPluginConfig();
	for (PluginNameMap::iterator it = mPluginNameMap.begin(); it != mPluginNameMap.end(); ++it)
	{
		LoadPluginLibrary(it->first);
	}
#endif

	//调用所有module的Awake函数
	for (auto iter = mPluginInstanceList.begin(); iter != mPluginInstanceList.end(); ++iter)
	{
		(*iter)->Awake();
	}

	return true;
}

inline bool NFCPluginManager::Init()
{
	NFLogInfo("NFPluginLoader Init................");
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
		NF_SAFE_DELETE(it->second);
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
	mnNowTime = NFGetTime();
	uint64_t startTime = NFGetTime();
	uint64_t endTime = 0;
	mCurFrameCount++;

	//BeginProfiler("MainLoop");
	PluginInstanceMap::iterator it = mPluginInstanceMap.begin();
	for (; it != mPluginInstanceMap.end(); ++it)
	{
		//BeginProfiler(it->second->GetPluginName() + "--Loop");
		bool tembRet = it->second->Execute();
		bRet = bRet && tembRet;
		//EndProfiler();
	}
	//EndProfiler();

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
		if (cost >= 80 && cost <= 200)
		{
			NFLogWarning("frame timeout:{}", cost);
		}
		else if (cost > 200)
		{
			NFLogError("frame timeout:{}, something wrong", cost);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	mSystemInfo.CountSystemInfo();

	if (mCurFrameCount % 1000 == 0)
	{
		PrintProfiler();
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
	if (strPath.empty()) return;

	mstrConfigPath = strPath;
}

void NFCPluginManager::SetConfigName(const std::string& strFileName)
{
	if (strFileName.empty())
	{
		return;
	}

	mstrConfigName = strFileName;
}

void NFCPluginManager::SetLuaScriptPath(const std::string& luaScriptPath)
{
	if (luaScriptPath.empty()) return;
	mstrLuaScriptPath = luaScriptPath;
}

const std::string& NFCPluginManager::GetLuaScriptPath() const
{
	return mstrLuaScriptPath;
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

const std::string& NFCPluginManager::GetLogPath() const
{
	return mstrLogPath;
}

void NFCPluginManager::SetLogPath(const std::string& strName)
{
	if (strName.empty()) return;
	mstrLogPath = strName;
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
	NFLogInfo("NFPluginLoader AfterInit................");
	for (auto iter = mPluginInstanceList.begin(); iter != mPluginInstanceList.end(); ++iter)
	{
		(*iter)->AfterInit();
	}

	return true;
}

bool NFCPluginManager::CheckConfig()
{
	NFLogInfo("NFPluginLoader CheckConfig................");
	PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
	for (; itCheckInstance != mPluginInstanceMap.end(); ++itCheckInstance)
	{
		itCheckInstance->second->CheckConfig();
	}

	return true;
}

bool NFCPluginManager::ReadyExecute()
{
	NFLogInfo("NFPluginLoader ReadyExecute................");
	PluginInstanceMap::iterator itCheckInstance = mPluginInstanceMap.begin();
	for (; itCheckInstance != mPluginInstanceMap.end(); ++itCheckInstance)
	{
		itCheckInstance->second->ReadyExecute();
	}

	return true;
}

bool NFCPluginManager::BeforeShut()
{
	NFLogInfo("NFPluginLoader BeforeShut................");
	PluginInstanceMap::iterator itBeforeInstance = mPluginInstanceMap.begin();
	for (; itBeforeInstance != mPluginInstanceMap.end(); ++itBeforeInstance)
	{
		itBeforeInstance->second->BeforeShut();
	}

	return true;
}

bool NFCPluginManager::Shut()
{
	NFLogInfo("NFPluginLoader Shut................");
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); ++itInstance)
	{
		itInstance->second->Shut();
	}

	return true;
}

bool NFCPluginManager::Finalize()
{
	NFLogInfo("NFPluginLoader Finalize................");
	PluginInstanceMap::iterator itInstance = mPluginInstanceMap.begin();
	for (; itInstance != mPluginInstanceMap.end(); ++itInstance)
	{
		itInstance->second->Finalize();
	}

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
		std::cerr << " Load Static Plugin [" << strPluginName
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

uint32_t NFCPluginManager::GetFrame() const
{
	return mFrame;
}

uint32_t NFCPluginManager::GetFrameTime() const
{
	return mFrameTime;
}

uint64_t NFCPluginManager::GetInitTime() const
{
	return mnInitTime;
}

uint64_t NFCPluginManager::GetNowTime() const
{
	return mnNowTime;
}

bool NFCPluginManager::LoadPluginLibrary(const std::string& strPluginDLLName)
{
	PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
	if (it == mPluginLibMap.end())
	{
		NFCDynLib* pLib = new NFCDynLib(strPluginDLLName);
		bool bLoad = pLib->Load();

		if (bLoad)
		{
			mPluginLibMap.insert(PluginLibMap::value_type(strPluginDLLName, pLib));

			DLL_START_PLUGIN_FUNC pFunc = (DLL_START_PLUGIN_FUNC)pLib->GetSymbol("DllStartPlugin");
			if (!pFunc)
			{
				std::cout << "Find function DllStartPlugin Failed in [" << pLib->GetName() << "]" << std::endl;
				assert(0);
				return false;
			}

			pFunc(this);

			return true;
		}
		else
		{
#if NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE
			char* error = dlerror();
			if (error)
			{
				std::cout << stderr << " Load shared lib[" << pLib->GetName() << "] failed, ErrorNo. = [" << error << "]" << std::endl;
				std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
				assert(0);
				return false;
			}
#elif NF_PLATFORM == NF_PLATFORM_WIN
			std::cout << stderr << " Load DLL[" << pLib->GetName() << "] failed, ErrorNo. = [" << GetLastError() << "]" << std::endl;
			std::cout << "Load [" << pLib->GetName() << "] failed" << std::endl;
			assert(0);
			return false;
#endif // NF_PLATFORM
		}
	}

	return false;
}

bool NFCPluginManager::UnLoadPluginLibrary(const std::string& strPluginDLLName)
{
	PluginLibMap::iterator it = mPluginLibMap.find(strPluginDLLName);
	if (it != mPluginLibMap.end())
	{
		NFCDynLib* pLib = it->second;
		DLL_STOP_PLUGIN_FUNC pFunc = (DLL_STOP_PLUGIN_FUNC)pLib->GetSymbol("DllStopPlugin");

		if (pFunc)
		{
			pFunc(this);
		}

		pLib->UnLoad();

		delete pLib;
		pLib = NULL;
		mPluginLibMap.erase(it);

		return true;
	}

	return false;
}

const NFSystemInfo& NFCPluginManager::GetSystemInfo() const
{
	return mSystemInfo;
}

void NFCPluginManager::BeginProfiler(const std::string& funcName)
{
	m_profilerMgr.BeginProfiler(funcName);
}

uint64_t NFCPluginManager::EndProfiler()
{
	return m_profilerMgr.EndProfiler();
}

void NFCPluginManager::ClearProfiler()
{
	m_profilerMgr.ResetAllProfilerTimer();
}

void NFCPluginManager::PrintProfiler()
{
	std::string str = m_profilerMgr.OutputTopProfilerTimer();
	NFLogDebug("{}", str);
}
