﻿// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLuaScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <assert.h>
#include <set>
#include "NFCLuaScriptModule.h"

#include "NFComm/NFCore/NFMMOMD5.h"
#include "NFComm/NFCore/NFCRC32.h"
#include "NFComm/NFCore/NFCRC16.h"
#include "NFComm/NFCore/NFBase64.h"

enum EnumLuaScriptTimer
{
	EnumLuaScriptTimer_ServerLoop = 0,
};

void NFLuaTimer::OnTimer(uint32_t nTimerID)
{
	if (mGlobalLuaFunc == "LuaNFrame.DispatchTimerLoop")
	{
		m_pLuaScriptModule->BeginProfiler("LuaNFrame.DispatchTimerLoop--"+ mTmpStr);
		m_pLuaScriptModule->TryRunGlobalScriptFunc("LuaNFrame.DispatchTimerLoop", mTmpStr);
		m_pLuaScriptModule->EndProfiler();
	}
	else if (mGlobalLuaFunc == "LuaNFrame.DispatchTimerOnce")
	{
		m_pLuaScriptModule->BeginProfiler("LuaNFrame.DispatchTimerOnce--"+mLuaFunc);
		m_pLuaScriptModule->TryRunGlobalScriptFunc("LuaNFrame.DispatchTimerOnce", mLuaFunc, mDataStr);
		m_pLuaScriptModule->EndProfiler();
	}
	else if (mGlobalLuaFunc == "LuaNFrame.DispatchWorker")
	{
		m_pLuaScriptModule->BeginProfiler("LuaNFrame.DispatchWorker--"+ mLuaFunc);
		m_pLuaScriptModule->TryRunGlobalScriptFunc("LuaNFrame.DispatchWorker", mLuaFunc, mDataStr);
		m_pLuaScriptModule->EndProfiler();
	}
	else
	{
		m_pLuaScriptModule->BeginProfiler("LuaNFrame.DispatchTimer--"+ mLuaFunc);
		m_pLuaScriptModule->TryRunGlobalScriptFunc("LuaNFrame.DispatchTimer", mLuaFunc, mDataStr);
		m_pLuaScriptModule->EndProfiler();
	}
}

bool NFCLuaScriptModule::Init()
{
//#if NF_PLATFORM == NF_PLATFORM_WIN
//	SetConsoleOutputCP(CP_UTF8);
//#endif
	m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();
	m_pLogModule = m_pPluginManager->FindModule<NFILogModule>();

	m_pPluginManager->FindModule<NFIServerNetEventModule>()->AddAccountEventCallBack(NF_ST_GAME, this, &NFCLuaScriptModule::OnAccountEventCallBack);

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
	TryRunGlobalScriptFunc("init_script_system", this);
	//TryLoadScriptFile("LoadHelper.lua");

	//TryRunGlobalScriptFunc("LuaNFrame.TimerInit", 0, "logtimer");
	//TryRunGlobalScriptFunc("LuaNFrame.TimerInit", 0, "pokertimer");
	TryRunGlobalScriptFunc("LuaNFrame.TimerInit", 0, "gametimer");
	//TryRunGlobalScriptFunc("LuaNFrame.TimerInit", 0, "utilstimer");

	//ProcessLoop("logtimer");
	ProcessLoop("gametimer");
	ProcessLoop("utilstimer");
}

const std::string& NFCLuaScriptModule::GetAppName() const
{
	return m_pPluginManager->GetAppName();
}

int NFCLuaScriptModule::GetAppID() const
{
	return m_pPluginManager->GetAppID();
}

uint64_t NFCLuaScriptModule::GetInitTime() const
{
	return m_pPluginManager->GetInitTime();
}

uint64_t NFCLuaScriptModule::GetNowTime() const
{
	return m_pPluginManager->GetNowTime();
}

std::string NFCLuaScriptModule::GetMD5(const std::string& str)
{
	return NFMMOMD5(str).toStr();
}

uint32_t NFCLuaScriptModule::GetCRC32(const std::string& s)
{
	return NFCRC32::Sum(s);
}

