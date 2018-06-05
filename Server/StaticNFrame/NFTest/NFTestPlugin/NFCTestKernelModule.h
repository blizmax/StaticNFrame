// -------------------------------------------------------------------------
//    @FileName         :    NFCTestKernelModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFTimerObj.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

#include <iostream>

class NFITestKernelModule : public NFIModule
{
};

class NFTestEvent0 : public NFEventContext
{
public:
	NFTestEvent0()
	{
		a = 0;
		c = 0;
	}

public:
	int a;
	std::string b;
	int c;
};

class NFTestEvent : public NFEventContext
{
public:
	NFTestEvent()
	{
		a = 0;
	}

public:
	int a;
	std::string b;
	std::string c;
};

enum eEventType
{
	eEvent_one_cycle = 1,
	eEvent_two = 2,
	eEvent_three = 3,
};

class NFTestObject : public NFEventObj
{
public:
	NFTestObject()
	{
		Subscribe(NFEVENT_TEST, 123456789, eEvent_one_cycle, __FUNCTION__);
		Subscribe(NFEVENT_TEST, 123456789, eEvent_two, __FUNCTION__);
		Subscribe(NFEVENT_TEST, 123456789, eEvent_three, __FUNCTION__);
	}

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) override
	{
		std::cout << __FUNCTION__ << "::";
		std::cout << "--nEventID:" << nEventID;
		std::cout << "--nSrcID:" << nSrcID;
		std::cout << "--bySrcType:" << static_cast<uint32_t>(bySrcType);
		std::cout << std::endl;
	}
};

class NFCTestKernelModule : public NFITestKernelModule, NFTimerObj, NFEventObj
{
	enum eTimer
	{
		eTimer_test = 1,
	};

public:
	explicit NFCTestKernelModule(NFIPluginManager* p);

	virtual ~NFCTestKernelModule()
	{
		delete m_pObject;
	}

public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
public:
	virtual void OnTimer(uint32_t nTimerID) override;

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) override;
protected:
	uint64_t lastTime = 0;
	NFTestObject* m_pObject;
};
