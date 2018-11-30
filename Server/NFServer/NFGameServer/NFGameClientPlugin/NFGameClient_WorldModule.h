// -------------------------------------------------------------------------
//    @FileName         :    NFGameClient_WorldModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIGameClient_WorldModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>

class NFCGameClient_WorldModule : public NFIGameClient_WorldModule
{
public:
	explicit NFCGameClient_WorldModule(NFIPluginManager* p);

	virtual ~NFCGameClient_WorldModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
private:
	NFINetClientModule* m_pNetClientModule;

	uint32_t m_unLinkId;
};

