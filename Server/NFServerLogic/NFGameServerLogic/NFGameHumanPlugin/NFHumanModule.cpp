// -------------------------------------------------------------------------
//    @FileName         :    NFHumanModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFHumanModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"
#include "NFComm/NFCore/NFCommon.h"

NFHumanModule::NFHumanModule(NFIPluginManager* p):NFIDynamicModule(p)
{
	m_pPluginManager = p;
}

NFHumanModule::~NFHumanModule()
{

}

bool NFHumanModule::Init()
{
	return true;
}

void NFHumanModule::CopyFromDB(NFMsg::playerinfo* pInfo, NFMsg::db_playerinfo* pDbInfo)
{
	if (pInfo == nullptr || pDbInfo == nullptr) return;

	pInfo->set_userid(pDbInfo->userid());
	pInfo->set_account(pDbInfo->account());
	pInfo->set_password(pDbInfo->password());
	pInfo->set_nickname(pDbInfo->nickname());
	pInfo->set_face_1(pDbInfo->face_1());
	pInfo->set_face_2(pDbInfo->face_2());
	pInfo->set_face_3(pDbInfo->face_3());
	pInfo->set_face_4(pDbInfo->face_4());
	pInfo->set_regdate(pDbInfo->regdate());

	pInfo->set_sex(pDbInfo->sex());
	pInfo->set_age(pDbInfo->age());
	pInfo->set_email(pDbInfo->email());

	pInfo->set_phonenum(pDbInfo->phonenum());
	pInfo->set_exp(pDbInfo->exp());
	pInfo->set_level(pDbInfo->level());
	pInfo->set_viplevel(pDbInfo->viplevel());
	pInfo->set_jetton(pDbInfo->jetton());

	pInfo->set_gold(pDbInfo->gold());
	pInfo->set_money(pDbInfo->money());

	pInfo->set_channel(pDbInfo->channel());
	pInfo->set_province(pDbInfo->province());
	pInfo->set_city(pDbInfo->city());

	pInfo->set_popularity(pDbInfo->popularity());
	pInfo->set_idenstatus(pDbInfo->idenstatus());
	pInfo->set_bindtype(pDbInfo->bindtype());

	pInfo->set_lasttime(pDbInfo->lasttime());
	pInfo->set_penulttime(pDbInfo->penulttime());
	pInfo->set_silent(pDbInfo->silent());
	pInfo->set_isban(pDbInfo->isban());
	pInfo->set_isrobot(pDbInfo->isrobot());
	pInfo->set_playtype(pDbInfo->playtype());
	pInfo->set_description(pDbInfo->description());
	pInfo->set_blacklist(pDbInfo->blacklist());

	pInfo->set_propertyid(pDbInfo->propertyid());
	pInfo->set_agencystatus(pDbInfo->agencystatus());
	pInfo->set_location("online");

	pInfo->set_registration1(pDbInfo->registration1());
	pInfo->set_registration2(pDbInfo->registration2());
	pInfo->set_registration3(pDbInfo->registration3());

	pInfo->set_signinmonth(pDbInfo->signinmonth());
	pInfo->set_bsignin(pDbInfo->bsignin());
	pInfo->set_turntablenum(pDbInfo->turntablenum());
	pInfo->set_tengames(pDbInfo->tengames());

	pInfo->set_todayshare(pDbInfo->todayshare());
	pInfo->set_caninvite(pDbInfo->caninvite());
	pInfo->set_totalgames(0);
}

