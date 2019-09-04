// -------------------------------------------------------------------------
//    @FileName         :    NFCLuaScriptComponent.h
//    @Author           :    GaoYi
//    @Date             :    2018/08/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFITaskComponent.h"
#include "NFComm/NFPluginModule/NFILuaModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIScriptModule.h"

#include "NFComm/NFPluginModule/NFILuaModule.h"
#include "NFComm/NFCore/NFFileUtility.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFPluginModule/NFIHttpClientModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include "NFCLuaThreadModule.h"

class NFCLuaThreadModule;
class NFCLuaScriptComponent;

enum EnumLuaThreadModule
{
	EnumLuaThreadModule_NULL = 0,
	EnumLuaThreadModule_LOAD = 1,
	EnumLuaThreadModule_Init = 2,
	EnumLuaThreadModule_Loop = 3,
	EnumLuaThreadModule_Work = 4,
};

class NFLuaThreadTask : public NFTask
{
public:
	NFLuaThreadTask()
	{
		m_taskType = EnumLuaThreadModule_NULL;
		m_pLuaThreadModule = nullptr;
		m_pComponent = nullptr;
	}
	uint32_t m_taskType;
	NFCLuaThreadModule* m_pLuaThreadModule;
	NFCLuaScriptComponent* m_pComponent;
};

class NFWorkActorTask : public NFLuaThreadTask
{
public:
	NFWorkActorTask(NFCLuaThreadModule* pLuaThreadModule, uint32_t taskType, const std::string& luaFunc = "", const std::string& param = "")
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskType = taskType;
		m_luaFunc = luaFunc;
		m_param = param;
		if (m_taskType == EnumLuaThreadModule_LOAD)
		{
			m_taskName = "WorkTask_Load";
		}
		else if (m_taskType == EnumLuaThreadModule_Work)
		{
			m_taskName = "WorkTask_" + luaFunc;
		}
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
public:
	std::string m_param;
	std::string m_luaFunc;
};

class NFServerLoopTask : public NFLuaThreadTask
{
public:
	NFServerLoopTask(NFCLuaThreadModule* pLuaThreadModule, uint32_t taskType, const std::string& param = "")
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskType = taskType;
		m_param = param;
		
		if (m_taskType == EnumLuaThreadModule_LOAD)
		{
			m_taskName = "ServerLoopTask_Load";
		}
		else if (m_taskType == EnumLuaThreadModule_Init)
		{
			m_taskName = "ServerLoopTask_init";
		}
		else if (m_taskType == EnumLuaThreadModule_Loop)
		{
			m_taskName = "ServerLoopTask_" + param;
		}
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess();
public:
	std::string m_param;
};

class NFProcessTimerActorTask : public NFLuaThreadTask
{
public:
	NFProcessTimerActorTask(NFCLuaThreadModule* pLuaThreadModule, uint32_t taskType, const std::string& luaFunc = "", const std::string& param = "")
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskType = taskType;
		m_luaFunc = luaFunc;
		m_param = param;
		m_taskName = "ProcessTimerTask_" + luaFunc;
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
public:
	std::string m_param;
	std::string m_luaFunc;
};

class NFProcessLoopTimerActorTask : public NFLuaThreadTask
{
public:
	NFProcessLoopTimerActorTask(NFCLuaThreadModule* pLuaThreadModule, uint32_t taskType, const std::string& luaFunc = "", const std::string& param = "")
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskType = taskType;
		m_luaFunc = luaFunc;
		m_param = param;
		m_taskName = "ProcessLoopTimerTask_" + luaFunc;
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
public:
	std::string m_param;
	std::string m_luaFunc;
};

class NFTcpMsgActorTask : public NFLuaThreadTask
{
public:
	NFTcpMsgActorTask(NFCLuaThreadModule* pLuaThreadModule, const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg)
	{
		m_balanceId = valueId;
		m_pLuaThreadModule = pLuaThreadModule;
		m_luaFunc = luaFunc;
		m_unLinkId = unLinkId;
		m_valueId = valueId;
		m_msgId = nMsgId;
		m_strMsg = strMsg;
		m_taskName = "TcpMsgTask_" + luaFunc + NFCommon::tostr(nMsgId);
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
public:
	std::string m_luaFunc;
	uint32_t m_unLinkId;
	uint64_t m_valueId;
	uint32_t m_msgId;
	std::string m_strMsg;
};

class NFHttpMsgActorTask : public NFLuaThreadTask
{
public:
	NFHttpMsgActorTask(NFCLuaThreadModule* pLuaThreadModule, const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg)
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_luaFunc = luaFunc;
		m_unLinkId = unLinkId;
		m_requestId = requestId;
		m_firstPath = firstPath;
		m_secondPath = secondPath;
		m_strMsg = strMsg;
		m_taskName = "HttpMsgTask_" + luaFunc + "_" + firstPath + "_" + secondPath;
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
public:
	std::string m_luaFunc;
	uint32_t m_unLinkId;
	std::string m_strMsg;
	uint32_t m_requestId;
	std::string m_firstPath;
	std::string m_secondPath;
};

class NFHotfixAllLuaActorTask : public NFLuaThreadTask
{
public:
	NFHotfixAllLuaActorTask(NFCLuaThreadModule* pLuaThreadModule, const std::vector<std::string>& vecStr)
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_vecLuaFile = vecStr;
		m_taskName = "HotfixAllLua";
	}

	NFHotfixAllLuaActorTask(NFCLuaThreadModule* pLuaThreadModule)
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskName = "HotfixAllLua";
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess();
public:
	std::vector<std::string> m_vecLuaFile;
};

