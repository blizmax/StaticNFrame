// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIProxyClient_GameModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCProxyClient_GameModule : public NFIProxyClient_GameModule
{
public:
	explicit NFCProxyClient_GameModule(NFIPluginManager* p);

	virtual ~NFCProxyClient_GameModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//
	void OnHandleWorldSendGameMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer(uint32_t unLinkId);
	void OnClientDisconnect(uint32_t unLinkId);

	virtual NF_SHARE_PTR<NFServerData> GetServerData(uint32_t serverId) override;
private:
	NFINetClientModule* m_pNetClientModule;

	NFMapEx<uint32_t, NFServerData> mGameMap;
	std::map<uint32_t, uint32_t> mLinkIdMap;
};
