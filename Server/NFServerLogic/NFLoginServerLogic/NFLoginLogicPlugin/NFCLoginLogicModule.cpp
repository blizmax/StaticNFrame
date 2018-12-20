

#include "NFCLoginLogicModule.h"
#include "NFComm/NFPluginModule/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"
#include "NFComm/NFCore/NFJson.h"
#include "NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFMD5.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFComm/NFCore/NFStringUtility.h"

NFCLoginLogicModule::NFCLoginLogicModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCLoginLogicModule::~NFCLoginLogicModule()
{

}

bool NFCLoginLogicModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pMongoModule = pPluginManager->FindModule<NFIMongoModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLoginClient_MasterModule = pPluginManager->FindModule<NFILoginClient_MasterModule>();
	m_pHttpServerModule = pPluginManager->FindModule<NFIHttpServerModule>();

	m_pHttpServerModule->AddRequestHandler(NF_ST_LOGIN, "/httplogin", NFHttpType::NF_HTTP_REQ_POST, this, &NFCLoginLogicModule::HttpHandleHttpLogin);

	m_pNetClientModule->AddReceiveCallBack(NF_ST_MASTER, EGMI_NET_MASTER_TO_LOGIN_PLAT_LOGIN, this, &NFCLoginLogicModule::OnHandleMasterAccountLoginReturn);

	m_pMongoModule->AddMongoServer(NF_ST_LOGIN, "mongodb://14.17.104.12:28900", "ttr-1");
	m_pMongoModule->CreateCollection(NF_ST_LOGIN, ACCOUNT_TABLE, ACCOUNT_TABLE_KEY);
	return true;
}

bool NFCLoginLogicModule::AfterInit()
{
	return true;
}

bool NFCLoginLogicModule::Execute()
{
	return true;
}

bool NFCLoginLogicModule::BeforeShut()
{
	return true;
}

bool NFCLoginLogicModule::Shut()
{
	auto pData = m_loginAccountMap.First();
	while (pData)
	{
		NF_SAFE_DELETE(pData);
		pData = m_loginAccountMap.Next();
	}
	m_loginAccountMap.ClearAll();
	return true;
}

void NFCLoginLogicModule::OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, NFEventContext* pEventContext)
{

}

bool NFCLoginLogicModule::HttpHandleHttpLogin(uint32_t linkId, const NFHttpRequest& req)
{
	NFMsg::http_login_cmd cmd;
	std::string jsonMsg = req.bodySlice.ToString();
	if (NFServerCommon::JsonStringToMessage(jsonMsg, cmd) == false)
	{
		return false;
	}

	if (cmd.do_() == "request-zone-list")
	{
		NFMsg::reqeust_zone_list httpLogin;
		bool ret = NFServerCommon::JsonStringToMessage(jsonMsg, httpLogin);
		if (ret == false)
		{
			NFLogError("json error | {}", jsonMsg);
			return false;
		}
		RequestZoneList(req, httpLogin);
	}
	else if (cmd.do_() == "plat-token-login")
	{
		NFMsg::plat_token_login_request httpLogin;
		bool ret = NFServerCommon::JsonStringToMessage(jsonMsg, httpLogin);
		if (ret == false)
		{
			NFLogError("json error | {}", jsonMsg);
			return false;
		}
		PlatTokenLogin(req, httpLogin);
	}
	else if (cmd.do_() == "request-select-zone")
	{
		NFMsg::reqeust_select_zone_request httpLogin;
		bool ret = NFServerCommon::JsonStringToMessage(jsonMsg, httpLogin);
		if (ret == false)
		{
			NFLogError("json error | {}", jsonMsg);
			return false;
		}
		RequestSelectZone(req, httpLogin);
	}

	return true;
}

