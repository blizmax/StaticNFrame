// -------------------------------------------------------------------------
//    @FileName         :    NFCTimerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFKernelPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFITimerModule.h"
#include "NFComm/NFPluginModule/NFTimerAxis.h"

class NFCTimerModule : public NFITimerModule
{
public:
	explicit NFCTimerModule(NFIPluginManager* p);

	virtual ~NFCTimerModule();

public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
public:
	//���ö�ʱ��
	virtual bool SetTimer(uint32_t nTimerID, uint64_t nInterVal, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL) override;

	//�رն�ʱ��
	virtual bool KillTimer(uint32_t nTimerID, NFTimerObj* handler) override;

	//�ر����ж�ʱ��
	virtual bool KillAllTimer(NFTimerObj* handler) override;

	//���ù̶�ʱ��Ķ�ʱ��
	virtual bool SetClocker(uint32_t nTimerID, uint64_t nStartTime, uint32_t nInterDays, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL) override;

	//���ù̶�ʱ��Ķ�ʱ��
	virtual bool SetCalender(uint32_t nTimerID, const std::string& timeStr, NFTimerObj* handler, uint32_t nCallCount = INFINITY_CALL) override;
protected:
	NFTimerAxis mTimerAxis;
};