uint16_t NFCLuaScriptModule::GetCRC16(const std::string& s)
{
	return NFCRC16::Sum(s);
}

std::string NFCLuaScriptModule::Base64Encode(const std::string& s)
{
	return NFBase64::Encode(s);
}

std::string NFCLuaScriptModule::Base64Decode(const std::string& s)
{
	return NFBase64::Decode(s);
}

void NFCLuaScriptModule::SendMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	if (m_pNetServerModule)
	{
		if (usLinkId != 0)
		{
			m_pNetServerModule->SendByServerID(usLinkId, nMsgID, strData, nPlayerID);
		}
		else
		{
			if (nPlayerID != 0)
			{
				auto pPlayerInfo = GetPlayerInfo(nPlayerID);
				if (pPlayerInfo)
				{
					m_pNetServerModule->SendByServerID(pPlayerInfo->GetProxyUnlinkId(), nMsgID, strData, nPlayerID);
				}
			}
		}
	}
}

void NFCLuaScriptModule::SendMsgToManyPlayer(const std::vector<uint64_t>& nVecPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	if (m_pNetServerModule)
	{
		for (size_t i = 0; i < nVecPlayerID.size(); i++)
		{
			uint64_t nPlayerID = nVecPlayerID[i];
			auto pPlayerInfo = GetPlayerInfo(nPlayerID);
			if (pPlayerInfo)
			{
				m_pNetServerModule->SendByServerID(pPlayerInfo->GetProxyUnlinkId(), nMsgID, strData, nPlayerID);
			}
		}
	}
}

void NFCLuaScriptModule::SendMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	if (m_pNetServerModule)
	{
		auto pPlayerInfo = mPlayerProxyInfoMap.First();
		while (pPlayerInfo)
		{
			m_pNetServerModule->SendByServerID(pPlayerInfo->GetProxyUnlinkId(), nMsgID, strData, pPlayerInfo->GetPlayerId());
			pPlayerInfo = mPlayerProxyInfoMap.Next();
		}
	}
}

void NFCLuaScriptModule::SendMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	if (m_pNetClientModule)
	{
		if (usLinkId != 0)
		{
			m_pNetClientModule->SendByServerID(usLinkId, nMsgID, strData, nPlayerID);
		}
		else
		{
			if (nPlayerID != 0)
			{
				auto pPlayerInfo = GetPlayerInfo(nPlayerID);
				if (pPlayerInfo)
				{
					m_pNetClientModule->SendByServerID(pPlayerInfo->GetWorldUnlinkId(), nMsgID, strData, nPlayerID);
				}
			}
		}
	}
}

void NFCLuaScriptModule::SendMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	if (m_pNetClientModule)
	{
		if (usLinkId != 0)
		{
			m_pNetClientModule->SendByServerID(usLinkId, nMsgID, strData, nPlayerID);
		}
	}
}

void NFCLuaScriptModule::SetDefaultLevel(uint32_t log_level)
{
	if (m_pLogModule)
	{
		m_pLogModule->SetDefaultLevel((NF_LOG_LEVEL)log_level);
	}
}

void NFCLuaScriptModule::SetDefaultFlush(uint32_t log_level)
{
	if (m_pLogModule)
	{
		m_pLogModule->SetDefaultFlush((NF_LOG_LEVEL)log_level);
	}
}

void NFCLuaScriptModule::LuaDebug(uint32_t logId, uint64_t guid, const std::string& str)
{
	if (m_pLogModule)
	{
		m_pLogModule->LuaDebug(logId, guid, str);
	}
}

void NFCLuaScriptModule::LuaInfo(uint32_t logId, uint64_t guid, const std::string& str)
{
	if (m_pLogModule)
	{
		m_pLogModule->LuaInfo(logId, guid, str);
	}
}

void NFCLuaScriptModule::LuaWarn(uint32_t logId, uint64_t guid, const std::string& str)
{
	if (m_pLogModule)
	{
		m_pLogModule->LuaWarn(logId, guid, str);
	}
}

