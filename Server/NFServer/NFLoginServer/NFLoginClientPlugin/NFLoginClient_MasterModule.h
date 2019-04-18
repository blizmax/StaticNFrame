// -------------------------------------------------------------------------
//    @FileName         :    NFProxyClient_GameModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyClientPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFILoginClient_MasterModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include <NFComm/NFPluginModule/NFServerDefine.h>

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCLoginClient_MasterModule : public NFILoginClient_MasterModule
{
	/**
	* @brief ����hash������
	*/
	using hash_functor = std::function<size_t(const std::string&)>;
public:
	explicit NFCLoginClient_MasterModule(NFIPluginManager* p);

	virtual ~NFCLoginClient_MasterModule();

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

	void OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	virtual NF_SHARE_PTR<NFServerData> GetSuitProxyServer(const std::string& name) override;
	virtual NF_SHARE_PTR<NFServerData> GetSuitGameServer(const std::string& name) override;
	virtual NF_SHARE_PTR<NFServerData> GetSuitWorldServer() override;
	virtual NF_SHARE_PTR<NFServerData> GetRandProxyServer() override;
	virtual NF_SHARE_PTR<NFServerData> GetRandGameServer() override;
	virtual NF_SHARE_PTR<NFServerData> GetProxyServerByServerId(uint32_t serverId) override;
	virtual NF_SHARE_PTR<NFServerData> GetGameServerByServerId(uint32_t serverId) override;

	virtual uint32_t GetMasterLinkId() const { return m_pMasterServerData->mUnlinkId; }
private:
	NFINetClientModule* m_pNetClientModule;
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
	NFMapEx<int, NFServerData> mWorldMap;
	NFConsistentHashMapEx<int, NFServerData> mProxyMap;
	NFConsistentHashMapEx<int, NFServerData> mGameMap;
	/**
	* hashֵ���㹫ʽ
	*/
	hash_functor                mhashf;
};
