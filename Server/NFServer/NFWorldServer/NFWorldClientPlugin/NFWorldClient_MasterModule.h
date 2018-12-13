// -------------------------------------------------------------------------
//    @FileName         :    NFCWorldClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFWorldClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIWorldClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>
#include <NFServer/NFServerCommon/NFIWorldClient_ProxyModule.h>

class NFCWorldClient_MasterModule : public NFIWorldClient_MasterModule
{
public:
	explicit NFCWorldClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCWorldClient_MasterModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	
	void RegisterServer();
	void ServerReport();
	void OnHandleServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFINetClientModule* m_pNetClientModule;
	NFIWorldClient_ProxyModule* m_pWorldClient_ProxyModule;
	uint32_t m_unLinkId;
};
