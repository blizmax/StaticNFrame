// -------------------------------------------------------------------------
//    @FileName         :    NFCWebServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCWebServerModule
//
// -------------------------------------------------------------------------

#include "NFWebServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCWebServerModule::NFCWebServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCWebServerModule::~NFCWebServerModule()
{
}

bool NFCWebServerModule::Init()
{
	return true;
}

bool NFCWebServerModule::AfterInit()
{
	return true;
}

bool NFCWebServerModule::Execute()
{
	return true;
}

bool NFCWebServerModule::BeforeShut()
{
	return true;
}

bool NFCWebServerModule::Shut()
{
	return true;
}