void NFCLoginLogicModule::RequestZoneList(const NFHttpRequest& req, const NFMsg::reqeust_zone_list& httpLogin)
{
	uint64_t nowTime = pPluginManager->GetNowTime() / 1000;
	NFMsg::reqeust_zone_list_respone respone;
	respone.set_do_(httpLogin.do_());
	respone.set_error("0");
	respone.set_st(nowTime);

	auto vecServerData = m_pLoginClient_MasterModule->GetAllGameServer();
	for (int i = 0; i < (int)vecServerData.size(); i++)
	{
		auto pServerData = vecServerData[i];
		if (pServerData == nullptr)
		{
			continue;
		}

		auto pServerConfig = NFConfigMgr::Instance()->GetServerConfig(pServerData->mServerInfo.server_id());
		if (pServerConfig == nullptr)
		{
			NFLogError("No Game Server Config!");
			return;
		}

		auto pData = respone.mutable_data();
		if (pData)
		{
			pData->set_bestzoneid(pServerData->mServerInfo.server_id());
			pData->set_gameid(pServerConfig->mWorldId);
			pData->set_gamename(pServerConfig->mServerName);
			pData->set_zoneid(pServerData->mServerInfo.server_id());

			auto pZone = pData->add_zonelist();
			pZone->set_gameid(pServerConfig->mWorldId);
			pZone->set_gamename(pServerConfig->mServerName);
			pZone->set_newzoneid(0);
			pZone->set_onlinenum(pServerData->mServerInfo.server_cur_count());
			pZone->set_opentime("");
			pZone->set_state(pServerData->mServerInfo.server_state());
			pZone->set_zoneid(pServerData->mServerInfo.server_id());
			pZone->set_zonename(pServerData->mServerInfo.server_name());
		}
	}

	std::string responeJson;
	bool ret = NFServerCommon::MessageToJsonString(respone, responeJson);
	if (ret == false)
	{
		return;
	}

	m_pHttpServerModule->ResponseMsg(NF_ST_LOGIN, req, responeJson, NFWebStatus::WEB_OK, "OK");
}

void NFCLoginLogicModule::OnHandleMasterAccountLoginReturn(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
{
	NFMsg::LoginAccount xMsg;
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgId, msg, nLen, xMsg);

	std::string account = xMsg.account();
	std::string openkey = xMsg.openkey();
	uint32_t platid = xMsg.platid();
	uint32_t gameid = xMsg.gameid();
	uint32_t zoneid = xMsg.zoneid();

	uint64_t nowTime = pPluginManager->GetNowTime() / 1000;

	NFMsg::LoginAccount* pAccount = GetLoginAccount(account);
	if (pAccount == nullptr) return;

	NFLogInfo("account:{} uid:{} gameid:{} login......", pAccount->account(), pAccount->uid(), gameid);

	uint32_t plat_login_timeout = 2592000;
	uint64_t uuid = pAccount->uid();

	std::string plat_key = xMsg.md5_plat_key();
	std::string plat_login = NFMD5::md5str(lexical_cast<std::string>(uuid));

	pAccount->set_openkey(openkey);
	pAccount->set_platid(platid);
	pAccount->set_gameid(gameid);
	pAccount->set_zoneid(zoneid);
	pAccount->set_md5_plat_key(plat_key);
	pAccount->set_md5_plat_login(plat_login);

	NFMsg::plat_token_login_respone respone;
	respone.set_gameid(gameid);
	respone.set_unigame_plat_key(plat_key);
	respone.set_unigame_plat_login(plat_login);
	respone.set_unigame_plat_timestamp(nowTime);
	respone.set_zoneid(zoneid);

	respone.set_do_("plat-token-login");
	auto pData = respone.mutable_data();
	pData->set_gameid(gameid);
	pData->set_sid(openkey + "::" + account);
	pData->set_timezone_name("CST");
	pData->set_timezone_offset(28800);
	pData->set_uid(NFCommon::tostr(uuid));
	pData->set_unigame_plat_key(plat_key);
	pData->set_unigame_plat_login(plat_login);
	pData->set_unigame_plat_login_life(plat_login_timeout);
	auto pPlatinfo = pData->mutable_platinfo();
	pPlatinfo->set_aaa(123);
	pPlatinfo->set_account(account);
	pPlatinfo->set_email("");
	pPlatinfo->set_gameid(gameid);
	pPlatinfo->set_gender("");
	pPlatinfo->set_nickname("");
	pPlatinfo->set_platid(0);
	pPlatinfo->set_timestamp(NFCommon::tostr(nowTime));
	pPlatinfo->set_uid(NFCommon::tostr(uuid));

	std::string responeJson;
	bool ret = NFServerCommon::MessageToJsonString(respone, responeJson);
	if (ret == false)
	{
		return;
	}
	//std::string set_url = "/httplogin?unigame_plat_sign=" + NFMD5::md5str(responeJson + NFCommon::tostr(nowTime) + plat_key);
	//NFLogError("last url | {}", set_url);
	m_pHttpServerModule->ResponseMsg(NF_ST_LOGIN, playerId, responeJson, NFWebStatus::WEB_OK, "OK");
}

