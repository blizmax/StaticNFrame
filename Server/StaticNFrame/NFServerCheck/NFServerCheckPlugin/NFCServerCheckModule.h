// -------------------------------------------------------------------------
//    @FileName         :    NFCServerCheckModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/11/9
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCheckPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <string>

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

class NFCServerCheckModule : public NFIModule, NFTimerObj, NFEventObj
{
public:
	explicit NFCServerCheckModule(NFIPluginManager* p);
	virtual ~NFCServerCheckModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnTimer(uint32_t nTimerID) override;

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);

	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
};
