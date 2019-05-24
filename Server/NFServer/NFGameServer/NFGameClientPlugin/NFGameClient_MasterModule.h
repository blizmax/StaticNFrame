// -------------------------------------------------------------------------
//    @FileName         :    NFCGameClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIGameClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFServer/NFServerCommon/NFIGameClient_WorldModule.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

class NFCGameClient_MasterModule : public NFIGameClient_MasterModule
{
public:
	explicit NFCGameClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCGameClient_MasterModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void SetOnlineNum(uint32_t num) { m_onlineNum = num; }
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
	void ServerReport();

	void OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
	uint32_t m_onlineNum;
};


