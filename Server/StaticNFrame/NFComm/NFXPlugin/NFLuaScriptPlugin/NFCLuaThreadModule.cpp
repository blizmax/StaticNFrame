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
	m_pNetServerModule = m_pPluginManager->FindModule<NFINetServerModule>();
	m_pNetClientModule = m_pPluginManager->FindModule<NFINetClientModule>();

	m_pPluginManager->FindModule<NFIServerNetEventModule>()->AddAccountEventCallBack(NF_ST_GAME, this, &NFCLuaThreadModule::OnAccountEventCallBack);

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
	HandleLuaTcpMsg();
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

/*
处理多线程LUA发过来的消息
*/
void NFCLuaThreadModule::HandleLuaTcpMsg()
{
	std::vector<NFTcpMessage> listTask;
	const bool ret = m_mTcpMsgQueue.Pop(listTask);
	if (ret)
	{
		const uint64_t start = NFTime::Tick();
		for (auto it = listTask.begin(); it != listTask.end(); ++it)
		{
			NFTcpMessage& xMsg = *it;
			if (xMsg.m_nMsgType == NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_PROXY_MSG)
			{
				SendMsgToPlayer(xMsg.m_usLinkId, xMsg.m_nPlayerID, xMsg.m_nMsgID, xMsg.m_nLen, xMsg.m_strData);
			}
			else if (xMsg.m_nMsgType == NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_WORLD_MSG)
			{
				SendMsgToWorld(xMsg.m_usLinkId, xMsg.m_nPlayerID, xMsg.m_nMsgID, xMsg.m_nLen, xMsg.m_strData);
			}
			else if (xMsg.m_nMsgType == NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_MASTER_MSG)
			{
				SendMsgToMaster(xMsg.m_usLinkId, xMsg.m_nPlayerID, xMsg.m_nMsgID, xMsg.m_nLen, xMsg.m_strData);
			}
			if (xMsg.m_nMsgType == NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_MANY_PLAYER_PROXY_MSG)
			{
				SendMsgToManyPlayer(xMsg.m_nVecPlayerID, xMsg.m_nMsgID, xMsg.m_nLen, xMsg.m_strData);
			}
			if (xMsg.m_nMsgType == NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ALL_PLAYER_PROXY_MSG)
			{
				SendMsgToAllPlayer(xMsg.m_nMsgID, xMsg.m_nLen, xMsg.m_strData);
			}
		}
	}

	listTask.clear();
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

void NFCLuaThreadModule::SendMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
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

void NFCLuaThreadModule::SendMsgToManyPlayer(const std::vector<uint64_t>& nVecPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
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

void NFCLuaThreadModule::SendMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
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

void NFCLuaThreadModule::SendMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
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

void NFCLuaThreadModule::SendMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	if (m_pNetClientModule)
	{
		if (usLinkId != 0)
		{
			m_pNetClientModule->SendByServerID(usLinkId, nMsgID, strData, nPlayerID);
		}
	}
}

void NFCLuaThreadModule::OnAccountEventCallBack(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData)
{
	if (nEvent == eAccountEventType_CONNECTED)
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
			//TryRunGlobalScriptFunc("TcpSessionClose", pServerData->GetPlayerId());
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

NF_SHARE_PTR<PlayerGameServerInfo> NFCLuaThreadModule::GetPlayerInfo(uint64_t playerId)
{
	return mPlayerProxyInfoMap.GetElement(playerId);
}

void NFCLuaThreadModule::AddMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	NFTcpMessage msg;
	msg.m_nMsgType = NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_PROXY_MSG;
	msg.m_usLinkId = usLinkId;
	msg.m_nPlayerID = nPlayerID;
	msg.m_nMsgID = nMsgID;
	msg.m_nLen = nLen;
	msg.m_strData = strData;

	m_mTcpMsgQueue.Push(msg);
}

void NFCLuaThreadModule::AddMsgToManyPlayer(const std::vector<uint64_t>& nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	NFTcpMessage msg;
	msg.m_nMsgType = NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_MANY_PLAYER_PROXY_MSG;
	msg.m_usLinkId = 0;
	msg.m_nPlayerID = 0;
	msg.m_nVecPlayerID = nPlayerID;
	msg.m_nMsgID = nMsgID;
	msg.m_nLen = nLen;
	msg.m_strData = strData;

	m_mTcpMsgQueue.Push(msg);
}

void NFCLuaThreadModule::AddMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{

	NFTcpMessage msg;
	msg.m_nMsgType = NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ALL_PLAYER_PROXY_MSG;
	msg.m_usLinkId = 0;
	msg.m_nPlayerID = 0;
	msg.m_nMsgID = nMsgID;
	msg.m_nLen = nLen;
	msg.m_strData = strData;

	m_mTcpMsgQueue.Push(msg);
}

void NFCLuaThreadModule::AddMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	NFTcpMessage msg;
	msg.m_nMsgType = NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_WORLD_MSG;
	msg.m_usLinkId = usLinkId;
	msg.m_nPlayerID = nPlayerID;
	msg.m_nMsgID = nMsgID;
	msg.m_nLen = nLen;
	msg.m_strData = strData;

	m_mTcpMsgQueue.Push(msg);
}