void NFCLoginLogicModule::PlatTokenLogin(const NFHttpRequest& req, const NFMsg::plat_token_login_request& request)
{
	std::string account = request.data().platinfo().account();
	std::string openkey = request.data().platinfo().sign();
	uint32_t platid = request.data().platinfo().platid();
	uint32_t gameid = request.gameid();
	uint32_t zoneid = request.zoneid();

	NFMsg::LoginAccount* pAccount = GetLoginAccount(account);
	if (pAccount == nullptr) return;

	NFMsg::LoginAccount loginAccount;
	loginAccount.set_uid(pAccount->uid());
	loginAccount.set_account(account);
	loginAccount.set_openkey(openkey);
	loginAccount.set_platid(platid);
	loginAccount.set_gameid(gameid);

	m_pNetClientModule->SendToServerByPB(m_pLoginClient_MasterModule->GetMasterLinkId(), EGMI_NET_LOGIN_TO_MASTER_PLAT_LOGIN, loginAccount, req.requestId);
	return;
}

void NFCLoginLogicModule::RequestSelectZone(const NFHttpRequest& req, const NFMsg::reqeust_select_zone_request& request)
{
	uint64_t uid = NFCommon::strto<long long>(request.uid());
	std::string unigame_plat_login = request.unigame_plat_login();
	uint32_t gameid = request.gameid();
	uint32_t zoneid = request.zoneid();
	uint64_t nowTime = pPluginManager->GetNowTime() / 1000;

	auto pProxyServerData = m_pLoginClient_MasterModule->GetRandProxyServer();
	if (pProxyServerData == nullptr)
	{
		NFLogError("No Proxy Server!");
		return;
	} 

	auto pGameServerData = m_pLoginClient_MasterModule->GetGameServerByServerId(zoneid);
	if (pGameServerData == nullptr)
	{
		NFLogError("Can't find Server by the server id:{}", zoneid);
		return;
	}

	NFMsg::respone_select_zone_respone respone;
	respone.set_do_(request.do_());
	respone.set_errno_("0");
	respone.set_gameid(gameid);
	respone.set_st(nowTime);
	respone.set_uid(request.uid());
	respone.set_unigame_plat_login(unigame_plat_login);
	respone.set_unigame_plat_timestamp(nowTime);
	respone.set_zoneid(zoneid);

	auto* pData = respone.mutable_data();
	pData->set_accountid(uid);
	pData->set_gameid(gameid);
	std::string gatewayurl = NF_FORMAT("http://{}:{}/shen/user", pProxyServerData->mServerInfo.server_ip(), pProxyServerData->mServerInfo.server_port());
	std::string gatewayurltcp = NF_FORMAT("ws://{}:{}", pProxyServerData->mServerInfo.server_ip(), pProxyServerData->mServerInfo.server_port());
	std::string gatewayurlws = NF_FORMAT("ws://{}:{}/shen/user", pProxyServerData->mServerInfo.server_ip(), pProxyServerData->mServerInfo.server_port());
	pData->set_gatewayurl(gatewayurl);
	pData->set_gatewayurltcp(gatewayurltcp);
	pData->set_gatewayurlws(gatewayurlws);
	pData->set_logintempid(0);
	pData->set_tokenid(0);
	pData->set_zoneid(zoneid);
	pData->set_zoneuid(uid);

	std::string responeJson;
	bool ret = NFServerCommon::MessageToJsonString(respone, responeJson);
	if (ret == false)
	{
		return;
	}

	NFStringUtility::Replace(responeJson, "errno_", "errno");
	m_pHttpServerModule->ResponseMsg(NF_ST_LOGIN, req, responeJson, NFWebStatus::WEB_OK, "OK");
}

NFMsg::LoginAccount* NFCLoginLogicModule::GetLoginAccount(const std::string& account)
{
	if (m_loginAccountMap.Count() == 0)
	{
		std::vector<std::string> vec = m_pMongoModule->FindAll(NF_ST_LOGIN, ACCOUNT_TABLE);
		for (int i = 0; i < (int)vec.size(); i++)
		{
			NFMsg::LoginAccount* pAccount = NF_NEW NFMsg::LoginAccount();
			if (NFServerCommon::JsonStringToMessage(vec[i], *pAccount) == false)
			{
				continue;
			}

			m_loginAccountMap.AddElement(account, pAccount);
		}
	}
	auto pAccount = m_loginAccountMap.GetElement(account);
	if (pAccount == nullptr)
	{
		pAccount = NF_NEW NFMsg::LoginAccount();
		pAccount->set_account(account);
		uint64_t uid = m_pKernelModule->GetUUID();
		pAccount->set_uid(uid);

		m_loginAccountMap.AddElement(account, pAccount);

		m_pMongoModule->UpdateOneByKey(NF_ST_LOGIN, ACCOUNT_TABLE, *pAccount, account);
	}

	return pAccount;
}