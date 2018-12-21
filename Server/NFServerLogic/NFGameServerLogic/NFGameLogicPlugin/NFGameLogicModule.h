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

#include "NFServerLogic/NFServerLogicCommon/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"

#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

class NFCGameLogicModule : public NFIGameLogicModule
{
public:
	explicit NFCGameLogicModule(NFIPluginManager* p);
	virtual ~NFCGameLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleProxyDisconnect(const uint32_t unLinkId);
	void OnHandleAccountConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleAccountDisConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleAccountReConnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	virtual NF_SHARE_PTR<AccountInfo> GetPlayerAccountInfo(uint64_t uid) { return m_playerAccountInfo.GetElement(uid); }
private:
	NFIServerNetEventModule* m_pServerNetEventModule;
	NFINetClientModule* m_pNetClientModule;
	NFILuaScriptModule* m_pLuaScriptModule;

	NFMapEx<uint64_t, AccountInfo> m_playerAccountInfo;
};
