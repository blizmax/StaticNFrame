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
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFCore/NFMap.hpp"

class NFCProxyLogicModule : public NFIProxyLogicModule, public NFTimerObj
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

	virtual void OnTimer(uint32_t nTimerID) override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleMessageFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleMessageFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleMessageFromWorldServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleMessageFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	
	void OnHandleAccountLoginFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleReconnectFromLoginServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnHandlePacketMsgFromGameServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	
	void OnHandleAccountLoginFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleReconnectFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleEnterTableFromClient(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	/**
	* @brief ��������
	*
	* @return void
	*/
	void OnHandleHeartBeat(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnHandleGameEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);
	void OnHandleWorldEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);
	void OnHandleLoginEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);

	NF_SHARE_PTR<NFServerData> GetGameServerByServerId(uint32_t serverId);
	NF_SHARE_PTR<NFServerData> GetWorldServerByServerId(uint32_t serverId);
private:
	NFConsistentHashMapEx<uint64_t, NFServerData> mGameMap;	//unlink -- serverdata
	NFConsistentHashMapEx<uint64_t, NFServerData> mWorldMap;	//unlink -- serverdata
	NFConsistentHashMapEx<uint64_t, NFServerData> mLoginMap;	//unlink -- serverdata
	NFMapEx<uint32_t, ProxyLinkInfo> mClientLinkInfo; //unlink -- proxyLinkInfo
	NFMapEx<uint64_t, ProxyLinkInfo> mPlayerLinkInfo; //playerId -- proxyLinkInfo
};