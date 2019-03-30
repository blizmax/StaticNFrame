// -------------------------------------------------------------------------
//    @FileName         :    NFSignalHandleModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFISignalHandleModule.h"

class NFCSignalHandleModule : public NFISignalHandleModule
{
public:
	NFCSignalHandleModule();
	virtual ~NFCSignalHandleModule();

	virtual bool Init();

	virtual bool ReadyExecute();

	virtual void InitSignal();

	virtual void HandleSignal(int signo);
private:

};