void NFCLuaScriptModule::LuaError(uint32_t logId, uint64_t guid, const std::string& str)
{
	if (m_pLogModule)
	{
		m_pLogModule->LuaError(logId, guid, str);
	}
}

void NFCLuaScriptModule::ProcessLoop(const std::string& dataStr)
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

	luaTimer->mGlobalLuaFunc = "LuaNFrame.DispatchTimerLoop";
	luaTimer->mInterVal = 1000;
	luaTimer->mTmpStr = dataStr;
	luaTimer->mCallCount = INFINITY_CALL;

	luaTimer->mCurCallCount = 0;
	luaTimer->mTimerId = ++m_luaTimerIndex;

	luaTimer->SetTimer(luaTimer->mTimerId, luaTimer->mInterVal, luaTimer->mCallCount);
	m_luaTimerMap.emplace(luaTimer->mTimerId, luaTimer);
}


void NFCLuaScriptModule::ProcessWork(const std::string& luaFunc, const NFLuaRef& dataStr)
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

	luaTimer->mGlobalLuaFunc = "LuaNFrame.DispatchWorker";
	luaTimer->mInterVal = 1;
	luaTimer->mLuaFunc = luaFunc;
	luaTimer->mDataStr = dataStr;
	luaTimer->mCallCount = 1;

	luaTimer->mCurCallCount = 0;
	luaTimer->mTimerId = ++m_luaTimerIndex;

	luaTimer->SetTimer(luaTimer->mTimerId, luaTimer->mInterVal, luaTimer->mCallCount);
	m_luaTimerMap.emplace(luaTimer->mTimerId, luaTimer);
}

void NFCLuaScriptModule::ProcessTimer(uint32_t timeSec, const std::string& luaFunc, const NFLuaRef& dataStr)
{
	if (timeSec <= 0)
	{
		timeSec = 1;
	}

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

	luaTimer->mGlobalLuaFunc = "LuaNFrame.DispatchTimerOnce";
	luaTimer->mLuaFunc = luaFunc;
	luaTimer->mInterVal = timeSec;
	luaTimer->mDataStr = dataStr;
	luaTimer->mCallCount = 1;


	luaTimer->mCurCallCount = 0;
	luaTimer->mTimerId = ++m_luaTimerIndex;

	luaTimer->SetTimer(luaTimer->mTimerId, luaTimer->mInterVal, luaTimer->mCallCount);
	m_luaTimerMap.emplace(luaTimer->mTimerId, luaTimer);
}

void NFCLuaScriptModule::ProcessLoopTimer(uint32_t timeSec, const std::string& luaFunc, const NFLuaRef& dataStr)
{
	ProcessTimer(timeSec, luaFunc, dataStr);
}

void NFCLuaScriptModule::BeginProfiler(const std::string& funcName)
{
	m_pPluginManager->BeginProfiler(funcName);
}

uint64_t NFCLuaScriptModule::EndProfiler()
{
	return m_pPluginManager->EndProfiler();
}

