// -------------------------------------------------------------------------
//    @FileName         :    NFProxyServerModule.h
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
#include "NFComm/NFCore/NFMap.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

class NFCProxyServerModule : public NFIProxyServerModule
{
public:
	explicit NFCProxyServerModule(NFIPluginManager* p);
	virtual ~NFCProxyServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnProxyInnerSocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleInnerOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//世界服务器注册协议回调
	void OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnWorldServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnWorldServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//游戏服务器注册协议回调
	void OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	virtual NF_SHARE_PTR<NFServerData> GetGameServerData(uint32_t serverId) { return mGameMap.GetElement(serverId); }
	virtual NF_SHARE_PTR<NFServerData> GetWorldServerData(uint32_t serverId) { return mWorldMap.GetElement(serverId); }

	void OnHandleInnerServerDisconnect(uint32_t unLinkId);
private:
	NFIServerNetEventModule* m_pServerNetEventModule;
	NFINetServerModule* m_pNetServerModule;
	NFMapEx<uint32_t, NFServerData> mWorldMap;
	NFMapEx<uint32_t, NFServerData> mGameMap;
};
