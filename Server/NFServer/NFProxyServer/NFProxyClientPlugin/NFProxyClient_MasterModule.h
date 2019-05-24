// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIProxyClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include "NFServer/NFServerCommon/NFIProxyClient_GameModule.h"
#include "NFServer/NFServerCommon/NFIProxyClient_LoginModule.h"
#include "NFServer/NFServerCommon/NFIProxyClient_WorldModule.h"

class NFCProxyClient_MasterModule : public NFIProxyClient_MasterModule
{
public:
	explicit NFCProxyClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCProxyClient_MasterModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
	void ServerReport();
private:
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
};

