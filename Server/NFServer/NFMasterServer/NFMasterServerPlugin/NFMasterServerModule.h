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
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"

class NFCMasterServerModule : public NFIMasterServerModule, public NFTimerObj
{
public:
	explicit NFCMasterServerModule(NFIPluginManager* p);
	virtual ~NFCMasterServerModule();

	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnTimer(uint32_t nTimerID) override;
protected:
	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);
	void OnHandleOtherMessage(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnServerRegisterProcess(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnServerReport(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnServerHttpMsgRet(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnServerErrorMsg(const uint32_t unLinkId, const uint64_t playerId, const uint32_t operateId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void SynServerToOthers(NF_SHARE_PTR<NFServerData> pServerData);

	void OnClientDisconnect(uint32_t unLinkId);

	virtual bool HttpHandleHttpGm(uint32_t linkId, const NFIHttpHandle& req);
	virtual bool HttpHandleHttpMsg(uint32_t linkId, const NFIHttpHandle& req);

	void SaveServerDataToDB();
	void ClearServerDataToDB();

	NF_SHARE_PTR<NFServerData> GetServerByServerId(uint32_t serverId);
	void SendMsgToAllServer(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID);
	void SendMsgToServer(uint32_t serverId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID);

private:
	NFMapEx<int, NFServerData> mServerMap;
};
