// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaThreadModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/08/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFILuaThreadModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFILuaModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFCore/NFQueue.hpp"

#include <vector>
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFTask.h"

#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

class NFCLuaThreadModule;

/**
* @brief actor��ʱ����Ϣ����
*
*/
class NFTimerMessage
{
public:
	/**
	* @brief ��Ϣ����
	*
	*/
	enum MessageType
	{
		ACTOR_TIMER_MSG_PROCESS_NULL = 0,
		ACTOR_TIMER_MSG_PROCESS_TIMER = 1,
		ACTOR_TIMER_MSG_PROCESS_LOOP_TIMER = 2,
	};

	/**
	* @brief ���캯��
	*
	*/
	NFTimerMessage()
	{

	}

	virtual ~NFTimerMessage()
	{
		m_delayTime = 0;
		nMsgType = ACTOR_TIMER_MSG_PROCESS_NULL;
	}

public:
	/**
	* @brief ��Ϣ����
	*
	*/
	int nMsgType;

	/**
	* @brief ��ʱʱ��
	*
	*/
	int m_delayTime;

	/**
	* @brief Ҫ��ʱִ�е�LUA����
	*
	*/
	std::string m_luaFunc;

	/**
	* @brief Ҫ��ʱִ�е�LUA�����Ĳ���
	*
	*/
	std::string m_tmpParam;
};

class NFLuaThreadTimer : public NFTimerObj
{
public:
	NFLuaThreadTimer(NFCLuaThreadModule* p)
	{
		Clear();
		m_pLuaScriptModule = p;
	}

	void Clear()
	{
		mTimerId = 0;
		mInterVal = 0;
		mCallCount = 0;
		mCurCallCount = 0;
	}

	uint32_t mTimerId;
	std::string mLuaFunc;
	uint32_t mMsgType;
	uint64_t mInterVal;
	uint32_t mCallCount;
	uint32_t mCurCallCount;
	std::string mDataStr;
	NFCLuaThreadModule* m_pLuaScriptModule;

	virtual void OnTimer(uint32_t nTimerID) override;
};

class NFCLuaThreadModule : public NFILuaScriptModule, public NFTimerObj
{
public:
	NFCLuaThreadModule(NFIPluginManager* p)
	{
		m_pPluginManager = p;
	}

	virtual ~NFCLuaThreadModule()
	{

	}

public:
	virtual bool Init();
	virtual bool AfterInit();
	virtual bool ReadyExecute();

	virtual bool Execute();

	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Finalize();

	virtual bool IsInitLua();

	virtual void OnTimer(uint32_t nTimerID);

	virtual bool StartActorPool(const int nCount);
	virtual bool CloseActorPool();

	virtual uint32_t AddTimer(uint32_t mMsgType, const std::string& luaFunc, uint64_t nInterVal, const std::string& dataStr);
public:
	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg) override;
	virtual void RunHttpRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg) override;
public:
	/**
	* @brief ���һ��Actor���
	*
	* @return
	*/
	virtual bool AddWorkActorComponent(NFITaskComponent* pComonnet)
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
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceWorkActor(uint64_t balanceId)
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
	* @brief ������һ��actor
	*
	* @return actor����
	*/
	int GetRandWorkActor()
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
	* @brief ͨ��ƽ��ID���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddWorkTask(NFTask* pTask)
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
	* @brief ѭ���첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddProcessLoopTask(NFTask* pTask)
	{
		if (pTask)
		{
			return FindModule<NFITaskModule>()->AddTask(m_processLoopActorId, pTask);
		}

		return false;
	}

	/**
* @brief ѭ���첽�����task
*
* @param pTask Ҫ�첽�����task
* @return
*/
	bool AddProcessTimerTask(NFTask* pTask)
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
	* @brief ѭ���첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddProcessWorkTask(NFTask* pTask)
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

	void AddProcessTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam)
	{
		NFTimerMessage msg;
		msg.nMsgType = NFTimerMessage::ACTOR_TIMER_MSG_PROCESS_TIMER;
		msg.m_delayTime = delayTimer;
		msg.m_luaFunc = luaFunc;
		msg.m_tmpParam = tmpParam;

		m_mQueue.Push(msg);
	}

	void AddProcessLoopTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam)
	{
		NFTimerMessage msg;
		msg.nMsgType = NFTimerMessage::ACTOR_TIMER_MSG_PROCESS_LOOP_TIMER;
		msg.m_delayTime = delayTimer;
		msg.m_luaFunc = luaFunc;
		msg.m_tmpParam = tmpParam;

		m_mQueue.Push(msg);
	}

	/*
	������߳�LUA�������Ķ�ʱ��
	*/
	void HandleLuaTimer();
protected:
	/**
	* @brief actor��������
	*/
	std::vector<int> m_vecWorkActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	atomic_uint32_t mnSuitIndex = 0;

	/**
	* @brief server loop actor
	*/
	int m_processLoopActorId;

	/**
	* @brief ���صĶ�ʱ����Ϣ��,
	* actor�߳̽����ݷ�����У� ���̴߳Ӷ�����ȡ���ݴ���
	*/
	NFQueueVector<NFTimerMessage> m_mQueue;

	/*
		��ʱ��
	*/
	std::map<uint64_t, NFLuaThreadTimer*> m_luaTimerMap;
	std::list<NFLuaThreadTimer*> m_luaTimerList;
	uint32_t m_luaTimerIndex;
};