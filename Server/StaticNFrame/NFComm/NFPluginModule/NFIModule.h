// -------------------------------------------------------------------------
//    @FileName         :    NFIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGIC_MODULE_H
#define NFI_LOGIC_MODULE_H

#include <string>
#include "NFIPluginManager.h"

class NFIPluginManager;

class NFIModule
{
public:
	NFIModule()
	{
	}

	virtual ~NFIModule()
	{
	}

	template <typename T>
	T* FindModule()
	{
		if (m_pPluginManager)
		{
			return m_pPluginManager->FindModule<T>();
		}
		return nullptr;
	}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool AfterInit()
	{
		return true;
	}

	virtual bool CheckConfig()
	{
		return true;
	}

	virtual bool ReadyExecute()
	{
		return true;
	}

	virtual bool Execute()
	{
		return true;
	}

	virtual bool BeforeShut()
	{
		return true;
	}

	virtual bool Shut()
	{
		return true;
	}

	virtual bool Finalize()
	{
		return true;
	}

	virtual bool OnReloadPlugin()
	{
		return true;
	}

	virtual NFIPluginManager* GetPluginManager() const
	{
		return m_pPluginManager;
	}

	std::string strName;
protected:
	NFIPluginManager* m_pPluginManager = NULL;
};
#endif

