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
#include "NFComm/NFPluginModule/NFIMongoModule.h"

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

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFINetServerModule* m_pNetServerModule;
	NFIHttpServerModule* m_pHttpServerModule;
	NFIMongoModule* m_pMongoModule;
};
