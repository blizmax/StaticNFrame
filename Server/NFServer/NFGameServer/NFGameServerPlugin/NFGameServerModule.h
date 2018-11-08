// -------------------------------------------------------------------------
//    @FileName         :    NFGameServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameServerPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIGameServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>

class NFCGameServerModule : public NFIGameServerModule
{
public:
	explicit NFCGameServerModule(NFIPluginManager* p);
	virtual ~NFCGameServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void HttpGetCallBack(const int state_code, const std::string& strRespData, const std::string& strUserData);
private:
	NFINetServerModule* m_pNetServerModule;
};
