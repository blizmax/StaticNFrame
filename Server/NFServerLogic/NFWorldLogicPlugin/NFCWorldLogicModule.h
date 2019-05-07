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

#include "NFServerLogic/NFServerLogicCommon/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

#include "NFComm/NFCore/NFMap.hpp"

class NFCWorldLogicModule : public NFIWorldLogicModule
{
public:
	explicit NFCWorldLogicModule(NFIPluginManager* p);
	virtual ~NFCWorldLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnHandleMessageFromServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	
	void OnHandleAccountLoginFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleAccountLoginFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnHandleGameEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);
	void OnHandleProxyEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);

	NF_SHARE_PTR<NFServerData> FindGameServerByServerId(uint32_t serverId);
	NF_SHARE_PTR<NFServerData> FindProxyServerByServerId(uint32_t serverId);
private:
	NFINetServerModule* m_pNetServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFIServerNetEventModule* m_pServerNetEventModule;
	NFMapEx<uint32_t, NFServerData> mGameMap;	//serverid -- serverdata
	NFMapEx<uint32_t, NFServerData> mProxyMap;	//serverid -- serverdata
	NFMapEx<std::string, PlayerWorldServerInfo> mPlayerInfoByAccount; //account -- PlayerWorldServerInfo
	NFMapEx<uint64_t, PlayerWorldServerInfo> mPlayerInfoByPlayerId; //playerId -- PlayerWorldServerInfo
};