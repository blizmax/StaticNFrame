// -------------------------------------------------------------------------
//    @FileName         :    NFCLogServerModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLogServerModule
//
// -------------------------------------------------------------------------

#include "NFLogServerModule.h"

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include <NFComm/NFPluginModule/NFConfigMgr.h>
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCLogServerModule::NFCLogServerModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCLogServerModule::~NFCLogServerModule()
{
}

bool NFCLogServerModule::Init()
{
	return true;
}

bool NFCLogServerModule::AfterInit()
{
	return true;
}

bool NFCLogServerModule::Execute()
{
	return true;
}

bool NFCLogServerModule::BeforeShut()
{
	return true;
}

bool NFCLogServerModule::Shut()
{
	return true;
}