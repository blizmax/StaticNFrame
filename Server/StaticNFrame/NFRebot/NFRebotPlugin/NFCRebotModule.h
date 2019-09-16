// -------------------------------------------------------------------------
//    @FileName         :    NFCRebotModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFRebotPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <string>

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"

class NFCRebotModule : public NFIModule, NFTimerObj
{
public:
	explicit NFCRebotModule(NFIPluginManager* p);
	virtual ~NFCRebotModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnTimer(uint32_t nTimerID) override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);

	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
};
