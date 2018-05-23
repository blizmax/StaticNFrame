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

#define REGISTER_ALONE_MODULE(pManager, classBaseName, className)  \
    assert((TIsDerived<classBaseName, NFIModule>::Result)); \
    assert((TIsDerived<className, classBaseName>::Result)); \
    pManager->RegisterAloneModule(#classBaseName, [] (NFIPluginManager* pMan) ->NFIModule* { return NF_NEW className(pMan);}); \

#define REGISTER_MODULE(pManager, classBaseName, className)  \
    assert((TIsDerived<classBaseName, NFIModule>::Result)); \
    assert((TIsDerived<className, classBaseName>::Result)); \
    NFIModule* pRegisterModule##className= new className(pManager); \
    pRegisterModule##className->strName = (#className); \
    REGISTER_ALONE_MODULE(pManager, classBaseName, className); \
    pManager->AddModule( #classBaseName, pRegisterModule##className );AddModule( #classBaseName, pRegisterModule##className );

#define UNREGISTER_MODULE(pManager, classBaseName, className) NFIModule* pUnRegisterModule##className =  \
    dynamic_cast<NFIModule*>( pManager->FindModule( typeid(classBaseName).name() )); \
	pManager->RemoveModule( #classBaseName ); RemoveModule( #classBaseName ); delete pUnRegisterModule##className;

#define REGISTER_STATIC_PLUGIN(pManager, className)  pManager->RegisteredStaticPlugin(#className, [] (NFIPluginManager* pMan) ->NFIPlugin* { return NF_NEW className(pMan);});

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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
				std::cout << pModule->strName << std::endl;
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
