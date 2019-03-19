// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/3/19
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIProxyClient_GameModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

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

	void RegisterServer(uint32_t linkId);
	void OnClientDisconnect(uint32_t unLinkId);

	virtual void OnHandleServerReport(const NFMsg::ServerInfoReport& xData);

	NF_SHARE_PTR<NFServerData> GetServerByServerId(uint32_t serverId) { return mGameMap.GetElement(serverId); }
	NF_SHARE_PTR<NFServerData> GetServerByUnlinkId(uint32_t unlinkId) { return mUnlinkGameMap.GetElement(unlinkId); }
private:
	NFINetClientModule* m_pNetClientModule;
	NFIServerNetEventModule* m_pServerNetEventModule;

	NFMapEx<uint32_t, NFServerData> mGameMap;	//serverid -- serverdata
	NFMapEx<uint32_t, NFServerData> mUnlinkGameMap; //unlinkid -- serverdata
};