class NFHotfixLuaFilesActorTask : public NFLuaThreadTask
{
public:
	NFHotfixLuaFilesActorTask(NFCLuaThreadModule* pLuaThreadModule, const std::vector<std::string>& vecStr)
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_vecLuaFile = vecStr;
		m_taskName = "HotfixLuaFiles";
	}

	NFHotfixLuaFilesActorTask(NFCLuaThreadModule* pLuaThreadModule)
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskName = "HotfixLuaFiles";
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
public:
	std::vector<std::string> m_vecLuaFile;
};

class NFLuaGcActorTask : public NFLuaThreadTask
{
public:
	NFLuaGcActorTask(NFCLuaThreadModule* pLuaThreadModule)
	{
		m_pLuaThreadModule = pLuaThreadModule;
		m_taskName = "LuaGC";
	}
	/**
	**  异步线程处理函数，将在另一个线程里运行
	*/
	virtual bool ThreadProcess();

	/**
	** 主线程处理函数，将在线程处理完后，提交给主先来处理，根据返回函数是否继续处理
		返回值： thread::TPTask::TPTaskState， 请参看TPTaskState
	*/
	virtual TPTaskState MainThreadProcess()
	{
		return TPTASK_STATE_COMPLETED;
	}
};

class NFCLuaScriptComponent : public NFITaskComponent, public NFILuaModule, public NFIModule
{
public:
	NFCLuaScriptComponent(NFCLuaThreadModule* pLuaThreadModule, NFIPluginManager* p);

	virtual ~NFCLuaScriptComponent();

	void ProcessTaskStart(NFTask* pTask) override
	{
		NFLuaThreadTask* pLuaTask = dynamic_cast<NFLuaThreadTask*>(pTask);
		if (pLuaTask)
		{
			pLuaTask->m_pComponent = this;
		}
	}

	void ProcessTask(NFTask* pTask) override
	{
		if (pTask)
		{
			uint64_t startTime = NFGetTime();
			pTask->ThreadProcess();
			pTask->m_useTime = NFGetTime() - startTime;
		}
	}

	void ProcessTaskEnd(NFTask* pTask) override
	{
		NFLuaThreadTask* pLuaTask = dynamic_cast<NFLuaThreadTask*>(pTask);
		if (pLuaTask)
		{
			pLuaTask->m_pComponent = nullptr;
		}
	}

public:
	virtual uint32_t AddTimer(const std::string& luaFunc, uint64_t nInterVal, uint32_t nCallCount, const NFLuaRef& dataStr);
	virtual uint32_t AddClocker(const std::string& luaFunc, uint64_t nStartTime, uint32_t nInterDays, uint32_t nCallCount, const NFLuaRef& dataStr);
	virtual void StopTimer(uint32_t nTimerID);
	virtual void StopClocker(uint32_t nTimerID);

	virtual void RunNetRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint64_t valueId, const uint32_t nMsgId, const std::string& strMsg);
	virtual void RunHttpRecvLuaFunc(const std::string& luaFunc, const uint32_t unLinkId, const uint32_t requestId, const std::string& firstPath, const std::string& secondPath, const std::string& strMsg);
public:
	virtual const std::string& GetAppName() const;
	virtual int GetAppID() const;
	virtual uint64_t GetInitTime() const;
	virtual uint64_t GetNowTime() const;
	virtual uint64_t GetMsecTime() const;
	virtual uint64_t GetSecTime() const;
	virtual std::string GetMD5(const std::string& str);
	virtual uint32_t GetCRC32(const std::string& s);
	virtual uint16_t GetCRC16(const std::string& s);
	virtual std::string Base64Encode(const std::string& s);
	virtual std::string Base64Decode(const std::string& s);
	virtual std::string Sha256(const std::string& s);

	virtual void SendMsgToPlayer(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToManyPlayer(const std::vector<uint64_t>& nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToAllPlayer(const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToWorld(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);
	virtual void SendMsgToMaster(uint32_t usLinkId, const uint64_t nPlayerID, const uint32_t nMsgID, const uint32_t nLen, const std::string& strData);

	virtual void SetDefaultLevel(uint32_t log_level);

	virtual void SetDefaultFlush(uint32_t log_level);

	virtual void LuaDebug(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void LuaInfo(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void LuaWarn(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void LuaError(uint32_t logId, uint64_t guid, const std::string& str);

	virtual void ProcessLoop(const std::string& dataStr);

	virtual void ProcessInit(const std::string& dataStr);

	/*
		这个函数是给LUA使用的， 发送一个work给actor处理，处理可能在别的线程里
	*/
	virtual void ProcessWork(const std::string& luaFunc, const NFLuaRef& dataStr);

	virtual void ProcessTimer(uint32_t timeSec, const std::string& luaFunc, const NFLuaRef& dataStr);

	virtual void ProcessLoopTimer(uint32_t timeSec, const std::string& luaFunc, const NFLuaRef& dataStr);

	/*
		处理process work发送过来的任务
	*/
	virtual void Do_ProcessWork(const std::string& luaFunc, const std::string& dataStr);

	virtual void Do_ProcessTimer(const std::string& luaFunc, const std::string& dataStr);

	virtual void Do_ProcessLoopTimer(const std::string& luaFunc, const std::string& dataStr);

	virtual void BeginProfiler(const std::string& luaFunc);
	virtual uint64_t EndProfiler();//return this time cost time(us) 微妙

	virtual void SetInitLua(bool b) { m_initLua = b; }
	virtual bool IsInitLua() const { return m_initLua; }

	virtual void GcStep();
public:
	bool Register();
	void LoadScript();
protected:
	NFILogModule* m_pLogModule;
	NFCLuaThreadModule* m_pLuaThreadModule;
	atomic_bool m_initLua;
};