bool NFCLuaScriptModule::Register()
{
	LuaIntf::LuaBinding(l).beginClass<NFCLuaScriptModule>("NFCLuaScriptModule")
		.addFunction("GetAppName", &NFCLuaScriptModule::GetAppName)
		.addFunction("GetAppID", &NFCLuaScriptModule::GetAppID)
		.addFunction("GetMD5", &NFCLuaScriptModule::GetMD5)
		.addFunction("GetCRC32", &NFCLuaScriptModule::GetCRC32)
		.addFunction("GetCRC16", &NFCLuaScriptModule::GetCRC16)
		.addFunction("Base64Encode", &NFCLuaScriptModule::Base64Encode)
		.addFunction("Base64Decode", &NFCLuaScriptModule::Base64Decode)
		.addFunction("AddTimer", &NFCLuaScriptModule::AddTimer)
		.addFunction("StopTimer", &NFCLuaScriptModule::StopTimer)
		.addFunction("AddClocker", &NFCLuaScriptModule::AddClocker)
		.addFunction("StopClocker", &NFCLuaScriptModule::StopClocker)
		.addFunction("SetDefaultLevel", &NFCLuaScriptModule::SetDefaultLevel)
		.addFunction("SetDefaultFlush", &NFCLuaScriptModule::SetDefaultFlush)
		.addFunction("LuaDebug", &NFCLuaScriptModule::LuaDebug)
		.addFunction("LuaInfo", &NFCLuaScriptModule::LuaInfo)
		.addFunction("LuaWarn", &NFCLuaScriptModule::LuaWarn)
		.addFunction("LuaError", &NFCLuaScriptModule::LuaError)
		.addFunction("SendMsgToPlayer", &NFCLuaScriptModule::SendMsgToPlayer)
		.addFunction("SendMsgToManyPlayer", &NFCLuaScriptModule::SendMsgToManyPlayer)
		.addFunction("SendMsgToAllPlayer", &NFCLuaScriptModule::SendMsgToAllPlayer)
		.addFunction("SendMsgToWorld", &NFCLuaScriptModule::SendMsgToWorld)
		.addFunction("SendMsgToMaster", &NFCLuaScriptModule::SendMsgToMaster)
		.addFunction("ProcessWork", &NFCLuaScriptModule::ProcessWork)
		.addFunction("ProcessTimer", &NFCLuaScriptModule::ProcessTimer)
		.addFunction("ProcessLoopTimer", &NFCLuaScriptModule::ProcessLoopTimer)
		.addFunction("BeginProfiler", &NFCLuaScriptModule::BeginProfiler)
		.addFunction("EndProfiler", &NFCLuaScriptModule::EndProfiler)
		.endClass();
	return true;
}

void NFCLuaScriptModule::RunHttpRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg)
{
	TryRunGlobalScriptFunc(luaFunc, unLinkId, requestId, firstPath, secondPath, strMsg);
}

void NFCLuaScriptModule::RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg)
{
	TryRunGlobalScriptFunc(luaFunc, unLinkId, valueId, nMsgId, strMsg);
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

uint32_t NFCLuaScriptModule::AddTimer(const std::string& luaFunc, uint64_t nInterVal, uint32_t nCallCount, const NFLuaRef& dataStr)
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
	luaTimer->mDataStr = dataStr;

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

uint32_t NFCLuaScriptModule::AddClocker(const std::string& luaFunc, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount, const NFLuaRef& dataStr)
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
	luaTimer->mDataStr = dataStr;

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

void NFCLuaScriptModule::OnAccountEventCallBack(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData)
{
	if(nEvent == eAccountEventType_CONNECTED)
	{
		if (mPlayerProxyInfoMap.ExistElement(pServerData->GetPlayerId()))
		{
			mPlayerProxyInfoMap.RemoveElement(pServerData->GetPlayerId());
		}
		mPlayerProxyInfoMap.AddElement(pServerData->GetPlayerId(), pServerData);
	}
	else if (nEvent == eAccountEventType_DISCONNECTED)
	{
		if (mPlayerProxyInfoMap.ExistElement(pServerData->GetPlayerId()))
		{
			mPlayerProxyInfoMap.RemoveElement(pServerData->GetPlayerId());
			TryRunGlobalScriptFunc("TcpSessionClose", pServerData->GetPlayerId());
		}
	}
	else if (nEvent == eAccountEventType_RECONNECTED)
	{
		if (mPlayerProxyInfoMap.ExistElement(pServerData->GetPlayerId()))
		{
			mPlayerProxyInfoMap.RemoveElement(pServerData->GetPlayerId());
		}
		mPlayerProxyInfoMap.AddElement(pServerData->GetPlayerId(), pServerData);
	}
}

NF_SHARE_PTR<PlayerGameServerInfo> NFCLuaScriptModule::GetPlayerInfo(uint64_t playerId)
{
	return mPlayerProxyInfoMap.GetElement(playerId);
}

void NFCLuaScriptModule::ReloadAllLuaFiles()
{
	TryRunGlobalScriptFunc("LuaNFrame.ReloadAll");
}

void NFCLuaScriptModule::ReloadLuaFiles()
{
	TryRunGlobalScriptFunc("LuaNFrame.ReloadFile");
}