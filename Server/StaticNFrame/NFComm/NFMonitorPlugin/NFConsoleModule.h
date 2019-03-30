// -------------------------------------------------------------------------
//    @FileName         :    NFConsoleModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-30
//    @Module           :    NFConsoleModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIConsoleModule.h"

#include "NFComm/NFCore/NFCmdLine.h"

#include <thread>
#include <future>

class NFCConsoleModule : public NFIConsoleModule
{
public:
	NFCConsoleModule(NFIPluginManager* p);
	virtual ~NFCConsoleModule();
public:
	virtual bool Awake();

	virtual bool Execute();

	virtual bool OnReloadPlugin();
public:
	virtual void CreateBackThread();
	virtual void BackThreadLoop();
private:
	NFCmdLine::NFParser mCmdParser;
	std::thread		mThread;
};