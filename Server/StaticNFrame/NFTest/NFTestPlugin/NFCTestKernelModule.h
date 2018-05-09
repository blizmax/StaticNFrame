// -------------------------------------------------------------------------
//    @FileName         :    {Filename}
//    @Author           :    GaoYi
//    @Date             :    2017-09-27
//    @Module           :   {Filename}
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFTimerObj.h"
#include "NFComm/NFPluginModule/NFEventObj.h"

class NFITestKernelModule : public NFIModule
{

};

class NFTestEvent0 : public NFEventContext
{
public:
	NFTestEvent0() { a = 0; c = 0; }
public:
	int a;
	std::string b;
	int c;	
};

class NFTestEvent : public NFEventContext
{
public:
	NFTestEvent() { a = 0; }
public:
	int a;
	std::string b;
	std::string c;
};

class NFCTestKernelModule : public NFITestKernelModule, NFTimerObj, NFEventObj
{
	enum eTimer
	{
		eTimer_test = 1,
	};
public:
    NFCTestKernelModule(NFIPluginManager* p);
public:
    virtual bool Init();

    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();

    virtual bool Shut();
public:
	virtual void OnTimer(uint32_t nTimerID);

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext);
protected:
	uint64_t lastTime = 0;
};
