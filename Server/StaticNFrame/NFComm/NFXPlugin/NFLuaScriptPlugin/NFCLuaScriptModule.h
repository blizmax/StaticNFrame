// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LUA_SCRIPT_MODULE_H
#define NFC_LUA_SCRIPT_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIScriptModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

class NFCLuaScriptModule
    : public NFILuaScriptModule
{
public:
    NFCLuaScriptModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

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
protected:
    NFIKernelModule* m_pKernelModule;
    NFIEventModule* m_pEventModule;
protected:
    int64_t mnTime;
};

#endif
