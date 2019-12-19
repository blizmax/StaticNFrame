// -------------------------------------------------------------------------
//    @FileName         :    NFCLoginServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCLoginServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFILoginServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"

#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFCore/NFMapEx.hpp"

class NFCLoginServerModule : public NFILoginServerModule
{
public:
	explicit NFCLoginServerModule(NFIPluginManager* p);
	virtual ~NFCLoginServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	//登录服务器注册协议回调
	void OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleServerDisconnect(uint32_t unLinkId);
private:
	NFMapEx<uint32_t, NFServerData> mProxyMap;
};
