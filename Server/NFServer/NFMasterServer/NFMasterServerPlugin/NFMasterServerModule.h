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

	//网关服务器注册协议回调
	void OnLoginServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//游戏服务器注册协议回调
	void OnWorldServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFINetServerModule* m_pNetServerModule;
};
