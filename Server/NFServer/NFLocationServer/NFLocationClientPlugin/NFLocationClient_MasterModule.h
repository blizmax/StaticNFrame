// -------------------------------------------------------------------------
//    @FileName         :    NFCLocationClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLocationClient_MasterModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFILocationClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCLocationClient_MasterModule : public NFILocationClient_MasterModule
{
public:
	explicit NFCLocationClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCLocationClient_MasterModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnHandleGmMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
	void ServerReport();

	void OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen) {}

	virtual uint32_t GetMasterLinkId() const { return m_pMasterServerData->mUnlinkId; }
private:
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
};
