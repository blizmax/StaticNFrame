// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <assert.h>
#include <set>
#include "NFCLuaScriptModule.h"
#include "NFLuaScriptPlugin.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINetServerModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIMongoModule.h"
#include "NFComm/NFPluginModule/NFIAsynMongoModule.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

void NFLuaTimer::OnTimer(uint32_t nTimerID)
{
	m_pLuaScriptModule->TryRunGlobalScriptFunc("CPPNFrame.RunTimer", mLuaFunc, nTimerID);
}

bool NFCLuaScriptModule::Init()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleOutputCP(CP_UTF8);
#elif NF_PLATFORM == NF_PLATFORM_LINUX
#endif
    Register();
	LoadScript();
    return true;
}

bool NFCLuaScriptModule::AfterInit()
{
    return true;
}

bool NFCLuaScriptModule::ReadyExecute()
{
	return true;
}

bool NFCLuaScriptModule::Shut()
{
    return true;
}

bool NFCLuaScriptModule::Finalize()
{
	for (auto iter = m_luaTimerMap.begin(); iter != m_luaTimerMap.end(); iter++)
	{
		NF_SAFE_DELETE(iter->second);
	}

	for (auto iter = m_luaTimerList.begin(); iter != m_luaTimerList.end(); iter++)
	{
		NF_SAFE_DELETE(*iter);
	}
	
	m_luaTimerList.clear();
	m_luaTimerMap.clear();
    return true;
}

bool NFCLuaScriptModule::Execute()
{
    //10秒钟reload一次
    return true;
}

bool NFCLuaScriptModule::BeforeShut()
{
    return true;
}

void NFCLuaScriptModule::LoadScript()
{
	TryAddPackagePath(m_pPluginManager->GetLuaScriptPath());
	TryLoadScriptFile("init.lua");

	TryRunGlobalScriptFunc("init_script_system", m_pPluginManager, this);
}

