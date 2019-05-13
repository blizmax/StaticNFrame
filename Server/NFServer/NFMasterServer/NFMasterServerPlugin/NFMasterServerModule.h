// -------------------------------------------------------------------------
//    @FileName         :    NFCMasterServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCMasterServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIMasterServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"

class NFCMasterServerModule : public NFIMasterServerModule
{
public:
	explicit NFCMasterServerModule(NFIPluginManager* p);
	virtual ~NFCMasterServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//登录服务器注册协议回调
	void OnLoginServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnLoginServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnLoginServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//世界服务器注册协议回调
	void OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnWorldServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnWorldServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//网关服务器注册协议回调
	void OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//游戏服务器注册协议回调
	void OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void SynServerToOthers(NF_SHARE_PTR<NFServerData> pServerData);

	void OnClientDisconnect(uint32_t unLinkId);

	virtual bool HttpHandleHttpGm(uint32_t linkId, const NFHttpHandle& req);
private:
	NFINetServerModule* m_pNetServerModule;
	NFIHttpServerModule* m_pHttpServerModule;
	NFIKernelModule* m_pKernelModule;
	NFIAsyMysqlModule* m_pAsyMysqlModule;
private:
	NFMapEx<int, NFServerData> mLoginMap;
	NFConsistentHashMapEx<int, NFServerData> mWorldMap;
	NFConsistentHashMapEx<int, NFServerData> mProxyMap;
	NFConsistentHashMapEx<int, NFServerData> mGameMap;
};
