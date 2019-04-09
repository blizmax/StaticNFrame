// -------------------------------------------------------------------------
//    @FileName         :    NFIPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFI_PLUGIN_MANAGER_H
#define NFI_PLUGIN_MANAGER_H

#include "NFIModule.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFSystemInfo.h"

#include <functional>
#include <assert.h>

#define ALL_SERVER ("AllServer")

class NFIPlugin;
class NFIPluginManager;

typedef std::function<NFIModule*(NFIPluginManager* pMan)> CREATE_ALONE_MODULE;

typedef std::function<NFIPlugin*(NFIPluginManager* pMan)> CREATE_PLUGIN_FUNCTION;

#define FIND_MODULE(classBaseName, className)  \
    assert((TIsDerived<classBaseName, NFIModule>::Result));

class NFIPluginManager : public NFIModule
{
public:
	NFIPluginManager()
	{
		m_pPluginManager = this;
	}

	virtual ~NFIPluginManager()
	{
	}

	template <typename T>
	T* FindModule()
	{
		NFIModule* pLogicModule = FindModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, NFIModule>::Result)
			{
				return nullptr;
			}
			//TODO OSX上dynamic_cast返回了NULL
#if NF_PLATFORM == NF_PLATFORM_APPLE
			T* pT = (T*)pLogicModule;
#else
			T* pT = dynamic_cast<T*>(pLogicModule);
#endif
			assert(nullptr != pT);

			return pT;
		}
		assert(nullptr);
		return nullptr;
	}

	template <typename T>
	T* CreateAloneModule()
	{
		NFIModule* pLogicModule = CreateAloneModule(typeid(T).name());
		if (pLogicModule)
		{
			if (!TIsDerived<T, NFIModule>::Result)
			{
				return NULL;
			}
			//TODO OSX上dynamic_cast返回了NULL
#if NF_PLATFORM == NF_PLATFORM_APPLE
			T* pT = (T*)pLogicModule;
#else
			T* pT = dynamic_cast<T*>(pLogicModule);
#endif
			assert(NULL != pT);

			return pT;
		}
		assert(NULL);
		return NULL;
	}

	virtual bool Begin() = 0;

	virtual bool End() = 0;

	virtual void RegisterAloneModule(const std::string& strModuleName, const CREATE_ALONE_MODULE& createFunc) = 0;   //

	virtual void UnRegisterAloneModule(const std::string& strModuleName) = 0;

	virtual NFIModule* CreateAloneModule(const std::string& strModuleName) = 0;

	virtual void RegisteredStaticPlugin(const std::string& strPluginName, const CREATE_PLUGIN_FUNCTION& createFunc) = 0;

	virtual void Registered(NFIPlugin* plugin) = 0;

	virtual void UnRegistered(NFIPlugin* plugin) = 0;

	virtual NFIPlugin* FindPlugin(const std::string& strPluginName) = 0;

	virtual void AddModule(const std::string& strModuleName, NFIModule* pModule) = 0;

	virtual void RemoveModule(const std::string& strModuleName) = 0;

	virtual NFIModule* FindModule(const std::string& strModuleName) = 0;

	virtual bool LoadPluginLibrary(const std::string& strPluginDLLName) = 0;

	virtual bool UnLoadPluginLibrary(const std::string& strPluginDLLName) = 0;

	virtual bool DynamicLoadPluginLibrary(const std::string& strPluginDLLName) = 0;

	virtual int GetAppID() const = 0;

	virtual int GetAppID(int serverType) const = 0;

	virtual void SetAppID(const int nAppID) = 0;

	virtual void SetAppID(int serverType, int appID) = 0;

	virtual const std::string& GetConfigPath() const = 0;
	virtual void SetConfigPath(const std::string& strPath) = 0;

	virtual void SetConfigName(const std::string& strFileName) = 0;
	virtual const std::string& GetConfigName() const = 0;

	virtual const std::string& GetAppName() const = 0;
	virtual void SetAppName(const std::string& strAppName) = 0;

	virtual const std::string& GetLogPath() const = 0;
	virtual void SetLogPath(const std::string& strName) = 0;

	virtual bool IsLoadAllServer() const = 0;

	virtual void SetLuaScriptPath(const std::string& luaScriptPath) = 0;

	virtual const std::string& GetLuaScriptPath() const = 0;

	virtual uint64_t GetInitTime() const = 0;

	virtual uint64_t GetNowTime() const = 0;

	virtual bool IsDaemon() const = 0;
	virtual void SetDaemon() = 0;

	virtual void SetOpenProfiler(bool b) = 0;
	virtual bool IsOpenProfiler() = 0;

	virtual void BeginProfiler(const std::string& luaFunc) = 0;
	//return BeginProfiler - EndProfiler 本次的时间差，单位微妙
	virtual uint64_t EndProfiler() = 0;

	//通过console控制服务器
	virtual bool GetExitApp() const = 0;
	virtual void SetExitApp(bool exitApp) = 0;
};

#endif

