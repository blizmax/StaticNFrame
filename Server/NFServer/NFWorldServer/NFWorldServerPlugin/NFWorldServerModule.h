// -------------------------------------------------------------------------
//    @FileName         :    NFCWorldServerModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFCWorldServerModule
//
// -------------------------------------------------------------------------

#pragma once

#include <NFServer/NFServerCommon/NFIWorldServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"
#include "NFComm/NFCore/NFMapEx.hpp"

class NFCWorldServerModule : public NFIWorldServerModule
{
public:
	explicit NFCWorldServerModule(NFIPluginManager* p);
	virtual ~NFCWorldServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//��Ϸ������ע��Э��ص�
	void OnGameServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnGameServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnGameServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	//���ط�����ע��Э��ص�
	void OnProxyServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnProxyServerUnRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnProxyServerRefreshProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnClientDisconnect(uint32_t unLinkId);

	virtual NF_SHARE_PTR<NFServerData> GetGameByLink(uint32_t unLinkId) { return mLinkGamMap.GetElement(unLinkId); }
private:
	NFMapEx<uint32_t, NFServerData> mGameMap;
	NFMapEx<uint32_t, NFServerData> mLinkGamMap; //linkid -- key
	NFMapEx<uint32_t, NFServerData> mProxyMap;
	uint32_t mServerId;
};