uint32_t NFHumanModule::LoadPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo)
{
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "function param error, pInfo == nullptr, account:{}, password:{}", account, password);
		return RETURN_CODE_ACCOUNT_NO_EXIST;
	}

	NFIMysqlModule* pMysqlModule = m_pPluginManager->FindModule<NFIMysqlModule>();
	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_account(account);
	bool ret = pMysqlModule->Query(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "ÕËºÅ²»´æÔÚ,account=" + account);
		return RETURN_CODE_ACCOUNT_NO_EXIST;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	if (pDbInfo->password() != password)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "µÇÂ¼ÃÜÂë²»Æ¥Åä,password=" + password);
		return RETURN_CODE_PASSWORD_NOT_MATCH;
	}

	NFHumanModule::CopyFromDB(pInfo, pDbInfo);

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	NFMsg::accountinfo accountInfo;
	accountInfo.set_account(account);
	accountInfo.set_password(password);
	accountInfo.set_userid(pInfo->userid());

	std::string strValue;
	if (accountInfo.SerializeToString(&strValue))
	{
		if (pNosqlDriver->Set("account" + pInfo->account(), strValue) == false)
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "Nosql set account failed, pInfo:{}", accountInfo.DebugString());
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "accountInfo.SerializeToString error:{}", accountInfo.DebugString());
	}
	
	if (pInfo->SerializeToString(&strValue))
	{
		if (pNosqlDriver->Set("playerinfo" + NFCommon::tostr(pInfo->userid()), strValue) == false)
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "Nosql set playerinfo failed, pInfo:{}", pInfo->DebugString());
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "pInfo.SerializeToString error:{}", pInfo->DebugString());
	}

	return RETURN_CODE_SUCCESS;
}

uint32_t NFHumanModule::GetPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo)
{
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "function param error, pInfo == nullptr, account:{}, password:{}", account, password);
		return RETURN_CODE_ACCOUNT_NO_EXIST;
	}

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	std::string strUserID;
	if (pNosqlDriver->Get("account" + account, strUserID))
	{
		NFMsg::accountinfo accountInfo;
		if (accountInfo.ParseFromString(strUserID))
		{
			if (accountInfo.password() != password)
			{
				NFBehaviorLog(0, account, "player", "GetPlayerInfoByCID", RETURN_CODE_PASSWORD_NOT_MATCH, "µÇÂ¼ÃÜÂë²»Æ¥Åä,password=" + password);
				return RETURN_CODE_PASSWORD_NOT_MATCH;
			}

			if (GetPlayerInfo(accountInfo.userid(), pInfo) == RETURN_CODE_SUCCESS)
			{
				return RETURN_CODE_SUCCESS;
			}

			return RETURN_CODE_SUCCESS;
		}
	}

	return LoadPlayerInfoByCID(account, password, pInfo);
}

uint32_t  NFHumanModule::GetPlayerInfo(uint32_t playerId, NFMsg::playerinfo* pInfo)
{
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, playerId, "function param error");
		return RETURN_CODE_ACCOUNT_NO_EXIST;
	}

	if (playerId == 0)
	{
		return 0;
	}

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	std::string strValue;
	if (pNosqlDriver->Get("playerinfo" + NFCommon::tostr(playerId), strValue))
	{
		if (pInfo->ParseFromString(strValue))
		{
			return RETURN_CODE_SUCCESS;
		}
	}

	return LoadPlayerInfo(playerId, pInfo);
}

uint32_t NFHumanModule::LoadPlayerInfo(uint32_t playerId, NFMsg::playerinfo* pInfo)
{
	NFIMysqlModule* pMysqlModule = m_pPluginManager->FindModule<NFIMysqlModule>();
	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_userid(NFCommon::tostr(playerId));
	bool ret = pMysqlModule->Query(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(playerId, "", "player", "LoadPlayerInfoByCID", -1, "¼ÓÔØÊý¾Ý¿âÍæ¼ÒÐÅÏ¢Ê§°Ü");
		return RETURN_CODE_ACCOUNT_NO_EXIST;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	if (pInfo == nullptr)
	{
		pInfo = new NFMsg::playerinfo();
	}

	NFHumanModule::CopyFromDB(pInfo, pDbInfo);

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();


	if (pNosqlDriver->Set("cidinfo" + pInfo->cid(), NFCommon::tostr(pInfo->userid())) == false)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "nosql set cidinfo failed:{}", pInfo->userid());
	}

	std::string strValue;
	if (pInfo->SerializeToString(&strValue))
	{
		if (pNosqlDriver->Set("playerinfo" + NFCommon::tostr(pInfo->userid()), strValue) == false)
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "nosql set playerifo failed:{}", pInfo->DebugString());
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pInfo->userid(), "pInfo.SerializeToString error:{}", pInfo->DebugString());
	}

	return RETURN_CODE_SUCCESS;
}