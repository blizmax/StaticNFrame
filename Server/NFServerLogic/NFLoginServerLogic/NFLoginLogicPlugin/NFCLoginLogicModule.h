// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFComm/NFPluginModule/NFEventObj.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>

#include "NFServerLogic/NFServerLogicCommon/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include <NFComm/NFPluginModule/NFIHttpServerModule.h>

#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFMessageDefine/NFMsgDefine.h"

class NFCLoginLogicModule : public NFILoginLogicModule
{
public:
	explicit NFCLoginLogicModule(NFIPluginManager* p);
	virtual ~NFCLoginLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleOtherMainSubMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint16_t nMainMsgId, const uint16_t nSubMsgId, const char* msg, const uint32_t nLen);

	virtual void OnHandlePlayerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	virtual void OnHandlePlayerDisConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnHandleProxyEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);

	void OnHandleAccountLoginFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandlePlayerDisconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandlePlayerReconnectFromProxyServer(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFMapEx<uint32_t, NFServerData> mProxyMap;	//unlinkid -- serverdata
	NFMapEx<uint64_t, PlayerGameServerInfo> mPlayerProxyInfoMap; //playerId -- PlayerWorldServerInfo
};