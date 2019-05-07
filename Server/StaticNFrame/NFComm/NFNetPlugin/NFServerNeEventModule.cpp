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
	m_pLuaScriptModule = nullptr;
}

NFCServerNetEventModule::~NFCServerNetEventModule()
{

}

bool NFCServerNetEventModule::Awake()
{
	//可以允许Lua Module不存在
	m_pLuaScriptModule = dynamic_cast<NFILuaScriptModule*>(m_pPluginManager->FindModule(typeid(NFILuaScriptModule).name()));
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

void NFCServerNetEventModule::RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	if (m_pLuaScriptModule)
	{
		m_pLuaScriptModule->RunServerNetEventLuaFunc(luaFunc, nEvent, unLinkId, pServerData);
	}
}

void NFCServerNetEventModule::RunAccountNetEventLuaFunc(const std::string& luaFunc, uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData)
{
	if (m_pLuaScriptModule)
	{
		m_pLuaScriptModule->RunAccountNetEventLuaFunc(luaFunc, nEvent, unLinkId, pServerData);
	}
}