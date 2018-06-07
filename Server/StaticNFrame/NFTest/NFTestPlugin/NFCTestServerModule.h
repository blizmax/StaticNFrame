// -------------------------------------------------------------------------
//    @FileName         :    NFCTestServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/04
//    @Email			:    445267987@qq.com
//    @Module           :    NFTestPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include <NFComm/NFPluginModule/NFIModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"

#include <unordered_map>
#include <memory>

class NFINetServerModule;

class TestPlayerData
{
public:
	TestPlayerData()
	{
		m_usLinkId = 0;
	}
	uint32_t m_usLinkId;
	std::string m_account;
};

class NFCTestServerModule : public NFIModule
{
public:
	explicit NFCTestServerModule(NFIPluginManager* p);
	virtual ~NFCTestServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandleQueryPlayerData(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFINetServerModule* m_pNetServerModule;
	std::unordered_map<uint32_t, std::string> m_unLinkIdToAccount;
	std::unordered_map<std::string, TestPlayerData*> m_playerDataMap;
};