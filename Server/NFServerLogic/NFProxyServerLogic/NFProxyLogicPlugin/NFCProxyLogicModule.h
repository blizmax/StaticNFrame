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
#include "NFServer/NFServerCommon/NFIProxyClient_GameModule.h"

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

	void OnHandleJsonMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFINetClientModule* m_pNetClientModule;
	NFINetServerModule* m_pNetServerModule;
	NFIProxyClient_GameModule* m_pProxyClient_GameModule;
	uint32_t m_worldServerUnlinkId;
};