// -------------------------------------------------------------------------
//    @FileName         :    NFSignalHandleModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFISignalHandleModule.h"
#include "NFComm/NFCore/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/prctl.h>
#endif

class NFCSignalHandleModule : public NFISignalHandleModule
{
public:
	NFCSignalHandleModule();
	virtual ~NFCSignalHandleModule();

	virtual bool Init();

	virtual bool ReadyExecute();

	virtual void InitSignal();

	virtual void HandleSignal(int signo);
};
