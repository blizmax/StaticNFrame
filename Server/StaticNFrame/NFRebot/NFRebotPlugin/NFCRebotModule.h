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
#include "NFComm/NFPluginModule/NFEventObj.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

class NFCRebotModule : public NFIModule, NFTimerObj, NFEventObj
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

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);

	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	std::map<uint32_t, uint32_t> m_rebotInfo;
};
