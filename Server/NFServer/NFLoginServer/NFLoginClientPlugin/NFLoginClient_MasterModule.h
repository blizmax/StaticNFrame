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
#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFCLoginClient_MasterModule : public NFILoginClient_MasterModule, public NFEventObj
{
	/**
	* @brief 定义hash处理器
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

	void OnHandleGmMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void RegisterServer();
	void ServerReport();

	void OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	virtual uint32_t GetMasterLinkId() const { return m_pMasterServerData->mUnlinkId; }
private:
	NF_SHARE_PTR<NFServerData> m_pMasterServerData;
};
