// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptComponent.h
//    @Author           :    GaoYi
//    @Date             :    2018/08/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFITaskComponent.h"
#include "NFComm/NFPluginModule/NFILuaModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCLuaScriptComponent : public NFITaskComponent, public NFILuaModule, public NFIModule
{
public:
	NFCLuaScriptComponent(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFCLuaScriptComponent()
	{

	}


	void ProcessTaskStart(NFTask* pTask) override
	{

	}

	void ProcessTask(NFTask* pTask) override
	{
		if (pTask)
		{
			pTask->ThreadProcess();
		}
	}

	void ProcessTaskEnd(NFTask* pTask) override
	{

	}
public:
	virtual bool Init();
	virtual bool AfterInit();
	virtual bool ReadyExecute();

	virtual bool Execute();

	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Finalize();
public:
	bool Register();
	void LoadScript();
};