bool NFCLuaScriptModule::Register()
{
	LuaIntf::LuaBinding(l).beginClass<NFIPluginManager>("NFIPluginManager")
		.addFunction("GetAppName", &NFIPluginManager::GetAppName)
		.addFunction("GetAppID", (int (NFIPluginManager::*)() const)&NFIPluginManager::GetAppID)
		.addFunction("GetInitTime", &NFIPluginManager::GetInitTime)
		.addFunction("GetNowTime", &NFIPluginManager::GetNowTime)
		.addFunction("BeginProfiler", &NFIPluginManager::BeginProfiler)
		.addFunction("EndProfiler", &NFIPluginManager::EndProfiler)
		.addFunction("IsLoadAllServer", &NFIPluginManager::IsLoadAllServer)
		.addFunction("GetConfigPath", &NFIPluginManager::GetConfigPath)
		.addFunction("GetKernelModule", &NFIPluginManager::FindModule<NFIKernelModule>)
		.addFunction("GetLogModule", &NFIPluginManager::FindModule<NFILogModule>)
		.addFunction("GetLuaModule", &NFIPluginManager::FindModule<NFILuaScriptModule>)
		.addFunction("GetServerModule", &NFIPluginManager::FindModule<NFINetServerModule>)
		.addFunction("GetClientModule", &NFIPluginManager::FindModule<NFINetClientModule>)
		.addFunction("GetHttpClientModule", &NFIPluginManager::FindModule<NFIHttpClientModule>)
		.addFunction("GetHttpServerModule", &NFIPluginManager::FindModule<NFIHttpServerModule>)
		.addFunction("GetMongoModule", &NFIPluginManager::FindModule<NFIMongoModule>)
		.addFunction("GetAsynMongoModule", &NFIPluginManager::FindModule<NFIAsynMongoModule>)
		.addFunction("GetServerNetEventModule", &NFIPluginManager::FindModule<NFIServerNetEventModule>)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFIKernelModule>("NFIKernelModule")
		.addFunction("GetUUID", &NFIKernelModule::GetUUID)
		.addFunction("GetMD5", &NFIKernelModule::GetMD5)
		.addFunction("GetCRC32", &NFIKernelModule::GetCRC32)
		.addFunction("GetCRC16", &NFIKernelModule::GetCRC16)
		.addFunction("Base64Encode", &NFIKernelModule::Base64Encode)
		.addFunction("Base64Decode", &NFIKernelModule::Base64Decode)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFCLuaScriptModule>("NFILuaScriptModule")
		.addFunction("AddTimer", &NFCLuaScriptModule::AddTimer)
		.addFunction("StopTimer", &NFCLuaScriptModule::StopTimer)
		.addFunction("AddClocker", &NFCLuaScriptModule::AddClocker)
		.addFunction("StopClocker", &NFCLuaScriptModule::StopClocker)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFIHttpClientModule>("NFIHttpClientModule")
		.addFunction("HttpRequestGet", &NFIHttpClientModule::LuaHttpGet)
		.addFunction("HttpRequestPost", &NFIHttpClientModule::LuaHttpPost)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFHttpHandle>("NFHttpRequest")
		.addProperty("url", &NFHttpHandle::get_url)
		.addProperty("path", &NFHttpHandle::get_path)
		.addProperty("remoteHost", &NFHttpHandle::get_remoteHost)
		.addProperty("type", &NFHttpHandle::get_type)
		.addProperty("body", &NFHttpHandle::get_body)
	.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFIHttpServerModule>("NFIHttpServerModule")
		.addFunction("AddRequestHandler", &NFIHttpServerModule::LuaAddRequestHandler)
		.addFunction("InitServer", (int (NFIHttpServerModule::*)(NF_SERVER_TYPES serverType, uint32_t nPort))&NFIHttpServerModule::InitServer)
		.addFunction("ResponseMsg", (bool (NFIHttpServerModule::*)(NF_SERVER_TYPES serverType, const NFHttpHandle& req, const std::string& strMsg, NFWebStatus code,
			const std::string& reason))&NFIHttpServerModule::ResponseMsg)
		.addFunction("ResponseMsgByRequestId", (bool (NFIHttpServerModule::*)(NF_SERVER_TYPES serverType, uint64_t requestId, const std::string& strMsg, NFWebStatus code,
			const std::string& reason))&NFIHttpServerModule::ResponseMsg)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFINetServerModule>("NFINetServerModule")
		.addFunction("AddServer", &NFINetServerModule::AddServer)
		.addFunction("GetLinkIp", &NFINetServerModule::GetLinkIp)
		.addFunction("AddReceiveLuaCallBackByMsgId", &NFINetServerModule::AddReceiveLuaCallBackByMsgId)
		.addFunction("AddReceiveLuaCallBackToOthers", &NFINetServerModule::AddReceiveLuaCallBackToOthers)
		.addFunction("AddEventLuaCallBack", &NFINetServerModule::AddEventLuaCallBack)
		.addFunction("SendByServerID", (void (NFINetServerModule::*)(uint32_t usLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID))(&NFINetServerModule::SendByServerID))
		.addFunction("SendToAllServer", (void (NFINetServerModule::*)(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID))(&NFINetServerModule::SendToAllServer))
		.addFunction("SendToAllServerByServerType", (void (NFINetServerModule::*)(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID))(&NFINetServerModule::SendToAllServer))
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFINetClientModule>("NFINetClientModule")
		.addFunction("AddServer", &NFINetClientModule::AddServer)
		.addFunction("AddWebServer", &NFINetClientModule::AddWebServer)
		.addFunction("CloseServer", &NFINetClientModule::CloseServer)
		.addFunction("AddReceiveLuaCallBackByMsgId", &NFINetClientModule::AddReceiveLuaCallBackByMsgId)
		.addFunction("AddReceiveLuaCallBackToOthers", &NFINetClientModule::AddReceiveLuaCallBackToOthers)
		.addFunction("AddEventLuaCallBack", &NFINetClientModule::AddEventLuaCallBack)
		.addFunction("SendByServerID", (void (NFINetClientModule::*)(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID))(&NFINetClientModule::SendByServerID))
		.addFunction("SendToAllServer", (void (NFINetClientModule::*)(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID))(&NFINetClientModule::SendToAllServer))
		.addFunction("SendToAllServerByServerType", (void (NFINetClientModule::*)(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID))(&NFINetClientModule::SendToAllServer))
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFILogModule>("NFILogModule")
		.addFunction("SetDefaultLevel", &NFILogModule::SetDefaultLevel)
		.addFunction("SetDefaultFlush", &NFILogModule::SetDefaultFlush)
		.addFunction("LuaDebug", &NFILogModule::LuaDebug)
		.addFunction("LuaInfo", &NFILogModule::LuaInfo)
		.addFunction("LuaWarn", &NFILogModule::LuaWarn)
		.addFunction("LuaError", &NFILogModule::LuaError)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFIMongoModule>("NFIMongoModule")
		.addFunction("AddMongoServer", (bool (NFIMongoModule::*)(const int nServerID, const std::string& uri, const std::string& dbname))&NFIMongoModule::AddMongoServer)
		.addFunction("CreateCollection", &NFIMongoModule::CreateCollection)
		.addFunction("DropCollection", &NFIMongoModule::DropCollection)
		.addFunction("UpdateOneByKey", (bool (NFIMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key))&NFIMongoModule::UpdateOneByKey)
		.addFunction("UpdateOne", (bool (NFIMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& json))&NFIMongoModule::UpdateOne)
		.addFunction("FindOne", &NFIMongoModule::FindOne)
		.addFunction("FindMany", &NFIMongoModule::FindMany)
		.addFunction("FindAll", &NFIMongoModule::FindAllToJson)
		.addFunction("FindOneByKey", (std::string (NFIMongoModule::*)(const int nServerID, const std::string& collectionName, int64_t key))&NFIMongoModule::FindOneByKey)
		.addFunction("UpdateFieldByKey", (bool (NFIMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key))&NFIMongoModule::UpdateFieldByKey)
		.addFunction("FindFieldByKey", (std::string(NFIMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& fieldPath, int64_t key))&NFIMongoModule::FindFieldByKey)
		.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFIAsynMongoModule>("NFIAsynMongoModule")
		.addFunction("AddMongoServer", (bool (NFIAsynMongoModule::*)(const int nServerID, const std::string& uri, const std::string& dbname))&NFIAsynMongoModule::AddMongoServer)
		.addFunction("CreateCollection", &NFIAsynMongoModule::CreateCollection)
		.addFunction("UpdateOneByKey", (bool (NFIAsynMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key))&NFIAsynMongoModule::UpdateOneByKey)
		.addFunction("UpdateOne", (bool (NFIAsynMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& json))&NFIAsynMongoModule::UpdateOne)
		.addFunction("UpdateFieldByKey", (bool (NFIAsynMongoModule::*)(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key))&NFIAsynMongoModule::UpdateFieldByKey)
		.endClass();
	
	LuaIntf::LuaBinding(l).beginClass<NFIServerNetEventModule>("NFIServerNetEventModule")
	.addFunction("AddEventCallBack", &NFIServerNetEventModule::AddEventLuaCallBack)
	.addFunction("AddAccountEventCallBack", &NFIServerNetEventModule::AddAccountEventLuaCallBack)
	.endClass();

	LuaIntf::LuaBinding(l).beginClass<NFServerData>("NFServerData")
		.addProperty("UnlinkId", &NFServerData::GetUnlinkId)
		.addProperty("ServerId", &NFServerData::GetServerId)
		.endClass();

	return true;
}

