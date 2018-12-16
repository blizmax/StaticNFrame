// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include <NFServer/NFServerCommon/NFIProxyServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFComm/NFPluginModule/NFEventObj.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>

#include "NFServerLogic/NFServerLogicCommon/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"

#include "NFComm/NFCore/NFMap.hpp"

class ProxyPlayerData
{
public:
	ProxyPlayerData()
	{
		unlinkId = 0;
		gameServerId = 0;
		gameUnlinkId = 0;
		uid = 0;
	}
	uint32_t unlinkId;
	uint32_t gameServerId;
	uint32_t gameUnlinkId;
	uint64_t uid;
	std::string account;
};

class NFCProxyLogicModule : public NFIProxyLogicModule, public NFEventObj
{
public:
	explicit NFCProxyLogicModule(NFIPluginManager* p);
	virtual ~NFCProxyLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) override;

	void OnHandleGmMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleGameJsonMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleJsonMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleUser_LoginTokenLoginUserPmd(const uint32_t unLinkId, const NFMsg::UserLoginTokenLoginUserPmd_C& msg);
	void OnHandleUser_UserJsMessageForwardUserPmd(const uint32_t unLinkId, const NFMsg::UserJsMessageForwardUserPmd_CS& msg);
	void OnHandleUser_UserLoginReconnectLoginUserPmd(const uint32_t unLinkId, const NFMsg::UserLoginReconnectLoginUserPmd_C& msg);

	ProxyPlayerData* GetPlayerData(uint64_t uid) { return mPlayerData.GetElement(uid); }
	ProxyPlayerData* GetPlayerDataByLinkId(uint32_t unlinkId) { return mUnlinkIdPlayerData.GetElement(unlinkId); }

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnAccountDisconnect(const uint32_t unLinkId);
	void OnHandleInnerServerDisconnect(uint32_t serverType, uint32_t serverId);
private:
	NFINetServerModule* m_pNetServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFIProxyServerModule* m_pNetProxyServerModule;

	NFMap<uint64_t, ProxyPlayerData> mPlayerData;
	NFMap<uint32_t, ProxyPlayerData> mUnlinkIdPlayerData;
};