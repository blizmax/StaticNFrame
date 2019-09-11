// -------------------------------------------------------------------------
//    @FileName         :    NFCTestActorModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#include "NFCTestModule.h"
#include "NFComm/NFCore/NFTime.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"

NFCTestModule::NFCTestModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCTestModule::~NFCTestModule()
{
}

bool NFCTestModule::Init()
{
	NFINetClientModule* pClientModule = FindModule<NFINetClientModule>();
	pClientModule->AddEventCallBack(NF_ST_REBOT, this, &NFCTestModule::OnProxySocketEvent);
	pClientModule->AddReceiveCallBack(NF_ST_REBOT, this, &NFCTestModule::OnHandleOtherMessage);
	for (int i = 0; i < 200; i++)
	{
		pClientModule->AddServer(NF_ST_REBOT, "45.249.246.175", 6003, 1);
	}
	
	return true;
}

void NFCTestModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
	if (pLuaScriptModule)
	{
		std::string strMsg(msg, nLen);
		pLuaScriptModule->RunNetRecvLuaFunc("LuaNFrame.DispatchRebotTcp", unLinkId, playerId, nMsgId, strMsg);
	}
	else
	{
		NFLogWarning(NF_LOG_SYSTEMLOG, 0, "msg:{} not handled!", nMsgId);
	}
}

void NFCTestModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
{
	if (nEvent == eMsgType_CONNECTED)
	{
		//NFLogDebug(NF_LOG_SYSTEMLOG, 0, "Rebot Player Connect Game Server Success!");
		NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
		if (pLuaScriptModule)
		{
			pLuaScriptModule->RunNetRecvLuaFunc("LuaNFrame.DispatchRebotEvent", unLinkId, 0, eMsgType_CONNECTED, "");
		}
	}
	else if (nEvent == eMsgType_DISCONNECTED)
	{
		//NFLogDebug(NF_LOG_SYSTEMLOG, 0, "Rebot Player DisConnect Game Server!");
		NFILuaScriptModule* pLuaScriptModule = FindModule<NFILuaScriptModule>();
		if (pLuaScriptModule)
		{
			pLuaScriptModule->RunNetRecvLuaFunc("LuaNFrame.DispatchRebotEvent", unLinkId, 0, eMsgType_DISCONNECTED, "");
		}
	}
}

void NFCTestModule::OnTimer(uint32_t nTimerID)
{
}

bool NFCTestModule::AfterInit()
{
	return true;
}


bool NFCTestModule::Execute()
{
	static bool flag = true;
	
	if (flag)
	{
		flag = false;
	}
	return true;
}

bool NFCTestModule::BeforeShut()
{
	return true;
}

bool NFCTestModule::Shut()
{
	return true;
}