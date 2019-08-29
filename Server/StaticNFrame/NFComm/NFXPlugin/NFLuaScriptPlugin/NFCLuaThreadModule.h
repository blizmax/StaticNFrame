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
* @brief actor定时器消息数据
*
*/
class NFTimerMessage
{
public:
	/**
	* @brief 消息类型
	*
	*/
	enum MessageType
	{
		ACTOR_TIMER_MSG_PROCESS_NULL = 0,
		ACTOR_TIMER_MSG_PROCESS_TIMER = 1,
		ACTOR_TIMER_MSG_PROCESS_LOOP_TIMER = 2,
	};

	/**
	* @brief 构造函数
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
	* @brief 消息类型
	*
	*/
	int nMsgType;

	/**
	* @brief 延时时间
	*
	*/
	int m_delayTime;

	/**
	* @brief 要延时执行的LUA函数
	*
	*/
	std::string m_luaFunc;

	/**
	* @brief 要延时执行的LUA函数的参数
	*
	*/
	std::string m_tmpParam;
};

class NFTcpMessage
{
public:
	/**
	* @brief 消息类型
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
	* @brief 添加一个Actor组件
	*
	* @return
	*/
	virtual bool AddWorkActorComponent(NFITaskComponent* pComonnet);

	/**
	* @brief 通过任务的动态均衡id，获得actor
	*		 为了防止数据库错乱，防止同时对数据库表中的一条数据，读取写入，
	*		 使用动态均衡id, 使得在某个时候只有一条线程对表中的一条数据，读取或写入
	* @param balanceId 动态均衡id
	* @return	一个actor索引
	*/
	int GetBalanceWorkActor(uint64_t balanceId);

	/**
	* @brief 随机获得一个actor
	*
	* @return actor索引
	*/
	int GetRandWorkActor();

	/**
	* @brief 通过平衡ID添加要异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	bool AddWorkTask(NFTask* pTask);

	/**
	* @brief 循环异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	bool AddProcessLoopTask(NFTask* pTask);

	/**
	* @brief 循环异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	bool AddProcessTimerTask(NFTask* pTask);

	/**
	* @brief 循环异步处理的task
	*
	* @param pTask 要异步处理的task
	* @return
	*/
	bool AddProcessWorkTask(NFTask* pTask);

	void AddProcessTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam);

	void AddProcessLoopTimer(uint32_t delayTimer, const std::string& luaFunc, const std::string& tmpParam);

	/*
	处理多线程LUA发过来的定时器
	*/
	void HandleLuaTimer();

	/*
	处理多线程LUA发过来的消息
	*/
	void HandleLuaTcpMsg();
public:
	void OnAccountEventCallBack(uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<PlayerGameServerInfo> pServerData);
	NF_SHARE_PTR<PlayerGameServerInfo> GetPlayerInfo(uint64_t playerId);
protected:
	/**
	* @brief actor索引数组
	*/
	std::vector<int> m_vecWorkActorPool;

	/**
	* @brief 用来平衡随机获得actor
	*/
	atomic_uint32_t mnSuitIndex = 0;

	/**
	* @brief server loop actor
	*/
	int m_processLoopActorId;

	/**
	* @brief 返回的定时器消息队,
	* actor线程将数据放入队列， 主线程从队列里取数据处理
	*/
	NFQueueVector<NFTimerMessage> m_mQueue;

	/*
		定时器
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
	* @brief 返回的定时器消息队,
	* actor线程将数据放入队列， 主线程从队列里取数据处理
	*/
	NFQueueVector<NFTcpMessage> m_mTcpMsgQueue;
};