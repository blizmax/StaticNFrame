// -------------------------------------------------------------------------
//    @FileName         :    NFCPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#ifndef NFC_PLUGIN_MANAGER_H
#define NFC_PLUGIN_MANAGER_H

#include <map>
#include <string>
#include <time.h>
#include <list>
#include <atomic>
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFPluginModule/NFCDynLib.h"
#include "NFComm/NFPluginModule/NFSystemInfo.h"
#include "NFComm/NFPluginModule/NFProfiler.h"

/*
	系统的Module，分为加载DLL创建的唯一Module，和自己独立创建出来的Module，
	DLL创建的唯一Module，统一由系统负责处理，
	创建出来的单独Module,
	它们的Awake,Init,AfterInit由创建者负责，
	其余ReadExecute，Execute。。。,以及资源的释放由系统负责
*/

class NFCPluginManager
	: public NFIPluginManager,
	  public NFSingleton<NFCPluginManager>
{
public:
	NFCPluginManager();
	virtual ~NFCPluginManager();

	virtual bool Begin() override;

	virtual bool End() override;

	virtual bool Awake() override;

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool CheckConfig() override;

	virtual bool ReadyExecute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;

	virtual bool OnReloadPlugin() override;

	//////////////////////////////////////////////////////////////////////////

	virtual void RegisteredStaticPlugin(const std::string& strPluginName, const CREATE_PLUGIN_FUNCTION& createFunc) override;

	virtual void Registered(NFIPlugin* pPlugin) override;

	virtual void UnRegistered(NFIPlugin* pPlugin) override;

	//////////////////////////////////////////////////////////////////////////

	virtual NFIPlugin* FindPlugin(const std::string& strPluginName) override;

	virtual void AddModule(const std::string& strModuleName, NFIModule* pModule) override;

	virtual void RemoveModule(const std::string& strModuleName) override;

	virtual NFIModule* FindModule(const std::string& strModuleName) override;

	virtual void RegisterAloneModule(const std::string& strModuleName, const CREATE_ALONE_MODULE& createFunc) override;

	virtual void UnRegisterAloneModule(const std::string& strModuleName) override;

	virtual NFIModule* CreateAloneModule(const std::string& strModuleName) override;

	virtual bool LoadPluginLibrary(const std::string& strPluginDLLName) override;
	virtual bool UnLoadPluginLibrary(const std::string& strPluginDLLName) override;

	virtual bool DynamicLoadPluginLibrary(const std::string& strPluginDLLName) override;

	virtual bool Execute() override;

	virtual int GetAppID() const override;

	virtual int GetAppID(int serverType) const override;

	virtual void SetAppID(const int nAppID) override;

	virtual void SetAppID(int serverType, int appID) override;

	virtual const std::string& GetConfigPath() const override;
	virtual void SetConfigPath(const std::string& strPath) override;

	virtual void SetConfigName(const std::string& strFileName) override;
	virtual const std::string& GetConfigName() const override;

	virtual const std::string& GetAppName() const override;

	virtual void SetAppName(const std::string& strAppName) override;

	virtual const std::string& GetLogPath() const override;

	virtual void SetLogPath(const std::string& strName) override;

	virtual void SetLuaScriptPath(const std::string& luaScriptPath) override;

	virtual const std::string& GetLuaScriptPath() const override;

	virtual bool IsLoadAllServer() const override;

	virtual uint32_t GetFrame() const;

	virtual uint32_t GetFrameTime() const;

	virtual uint64_t GetInitTime() const;

	virtual uint64_t GetNowTime() const;

	virtual bool IsDaemon() const;
	virtual void SetDaemon();
protected:

	bool LoadStaticPlugin(const std::string& strPluginDLLName);
	bool UnLoadStaticPlugin(const std::string& strPluginDLLName);

	bool LoadPluginConfig();
	bool RegisterStaticPlugin();

	bool LoadKernelPlugin();

	virtual void BeginProfiler(const std::string& luaFunc);
	virtual uint64_t EndProfiler();//return this time cost time(us) 微妙
	virtual void ClearProfiler();
	virtual void PrintProfiler();
	virtual void SetOpenProfiler(bool b);
	virtual bool IsOpenProfiler();
private:
	const uint32_t mFrame = 30; //服务器帧率，一秒30帧
	const uint32_t mFrameTime = 1000 / mFrame; //一帧多少时间
	uint32_t mCurFrameCount = 0;
private:
	int mnAppID;
	uint64_t mnInitTime;	//服务器开始时间，ms
	uint64_t mnNowTime;		//服务器帧时间，ms
	std::string mstrConfigPath;
	std::string mstrConfigName;
	std::string mstrAppName;
	std::string mstrLogPath;
	std::string mstrLuaScriptPath;
	bool mIsDaemon;

	typedef std::map<std::string, NFCDynLib*> PluginLibMap;
	typedef std::vector<std::string> PluginNameVec;
	typedef std::map<std::string, NFIPlugin*> PluginInstanceMap;
	typedef std::list<NFIPlugin*> PluginInstanceList;
	typedef std::map<std::string, NFIModule*> ModuleInstanceMap;
	typedef std::map<std::string, CREATE_PLUGIN_FUNCTION> PluginFuncMap; //静态加载Plugin, 先注册创建函数
	typedef std::map<int, int> ServerTypeToIdMap; //负责AllServer情况下，ServerType与ServerId的关系

	typedef std::map<std::string, CREATE_ALONE_MODULE> ModuleAloneFuncMap;  //名字与单独创建Module的函数
	typedef std::multimap<std::string, NFIModule*> ModuleAloneMultiMap;     //名字以及单独创建的Module集合

	typedef void (*DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm);
	typedef void (*DLL_STOP_PLUGIN_FUNC)(NFIPluginManager* pm);

	PluginLibMap mPluginLibMap;
	PluginNameVec mPluginNameVec;
	PluginInstanceMap mPluginInstanceMap;
	PluginInstanceList mPluginInstanceList;
	ModuleInstanceMap mModuleInstanceMap;
	ServerTypeToIdMap mServerTypeIdMap; //负责AllServer情况下，ServerType与ServerId的关系
	PluginFuncMap mPluginFuncMap; ////静态加载Plugin, 先注册创建和销毁函数

	ModuleAloneFuncMap mModuleAloneFuncMap;         //名字与单独创建Module的函数
	ModuleAloneMultiMap mModuleAloneMultiMap;       //名字以及单独创建的Module集合

	NFProfiler m_profilerMgr;
public:
	//通过console控制服务器
	virtual bool GetExitApp() const { return mExitApp; }
	virtual void SetExitApp(bool exitApp) { mExitApp = exitApp; }
private:
	//通过console控制服务器变量
	std::atomic_bool mExitApp;
};

#endif

