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
	ϵͳ��Module����Ϊ����DLL������ΨһModule�����Լ���������������Module��
	DLL������ΨһModule��ͳһ��ϵͳ������
	���������ĵ���Module,
	���ǵ�Awake,Init,AfterInit�ɴ����߸���
	����ReadExecute��Execute������,�Լ���Դ���ͷ���ϵͳ����
*/

class NFCPluginManager
	: public NFIPluginManager,
	  public NFSingleton<NFCPluginManager>
{
public:
	NFCPluginManager();
	virtual ~NFCPluginManager();

	virtual bool InitSingleton();

	virtual bool Awake() override;

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool CheckConfig() override;

	virtual bool ReadyExecute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;

	virtual bool OnReloadPlugin() override;

	virtual bool ReleaseSingletion();

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

	virtual NFIModule* CreateAloneModule(const std::string& strModuleName) override;

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
	bool LoadPluginConfig();
	bool RegisterStaticPlugin();

	bool LoadStaticPlugin(const std::string& strPluginDLLName);
	bool UnLoadStaticPlugin(const std::string& strPluginDLLName);

	bool LoadPluginLibrary(const std::string& strPluginDLLName);
	bool UnLoadPluginLibrary(const std::string& strPluginDLLName);

	bool LoadKernelPlugin();

	virtual void BeginProfiler(const std::string& luaFunc);
	virtual uint64_t EndProfiler();//return this time cost time(us) ΢��
	virtual void ClearProfiler();
	virtual void PrintProfiler();
	virtual void SetOpenProfiler(bool b);
	virtual bool IsOpenProfiler();
private:
	const uint32_t mFrame = 30; //������֡�ʣ�һ��30֡
	const uint32_t mFrameTime = 1000 / mFrame; //һ֡����ʱ��
	uint32_t mCurFrameCount = 0;
private:
	int mnAppID;
	uint64_t mnInitTime;	//��������ʼʱ�䣬ms
	uint64_t mnNowTime;		//������֡ʱ�䣬ms
	std::string mstrConfigPath;
	std::string mstrConfigName;
	std::string mstrAppName;
	std::string mstrLogPath;
	std::string mstrLuaScriptPath;
	bool mIsDaemon;

	typedef std::map<std::string, NFCDynLib*> PluginLibMap;
	typedef std::map<std::string, bool> PluginNameMap;
	typedef std::map<std::string, NFIPlugin*> PluginInstanceMap;
	typedef std::list<NFIPlugin*> PluginInstanceList;
	typedef std::map<std::string, NFIModule*> ModuleInstanceMap;
	typedef std::map<std::string, CREATE_PLUGIN_FUNCTION> PluginFuncMap; //��̬����Plugin, ��ע�ᴴ������
	typedef std::map<int, int> ServerTypeToIdMap; //����AllServer����£�ServerType��ServerId�Ĺ�ϵ

	typedef std::map<std::string, CREATE_ALONE_MODULE> ModuleAloneFuncMap;  //�����뵥������Module�ĺ���
	typedef std::multimap<std::string, NFIModule*> ModuleAloneMultiMap;     //�����Լ�����������Module����

	typedef void (*DLL_START_PLUGIN_FUNC)(NFIPluginManager* pm);
	typedef void (*DLL_STOP_PLUGIN_FUNC)(NFIPluginManager* pm);

	PluginLibMap mPluginLibMap;
	PluginNameMap mPluginNameMap;
	PluginInstanceMap mPluginInstanceMap;
	PluginInstanceList mPluginInstanceList;
	ModuleInstanceMap mModuleInstanceMap;
	ServerTypeToIdMap mServerTypeIdMap; //����AllServer����£�ServerType��ServerId�Ĺ�ϵ
	PluginFuncMap mPluginFuncMap; ////��̬����Plugin, ��ע�ᴴ�������ٺ���

	ModuleAloneFuncMap mModuleAloneFuncMap;         //�����뵥������Module�ĺ���
	ModuleAloneMultiMap mModuleAloneMultiMap;       //�����Լ�����������Module����

	NFProfiler m_profilerMgr;
public:
	//ͨ��console���Ʒ�����
	virtual bool GetExitApp() const { return mExitApp; }
	virtual void SetExitApp(bool exitApp) { mExitApp = exitApp; }
private:
	//ͨ��console���Ʒ���������
	std::atomic_bool mExitApp;
};

#endif

