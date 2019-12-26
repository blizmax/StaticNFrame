// -------------------------------------------------------------------------
//    @FileName         :    NFCCommonClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCCommonClient_MasterModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFICommonClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"

class NFCCommonClient_MasterModule : public NFICommonClient_MasterModule, public NFEventObj
{
public:
	explicit NFCCommonClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCCommonClient_MasterModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);

	virtual void SetServerType(NF_SERVER_TYPES serverType) override;
	virtual NF_SHARE_PTR<NFServerData> GetMasterServerData() override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
	void ServerReport();

	void OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnHandleHttpMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnHandleGmMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
	NF_SERVER_TYPES mServerType;
};


