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

#include "NFServerLogic/NFServerLogicCommon/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFEventObj.h"
#include <NFComm/NFPluginModule/NFIHttpServerModule.h>
#include "NFServer/NFServerCommon/NFILoginClient_MasterModule.h"

#include "NFComm/NFCore/NFJson.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIMongoModule.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFMessageDefine/NFMsgDefine.h"

#define ACCOUNT_TABLE "Account"
#define ACCOUNT_TABLE_KEY "account"

#define UID_TABLE "UidTable"
#define UID_TABLE_KEY "key"

class NFCLoginLogicModule : public NFILoginLogicModule, public NFEventObj
{
public:
	explicit NFCLoginLogicModule(NFIPluginManager* p);
	virtual ~NFCLoginLogicModule();
public:
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool Execute() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext) override;

	virtual bool HttpHandleHttpLogin(uint32_t linkId, const NFHttpRequest& req);

	virtual void RequestZoneList(const NFHttpRequest& req, const NFMsg::reqeust_zone_list& httpLogin);
	virtual void PlatTokenLogin(const NFHttpRequest& req, const NFMsg::plat_token_login_request& request);
	virtual void RequestSelectZone(const NFHttpRequest& req, const NFMsg::reqeust_select_zone_request& json);

	void OnHandleMasterAccountLoginReturn(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);


	NFMsg::LoginAccount* GetLoginAccount(const std::string& account);

	void CreateUidFromDb();
	uint32_t GetUid();
private:
	NFIHttpServerModule* m_pHttpServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFILoginClient_MasterModule* m_pLoginClient_MasterModule;
	NFIKernelModule* m_pKernelModule;
	NFIMongoModule* m_pMongoModule;
	NFMap<std::string, NFMsg::LoginAccount> m_loginAccountMap;

	uint32_t m_curUUID;
	uint32_t m_maxUUID;
};