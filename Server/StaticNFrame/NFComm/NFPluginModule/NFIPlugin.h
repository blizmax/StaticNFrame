// -------------------------------------------------------------------------
//    @FileName         :    NFIPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIPlugin
//
// -------------------------------------------------------------------------

#ifndef NFI_PLUGIN_H
#define NFI_PLUGIN_H

#include <iostream>
#include <assert.h>
#include <map>
#include <vector>
#include "NFComm/NFCore/NFPlatform.h"
#include "NFIPluginManager.h"

#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFConfigMgr.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"

/*
	说明:动态加载时，当公共库比如NFPluginModule编译的是一个静态库时，这时候单件模式里的static变量，
	在每个plugin编译的DLL里都有一个单独的全景变量，所以需要下面的代码来把所有的变量重新设置一遍
*/
#define INIT_SINGLETON_PLUGINMANAGER(pManager)			\
NFIEventModule* pEventModule = (NFIEventModule*)pManager->FindModule(typeid(NFIEventModule).name());\
NFITimerModule* pTimerModule = (NFITimerModule*)pManager->FindModule(typeid(NFITimerModule).name());\
NFILogModule* pLogModule = (NFILogModule*)pManager->FindModule(typeid(NFILogModule).name());\
NFIConfigModule* pConfigModule = (NFIConfigModule*)pManager->FindModule(typeid(NFIConfigModule).name());\
NFConfigMgr::Instance()->Init(pConfigModule);\
NFEventMgr::Instance()->Init(pEventModule);\
NFTimerMgr::Instance()->Init(pTimerModule);\
NFLogMgr::Instance()->Init(pLogModule);\

#define RELEASE_SINGLETON_PLUGINMANAGER		\
NFConfigMgr::Instance()->UnInit();\
NFConfigMgr::Instance()->ReleaseInstance();\
NFTimerMgr::Instance()->UnInit();\
NFTimerMgr::Instance()->ReleaseInstance();\
NFEventMgr::Instance()->UnInit();\
NFEventMgr::Instance()->ReleaseInstance();\
NFLogMgr::Instance()->UnInit();\
NFLogMgr::Instance()->ReleaseInstance();\

#define REGISTER_ALONE_MODULE(pManager, classBaseName, className)  \
    assert((TIsDerived<classBaseName, NFIModule>::Result)); \
    assert((TIsDerived<className, classBaseName>::Result)); \
    pManager->RegisterAloneModule(#classBaseName, [] (NFIPluginManager* pMan) ->NFIModule* { return NF_NEW className(pMan);}); \

//为什么要在这里加上pManager->InitSingleton()呢， 主要是为了在动态加载的情况下，在每个DLL中，都初始化一次单件系统
#define REGISTER_MODULE(pManager, classBaseName, className)  \
    assert((TIsDerived<classBaseName, NFIModule>::Result)); \
    assert((TIsDerived<className, classBaseName>::Result)); \
    NFIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
	REGISTER_ALONE_MODULE(pManager, classBaseName, className); \
    pManager->AddModule( #classBaseName, pRegisterModule##className );AddModule( #classBaseName, pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, classBaseName, className) NFIModule* pUnRegisterModule##className =  \
    dynamic_cast<NFIModule*>( pManager->FindModule( typeid(classBaseName).name() )); \
	pManager->RemoveModule( #classBaseName );RemoveModule(#classBaseName); delete pUnRegisterModule##className;

#define REGISTER_STATIC_PLUGIN(pManager, className)  pManager->RegisteredStaticPlugin(#className, [] (NFIPluginManager* pMan) ->NFIPlugin* { return NF_NEW className(pMan);});

//#define CREATE_PLUGIN(pManager, className)  NFIPlugin* pCreatePlugin##className = new className(pManager); pManager->Registered( pCreatePlugin##className );
#define CREATE_PLUGIN(pManager, className)  NFIPlugin* pCreatePlugin##className = new className(pManager); INIT_SINGLETON_PLUGINMANAGER(pManager); pManager->Registered( pCreatePlugin##className );

#define DESTROY_PLUGIN(pManager, className) pManager->UnRegistered( pManager->FindPlugin((#className)) );

class NFIPluginManager;

class NFIPlugin : public NFIModule,
                  public std::map<std::string, NFIModule>
{
public:
	NFIPlugin()
	{
	}

	virtual int GetPluginVersion() = 0;

	virtual std::string GetPluginName() = 0;

	virtual void Install() = 0;

	virtual bool Awake() override;

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool CheckConfig() override;

	virtual bool ReadyExecute() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Finalize() override;

	virtual bool OnReloadPlugin() override;

	virtual void Uninstall() = 0;

	virtual void AddModule(const std::string& moduleName, NFIModule* pModule);

	virtual void RemoveModule(const std::string& moduleName);
protected:
	std::map<std::string, NFIModule*> m_mapModule;
	std::vector<NFIModule*> m_vecModule;
};

inline bool NFIPlugin::Awake()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->Awake();
			if (!bRet)
			{
				std::cout << pModule->strName << "Awake failed!"<< std::endl;
				assert(0);
			}
		}
	}
	return true;
}

