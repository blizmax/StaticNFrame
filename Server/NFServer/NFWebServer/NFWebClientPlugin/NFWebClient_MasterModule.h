// -------------------------------------------------------------------------
//    @FileName         :    NFCWebClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFCWebClient_MasterModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIWebClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCWebClient_MasterModule : public NFIWebClient_MasterModule, public NFEventObj
{
public:
	explicit NFCWebClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCWebClient_MasterModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);
	void OnHandleGmMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
	void ServerReport();

	void OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen) { }

	virtual uint32_t GetMasterLinkId() const { return m_pMasterServerData->mUnlinkId; }
private:
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
};
