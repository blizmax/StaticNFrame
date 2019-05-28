// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFComm/NFPluginModule/NFEventObj.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>

#include "NFServerLogic/NFServerLogicCommon/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

class NFCGameLogicModule : public NFIGameLogicModule
{
public:
	explicit NFCGameLogicModule(NFIPluginManager* p);
	virtual ~NFCGameLogicModule();
public:
	virtual bool Awake() override;

	virtual bool Init() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	/**
	* @brief 处理玩家网关修改
	*
	* @return void
	*/
	void OnHandleChangeProxy(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandlePlayerDisconnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
	void OnHandlePlayerReconnect(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void ChangePlayerGameInfo(uint64_t playerId, uint32_t proxyId);
	void OnHandleProxyEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);
	void OnHandleWorldEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);

	virtual void SendMsgToClientByPlayerId(uint64_t playerId, uint32_t nMsgId, const google::protobuf::Message& xData);
	virtual void SendMsgToWorldByPlayerId(uint64_t playerId, uint32_t nMsgId, const google::protobuf::Message& xData);
private:
	NFMapEx<uint32_t, NFServerData> mProxyMap;	//unlink -- serverdata
	NFMapEx<uint32_t, NFServerData> mWorldMap;	//unlink -- serverdata
	NFMapEx<uint64_t, PlayerGameServerInfo> mPlayerProxyInfoMap;
};
