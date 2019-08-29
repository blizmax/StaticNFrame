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

#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFComm/NFPluginModule/NFTask.h"

#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

#include "NFComm/NFPluginModule/NFINetServerModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"

#include "NFComm/NFCore/NFMapEx.hpp"

#include <vector>
#include <atomic>

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

class NFTcpMessage
{
public:
	/**
	* @brief ��Ϣ����
	*
	*/
	enum MessageType
	{
		ACTOR_TCP_MESSAGE_TYPE_NULL = 0,
		ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_PROXY_MSG = 1,
		ACTOR_TCP_MESSAGE_TYPE_MANY_PLAYER_PROXY_MSG = 2,
		ACTOR_TCP_MESSAGE_TYPE_ALL_PLAYER_PROXY_MSG = 3,
		ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_WORLD_MSG = 4,
		ACTOR_TCP_MESSAGE_TYPE_ONE_PLAYER_MASTER_MSG = 5,
	};

	NFTcpMessage()
	{
		m_nMsgType = ACTOR_TCP_MESSAGE_TYPE_NULL;
		m_usLinkId = 0;
		m_nPlayerID = 0;
		m_nMsgID = 0;
		m_nLen = 0;
	}

	uint32_t m_nMsgType;
	uint32_t m_usLinkId;
	uint64_t m_nPlayerID;
	std::vector<uint64_t> m_nVecPlayerID;
	uint32_t m_nMsgID;
	uint32_t m_nLen;
	std::string m_strData;
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

	virtual void SendMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToManyPlayer(const std::vector<uint64_t>& nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);

	virtual void AddMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void AddMsgToManyPlayer(const std::vector<uint64_t>& nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void AddMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void AddMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void AddMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
public:
	/**
	* @brief ���һ��Actor���
	*
	* @return
	*/
	virtual bool AddWorkActorComponent(NFITaskComponent* pComonnet);

	/**
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceWorkActor(uint64_t balanceId);

	/**
	* @brief ������һ��actor
	*
	* @return actor����
	*/
	int GetRandWorkActor();

	/**
	* @brief ͨ��ƽ��ID���Ҫ�첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddWorkTask(NFTask* pTask);

	/**
	* @brief ѭ���첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddProcessLoopTask(NFTask* pTask);

	/**
	* @brief ѭ���첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddProcessTimerTask(NFTask* pTask);

	/**
	* @brief ѭ���첽�����task
	*
	* @param pTask Ҫ�첽�����task
	* @return
	*/
	bool AddProcessWorkTask(NFTask* pTask);

	void AddProcessTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam);

	void AddProcessLoopTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam);

	/*
	������߳�LUA�������Ķ�ʱ��
	*/
	void HandleLuaTimer();

	/*
	������߳�LUA����������Ϣ
	*/
	void HandleLuaTcpMsg();
public:
	void OnAccountEventCallBack(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData);
	NF_SHARE_PTR<PlayerGameServerInfo> GetPlayerInfo(uint64_t playerId);
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

	NFMapEx<uint64_t, PlayerGameServerInfo> mPlayerProxyInfoMap;
protected:
	NFINetServerModule* m_pNetServerModule;
	NFINetClientModule* m_pNetClientModule;
protected:
	/**
	* @brief ���صĶ�ʱ����Ϣ��,
	* actor�߳̽����ݷ�����У� ���̴߳Ӷ�����ȡ���ݴ���
	*/
	NFQueueVector<NFTcpMessage> m_mTcpMsgQueue;
};