void NFCLuaThreadModule::AddMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData)
{
	NFTcpMessage msg;
	msg.m_nMsgType = NFTcpMessage::ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_MASTER_MSG;
	msg.m_usLinkId = usLinkId;
	msg.m_nPlayerID = nPlayerID;
	msg.m_nMsgID = nMsgID;
	msg.m_nLen = nLen;
	msg.m_strData = strData;

	m_mTcpMsgQueue.Push(msg);
}

/**
* @brief 添加一个Actor组件
*
* @return
*/
bool NFCLuaThreadModule::AddWorkActorComponent(NFITaskComponent* pComonnet)
{
	int actorId = FindModule<NFITaskModule>()->RequireActor();
	if (actorId <= 0)
	{
		return false;
	}

	FindModule<NFITaskModule>()->AddActorComponent(actorId, pComonnet);

	m_vecWorkActorPool.push_back(actorId);
	return true;
}

/**
* @brief 通过任务的动态均衡id，获得actor
*		 为了防止数据库错乱，防止同时对数据库表中的一条数据，读取写入，
*		 使用动态均衡id, 使得在某个时候只有一条线程对表中的一条数据，读取或写入
* @param balanceId 动态均衡id
* @return	一个actor索引
*/
int NFCLuaThreadModule::GetBalanceWorkActor(uint64_t balanceId)
{
	if (balanceId == 0)
	{
		return GetRandWorkActor();
	}
	else
	{
		if (m_vecWorkActorPool.size() <= 0)
		{
			return -1;
		}
		mnSuitIndex = balanceId % m_vecWorkActorPool.size();
		return m_vecWorkActorPool[mnSuitIndex];
	}
}

/**
* @brief 随机获得一个actor
*
* @return actor索引
*/
int NFCLuaThreadModule::GetRandWorkActor()
{
	if (m_vecWorkActorPool.size() <= 0)
	{
		return -1;
	}

	mnSuitIndex++;
	mnSuitIndex = mnSuitIndex % m_vecWorkActorPool.size();

	return m_vecWorkActorPool[mnSuitIndex];
}

/**
* @brief 通过平衡ID添加要异步处理的task
*
* @param pTask 要异步处理的task
* @return
*/
bool NFCLuaThreadModule::AddWorkTask(NFTask* pTask)
{
	if (pTask)
	{
		int actorId = GetBalanceWorkActor(pTask->GetBalanceId());
		if (actorId > 0)
		{
			return FindModule<NFITaskModule>()->AddTask(actorId, pTask);
		}
	}

	return false;
}

/**
* @brief 循环异步处理的task
*
* @param pTask 要异步处理的task
* @return
*/
bool NFCLuaThreadModule::AddProcessLoopTask(NFTask* pTask)
{
	if (pTask)
	{
		return FindModule<NFITaskModule>()->AddTask(m_processLoopActorId, pTask);
	}

	return false;
}

/**
* @brief 循环异步处理的task
*
* @param pTask 要异步处理的task
* @return
*/
bool NFCLuaThreadModule::AddProcessTimerTask(NFTask* pTask)
{
	if (pTask)
	{
		int actorId = GetBalanceWorkActor(pTask->GetBalanceId());
		if (actorId > 0)
		{
			return FindModule<NFITaskModule>()->AddTask(actorId, pTask);
		}
	}

	return false;
}

/**
* @brief 循环异步处理的task
*
* @param pTask 要异步处理的task
* @return
*/
bool NFCLuaThreadModule::AddProcessWorkTask(NFTask* pTask)
{
	if (pTask)
	{
		int actorId = GetBalanceWorkActor(pTask->GetBalanceId());
		if (actorId > 0)
		{
			return FindModule<NFITaskModule>()->AddTask(actorId, pTask);
		}
	}

	return false;
}

void NFCLuaThreadModule::AddProcessTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam)
{
	NFTimerMessage msg;
	msg.nMsgType = NFTimerMessage::ACTOR_TIMER_MSG_PROCESS_TIMER;
	msg.m_delayTime = delayTimer;
	msg.m_luaFunc = luaFunc;
	msg.m_tmpParam = tmpParam;

	m_mQueue.Push(msg);
}

void NFCLuaThreadModule::AddProcessLoopTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam)
{
	NFTimerMessage msg;
	msg.nMsgType = NFTimerMessage::ACTOR_TIMER_MSG_PROCESS_LOOP_TIMER;
	msg.m_delayTime = delayTimer;
	msg.m_luaFunc = luaFunc;
	msg.m_tmpParam = tmpParam;

	m_mQueue.Push(msg);
}