void NFCLuaScriptModule::RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg)
{
	TryRunGlobalScriptFunc(luaFunc, unLinkId, valueId, nMsgId, strMsg);
}

void NFCLuaScriptModule::RunNetEventLuaFunc(const std::string& luaFunc, const eMsgType nEvent, const uint32_t unLinkId)
{
	TryRunGlobalScriptFunc(luaFunc, nEvent, unLinkId);
}

void NFCLuaScriptModule::RunHtttpClientLuaFunc(const std::string& luaFunc, const int state_code, const std::string& strRespData, const std::string& strUserData)
{
	TryRunGlobalScriptFunc("unilight.HttpClientRequestCallBack", luaFunc, state_code, strRespData, strUserData);
}

void NFCLuaScriptModule::RunHttpServerLuaFunc(const std::string& luaFunc, uint32_t serverType, const NFHttpHandle & req)
{
	TryRunGlobalScriptFunc("unilight.HttpServerRequestCallBack", luaFunc, serverType, req);
}

void NFCLuaScriptModule::StopTimer(uint32_t nTimerID)
{
	auto iter = m_luaTimerMap.find(nTimerID);
	if (iter != m_luaTimerMap.end())
	{
		NFLuaTimer* luaTimer = iter->second;
		luaTimer->KillTimer(nTimerID);

		m_luaTimerMap.erase(iter);
		luaTimer->Clear();
		m_luaTimerList.push_back(luaTimer);
	}
}

