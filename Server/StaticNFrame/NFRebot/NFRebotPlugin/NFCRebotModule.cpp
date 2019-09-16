// -------------------------------------------------------------------------
//    @FileName         :    NFCRebotModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFRebotPlugin
//
// -------------------------------------------------------------------------

#include "NFCRebotModule.h"
#include "NFComm/NFCore/NFTime.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

NFCRebotModule::NFCRebotModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCRebotModule::~NFCRebotModule()
{
}

bool NFCRebotModule::Init()
{
	NFINetClientModule* pClientModule = FindModule<NFINetClientModule>();
	pClientModule->AddEventCallBack(NF_ST_REBOT, this, &NFCRebotModule::OnProxySocketEvent);
	pClientModule->AddReceiveCallBack(NF_ST_REBOT, this, &NFCRebotModule::OnHandleOtherMessage);

	NFServerConfig* pConfig = NFServerCommon::GetAppConfig(m_pPluginManager, NF_ST_REBOT);
	if (pConfig)
	{
		for (uint32_t i = 0; i < pConfig->mMaxConnectNum; i++)
		{
			pClientModule->AddServer(NF_ST_REBOT, pConfig->mServerIp, pConfig->mServerPort, 1);
		}
	}
	else
	{
		NFLogError(NF_LOG_SERVER_CONNECT_SERVER, 0, "I Can't get the Rebot Server config!");
		return false;
	}
	
	return true;
}

void NFCRebotModule::OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
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

void NFCRebotModule::OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId)
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

void NFCRebotModule::OnTimer(uint32_t nTimerID)
{
}

bool NFCRebotModule::AfterInit()
{
	return true;
}


bool NFCRebotModule::Execute()
{
	static bool flag = true;
	
	if (flag)
	{
		flag = false;
	}
	return true;
}

bool NFCRebotModule::BeforeShut()
{
	return true;
}

bool NFCRebotModule::Shut()
{
	return true;
}