inline bool NFIPlugin::Init()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];;
		if (pModule)
		{
			bool bRet = pModule->Init();
			if (!bRet)
			{
				std::cout << pModule->strName << " Init failed!" << std::endl;
				assert(0);
			}
		}
	}
	return true;
}

inline bool NFIPlugin::AfterInit()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->AfterInit();
			if (!bRet)
			{
				std::cout << pModule->strName << "AfterInit failed!" << std::endl;
				assert(0);
			}
		}
	}
	return true;
}

inline bool NFIPlugin::CheckConfig()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->CheckConfig();
			if (!bRet)
			{
				std::cout << pModule->strName << "CheckConfig failed!" << std::endl;
				assert(0);
			}
		}
	}

	return true;
}

inline bool NFIPlugin::ReadyExecute()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->ReadyExecute();
			if (!bRet)
			{
				std::cout << pModule->strName << "ReadyExecute failed!" << std::endl;
				assert(0);
			}
		}
	}

	return true;
}

inline bool NFIPlugin::Execute()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->Execute();
			if (!bRet)
			{
				std::cout << pModule->strName << "Execute failed!" << std::endl;
				assert(0);
			}
		}
	}

	return true;
}

inline bool NFIPlugin::BeforeShut()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->BeforeShut();
			if (!bRet)
			{
				std::cout << pModule->strName << "BeforeShut failed!" << std::endl;
				assert(0);
			}
		}
	}
	return true;
}

inline bool NFIPlugin::Shut()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->Shut();
			if (!bRet)
			{
				std::cout << pModule->strName << "Shut failed!" << std::endl;
				assert(0);
			}
		}
	}

	return true;
}

inline bool NFIPlugin::Finalize()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->Finalize();
			if (!bRet)
			{
				std::cout << pModule->strName << "Finalize failed!" << std::endl;
				assert(0);
			}
		}
	}

	return true;
}

inline bool NFIPlugin::OnReloadPlugin()
{
	for (size_t i = 0; i < m_vecModule.size(); i++)
	{
		NFIModule* pModule = m_vecModule[i];
		if (pModule)
		{
			bool bRet = pModule->OnReloadPlugin();
			if (!bRet)
			{
				std::cout << pModule->strName << "OnReleadPlugin failed!" << std::endl;
				assert(0);
			}
		}
	}

	return true;
}

inline void NFIPlugin::AddModule(const std::string& moduleName, NFIModule* pModule)
{
	if (m_mapModule.find(moduleName) != m_mapModule.end())
	{
		NF_ASSERT_MSG(false, moduleName + " Has Registerd! Exit!");
		exit(0);
	}

	m_mapModule.emplace(moduleName, pModule);
	m_vecModule.push_back(pModule);
}

inline void NFIPlugin::RemoveModule(const std::string& moduleName)
{
	auto it = m_mapModule.find(moduleName);
	if (it != m_mapModule.end())
	{
		for (auto vecIt = m_vecModule.begin(); vecIt != m_vecModule.end(); ++vecIt)
		{
			if (*vecIt == it->second)
			{
				m_vecModule.erase(vecIt);
				break;
			}
		}
		m_mapModule.erase(it);
	}
}
#endif

