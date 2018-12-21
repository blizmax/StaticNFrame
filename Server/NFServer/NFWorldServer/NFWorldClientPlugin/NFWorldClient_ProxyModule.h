// -------------------------------------------------------------------------
//    @FileName         :    NFCGameClient_ProxyModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIWorldClient_ProxyModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include <NFComm/NFCore/NFMap.hpp>

class NFCWorldClient_ProxyModule : public NFIWorldClient_ProxyModule
{
public:
	explicit NFCWorldClient_ProxyModule(NFIPluginManager* p);

	virtual ~NFCWorldClient_ProxyModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer(uint32_t unlinkId);
	void OnClientDisconnect(uint32_t unLinkId);

	virtual void OnHandleProxyReport(const NFMsg::ServerInfoReport& xData);

	NF_SHARE_PTR<NFServerData> GetServerByServerId(uint32_t serverId) { return mProxyMap.GetElement(serverId); }
	NF_SHARE_PTR<NFServerData> GetServerByUnlinkId(uint32_t unlinkId) { return mUnlinkProxyMap.GetElement(unlinkId); }
private:
	NFINetClientModule* m_pNetClientModule;
	NFIServerNetEventModule* m_pServerNetEventModule;

	NFMapEx<uint32_t, NFServerData> mProxyMap;	//serverid -- serverdata
	NFMapEx<uint32_t, NFServerData> mUnlinkProxyMap; //unlinkid -- serverdata
};