void NFCLuaScriptModule::StopClocker(uint32_t nTimerID)
{
	StopTimer(nTimerID);
}

uint32_t NFCLuaScriptModule::AddTimer(const std::string& luaFunc, uint64_t nInterVal, uint32_t nCallCount)
{
	NFLuaTimer* luaTimer = nullptr;
	if (m_luaTimerList.empty())
	{
		luaTimer = NF_NEW NFLuaTimer(this);
	}
	else
	{
		luaTimer = m_luaTimerList.front();
		m_luaTimerList.pop_front();
		luaTimer->m_pLuaScriptModule = this;
	}

	luaTimer->mLuaFunc = luaFunc;
	luaTimer->mInterVal = nInterVal;

	if (nCallCount == 0)
	{
		luaTimer->mCallCount = INFINITY_CALL;
	}
	else
	{
		luaTimer->mCallCount = nCallCount;
	}

	luaTimer->mCurCallCount = 0;
	luaTimer->mTimerId = ++m_luaTimerIndex;

	luaTimer->SetTimer(luaTimer->mTimerId, luaTimer->mInterVal, luaTimer->mCallCount);
	m_luaTimerMap.emplace(luaTimer->mTimerId, luaTimer);

	return luaTimer->mTimerId;
}

uint32_t NFCLuaScriptModule::AddClocker(const std::string& luaFunc, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount)
{
	NFLuaTimer* luaTimer = nullptr;
	if (m_luaTimerList.empty())
	{
		luaTimer = NF_NEW NFLuaTimer(this);
	}
	else
	{
		NFLuaTimer* luaTimer = m_luaTimerList.front();
		m_luaTimerList.pop_front();
		luaTimer->m_pLuaScriptModule = this;
	}

	luaTimer->mLuaFunc = luaFunc;
	if (nCallCount == 0)
	{
		luaTimer->mCallCount = INFINITY_CALL;
	}
	else
	{
		luaTimer->mCallCount = nCallCount;
	}
	luaTimer->mCurCallCount = 0;
	luaTimer->mTimerId = ++m_luaTimerIndex;

	luaTimer->SetFixTimer(luaTimer->mTimerId, nStartTime, nInterDays, luaTimer->mCallCount);
	m_luaTimerMap.emplace(luaTimer->mTimerId, luaTimer);
	return luaTimer->mTimerId;
}

void NFCLuaScriptModule::RunServerNetEventLuaFunc(const std::string& luaFunc, eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData)
{
	m_pPluginManager->BeginProfiler(luaFunc);
	TryRunGlobalScriptFunc(luaFunc, nEvent, unLinkId, pServerData);
	m_pPluginManager->EndProfiler();
}

void NFCLuaScriptModule::RunAccountNetEventLuaFunc(const std::string& luaFunc, uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData)
{
	m_pPluginManager->BeginProfiler(luaFunc);
	TryRunGlobalScriptFunc(luaFunc, nEvent, unLinkId, pServerData);
	m_pPluginManager->EndProfiler();
}