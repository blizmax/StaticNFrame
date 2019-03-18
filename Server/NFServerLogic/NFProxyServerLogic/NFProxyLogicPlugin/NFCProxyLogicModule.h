// -------------------------------------------------------------------------
//    @FileName         :    NFProxyLogicModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/12
//    @Email			:    445267987@qq.com
//    @Module           :    NFProxyServerPlugin
//
// -------------------------------------------------------------------------

#pragma once
#include <NFServer/NFServerCommon/NFIProxyServerModule.h>
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include <NFComm/NFPluginModule/NFINetServerModule.h>
#include <NFComm/NFPluginModule/NFEventObj.h>
#include <NFComm/NFPluginModule/NFINetClientModule.h>

#include "NFServerLogic/NFServerLogicCommon/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include "NFComm/NFPluginModule/NFIServerNetEventModule.h"

#include "NFComm/NFCore/NFMap.hpp"

class ProxyPlayerData
{
public:
	ProxyPlayerData()
	{
		gameId = 0;
		unlinkId = 0;
		gameServerId = 0;
		gameUnlinkId = 0;
		uid = 0;
	}
	uint32_t unlinkId;		//玩家连接ID
	uint32_t gameServerId;	//玩家连接点逻辑服务器
	uint32_t gameId;		//玩家所在游戏ID
	uint32_t gameUnlinkId;	//玩家连接逻辑服务器连接ID
	uint64_t uid;
	std::string account;
};

class NFCProxyLogicModule : public NFIProxyLogicModule
{
public:
	explicit NFCProxyLogicModule(NFIPluginManager* p);
	virtual ~NFCProxyLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	void OnProxySocketEvent(const eMsgType nEvent, const uint32_t unLinkId);

	void OnHandleGameEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);
	void OnHandleWorldEventCallBack(eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData);
private:
	NFINetServerModule* m_pNetServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFIProxyServerModule* m_pNetProxyServerModule;
	NFIServerNetEventModule* m_pServerNetEventModule;
};