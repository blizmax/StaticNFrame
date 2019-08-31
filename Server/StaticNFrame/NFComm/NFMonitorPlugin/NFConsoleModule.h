// -------------------------------------------------------------------------
//    @FileName         :    NFConsoleModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-30
//    @Module           :    NFConsoleModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIConsoleModule.h"
#include "NFComm/NFPluginModule/NFTimerObj.h"
#include "NFComm/NFCore/NFCmdLine.h"
#include "NFComm/NFCore/NFQueue.hpp"

#include <thread>
#include <future>

enum NFConsoleMsgEnum
{
	NFConsoleMsg_Null = 0,
	NFConsoleMsg_Exit = 1,
	NFConsoleMsg_Profiler = 2,
	NFConsoleMsg_Reload = 3,	//重载引擎配置
	NFConsoleMsg_Dynamic = 4,	//动态加载引擎
	NFConsoleMsg_ProductFile = 5, //类节点头文件
};

struct NFConsoleMsg
{
	NFConsoleMsgEnum mMsgType;
	std::string mParam1;
	std::string mParam2;
	std::string mParam3;
	std::string mParam4;
	std::string mParam5;
};

class NFCConsoleModule : public NFIConsoleModule, public NFTimerObj
{
public:
	NFCConsoleModule(NFIPluginManager* p);
	virtual ~NFCConsoleModule();
public:
	virtual bool Awake();

	virtual bool Execute();

	virtual bool Shut();

	virtual bool OnReloadPlugin();

	virtual void OnTimer(uint32_t nTimerID);
public:
	virtual void CreateBackThread();
	virtual void BackThreadLoop();
private:
	NFCmdLine::NFParser mCmdParser;
	std::thread		mThread;
	NFQueueVector<NFConsoleMsg>   mQueueMsg;
};