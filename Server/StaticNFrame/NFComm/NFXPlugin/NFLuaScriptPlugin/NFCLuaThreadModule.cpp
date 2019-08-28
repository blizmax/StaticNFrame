// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaThreadModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/08/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCLuaThreadModule.h"
#include "NFCLuaScriptComponent.h"

void NFLuaThreadTimer::OnTimer(uint32_t nTimerID)
{
	if (mMsgType == NFTimerMessage::ACTOR_TIMER_MSG_PROCESS_TIMER)
	{
		m_pLuaScriptModule->AddProcessTimerTask(new NFProcessTimerActorTask(m_pLuaScriptModule, EnumLuaThreadModule_Work, mLuaFunc, mDataStr));
	}
	else if (mMsgType == NFTimerMessage::ACTOR_TIMER_MSG_PROCESS_LOOP_TIMER)
	{
		m_pLuaScriptModule->AddProcessLoopTask(new NFProcessLoopTimerActorTask(m_pLuaScriptModule, EnumLuaThreadModule_Work, mLuaFunc, mDataStr));
	}
}

bool NFCLuaThreadModule::Init()
{
	StartActorPool(10);
	SetTimer(EnumLuaThreadModule_LOAD, 1, 1);
	return true;
}

bool NFCLuaThreadModule::AfterInit()
{
	return true;
}

bool NFCLuaThreadModule::ReadyExecute()
{
	return true;
}

bool NFCLuaThreadModule::Execute()
{
	HandleLuaTimer();
	return true;
}

bool NFCLuaThreadModule::BeforeShut()
{
	return true;
}

bool NFCLuaThreadModule::Shut()
{
	return true;
}

bool NFCLuaThreadModule::Finalize()
{
	return true;
}

bool  NFCLuaThreadModule::IsInitLua()
{
	for (size_t i = 0; i < m_vecWorkActorPool.size(); i++)
	{
		int actorId = m_vecWorkActorPool[i];
		const std::vector<NFITaskComponent*> vecComponent = FindModule<NFITaskModule>()->GetTaskComponent(actorId);
		for (size_t i = 0; i < vecComponent.size(); i++)
		{
			NFCLuaScriptComponent* pComponent = dynamic_cast<NFCLuaScriptComponent*>(vecComponent[i]);
			if (pComponent)
			{
				if (pComponent->IsInitLua() == false)
				{
					return false;
				}
			}
		}
	}

	const std::vector<NFITaskComponent*> vecComponent = FindModule<NFITaskModule>()->GetTaskComponent(m_processLoopActorId);
	for (size_t i = 0; i < vecComponent.size(); i++)
	{
		NFCLuaScriptComponent* pComponent = dynamic_cast<NFCLuaScriptComponent*>(vecComponent[i]);
		if (pComponent)
		{
			if (pComponent->IsInitLua() == false)
			{
				return false;
			}
		}
	}
	return true;
}

void NFCLuaThreadModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == EnumLuaThreadModule_LOAD)
	{
		AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_LOAD));
		for (size_t i = 0; i < m_vecWorkActorPool.size(); i++)
		{
			int actorId = m_vecWorkActorPool[i];
			FindModule<NFITaskModule>()->AddTask(actorId, new NFWorkActorTask(this, EnumLuaThreadModule_LOAD));
		}
	}
	if (nTimerID == EnumLuaThreadModule_Init)
	{
		if (IsInitLua())
		{
			KillTimer(EnumLuaThreadModule_Init);
			AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_Init, "gametimer"));
			//AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_Init, "utilstimer"));
			//AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_Init, "logtimer"));
			//AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_Init, "pokertimer"));
		}
	}
	if (nTimerID == EnumLuaThreadModule_Loop)
	{
		AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_Loop, "gametimer"));
		AddProcessLoopTask(new NFServerLoopTask(this, EnumLuaThreadModule_Loop, "utilstimer"));
	}
}

void NFCLuaThreadModule::RunHttpRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg)
{
	AddWorkTask(new NFHttpMsgActorTask(this, luaFunc, unLinkId, requestId, firstPath, secondPath, strMsg));
}

void NFCLuaThreadModule::RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg)
{
	AddWorkTask(new NFTcpMsgActorTask(this, luaFunc, unLinkId, valueId, nMsgId, strMsg));
}

bool NFCLuaThreadModule::StartActorPool(const int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		NFCLuaScriptComponent* pComonnet = NF_NEW NFCLuaScriptComponent(this, m_pPluginManager);
		AddWorkActorComponent(pComonnet);
	}

	NFCLuaScriptComponent* pLoopComonnet = NF_NEW NFCLuaScriptComponent(this, m_pPluginManager);
	m_processLoopActorId = FindModule<NFITaskModule>()->RequireActor();
	if (m_processLoopActorId <= 0)
	{
		return false;
	}

	FindModule<NFITaskModule>()->AddActorComponent(m_processLoopActorId, pLoopComonnet);

	return true;
}

bool NFCLuaThreadModule::CloseActorPool()
{
	return true;
}

uint32_t NFCLuaThreadModule::AddTimer(uint32_t msgType, const std::string& luaFunc, uint64_t nInterVal, const std::string& dataStr)
{
	NFLuaThreadTimer* luaTimer = nullptr;
	if (m_luaTimerList.empty())
	{
		luaTimer = NF_NEW NFLuaThreadTimer(this);
	}
	else
	{
		luaTimer = m_luaTimerList.front();
		m_luaTimerList.pop_front();
		luaTimer->m_pLuaScriptModule = this;
	}

	luaTimer->mMsgType = msgType;
	luaTimer->mLuaFunc = luaFunc;
	luaTimer->mInterVal = nInterVal;
	luaTimer->mDataStr = dataStr;
	luaTimer->mCallCount = 1;

	luaTimer->mCurCallCount = 0;
	luaTimer->mTimerId = ++m_luaTimerIndex;

	luaTimer->SetTimer(luaTimer->mTimerId, luaTimer->mInterVal, luaTimer->mCallCount);
	m_luaTimerMap.emplace(luaTimer->mTimerId, luaTimer);

	return luaTimer->mTimerId;
}

void NFCLuaThreadModule::HandleLuaTimer()
{
	std::vector<NFTimerMessage> listTask;
	const bool ret = m_mQueue.Pop(listTask);
	if (ret)
	{
		const uint64_t start = NFTime::Tick();
		for (auto it = listTask.begin(); it != listTask.end(); ++it)
		{
			NFTimerMessage& xMsg = *it;
			AddTimer(xMsg.nMsgType, xMsg.m_luaFunc, xMsg.m_delayTime, xMsg.m_tmpParam);
		}
	}

	listTask.clear();
}
