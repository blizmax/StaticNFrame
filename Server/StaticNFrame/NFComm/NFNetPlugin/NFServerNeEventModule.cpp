// -------------------------------------------------------------------------
//    @FileName         :    NFCServerNetEventModule.h
//    @Author           :    GaoYi
//    @Date             :    2018-12-21
//    @Module           :    NFCServerNetEventModule
//
// -------------------------------------------------------------------------

#include "NFServerNetEventModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

NFCServerNetEventModule::NFCServerNetEventModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCServerNetEventModule::~NFCServerNetEventModule()
{

}

bool NFCServerNetEventModule::Awake()
{
	return true;
}

bool NFCServerNetEventModule::Init()
{
	return true;
}

bool NFCServerNetEventModule::AfterInit()
{
	return true;
}

bool NFCServerNetEventModule::BeforeShut()
{
	return true;
}

bool NFCServerNetEventModule::Shut()
{
	mxCallBack.clear();
	mxAccountCallBack.clear();
	return true;
}

bool NFCServerNetEventModule::Finalize()
{
	return true;
}

bool NFCServerNetEventModule::Execute()
{
	return true;
}