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

NFHumanModule::NFHumanModule(NFIPluginManager* p):NFIDynamicModule(p)
{
	m_pPluginManager = p;
}

NFHumanModule::~NFHumanModule()
{

}

bool NFHumanModule::Init()
{
	LoadPlayerInfoByCID("name115", "pwd115", nullptr);
	return true;
}

uint32_t NFHumanModule::LoadPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo)
{
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

	if (pInfo == nullptr)
	{
		pInfo = new NFMsg::playerinfo();
	}

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

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	NFMsg::accountinfo accountInfo;
	accountInfo.set_account(account);
	accountInfo.set_password(password);
	accountInfo.set_userid(pInfo->userid());

	std::string strValue;
	if (accountInfo.SerializeToString(&strValue))
	{
		pNosqlDriver->Set("account" + pInfo->account(), strValue);
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "accountInfo.SerializeToString error:{}", accountInfo.DebugString());
	}
	
	if (pInfo->SerializeToString(&strValue))
	{
		pNosqlDriver->Set("playerinfo" + pInfo->userid(), strValue);
	}
	else
	{
		NFLogError(NF_LOG_SYSTEMLOG, 0, "pInfo.SerializeToString error:{}", accountInfo.DebugString());
	}

	return RETURN_CODE_SUCCESS;
}

uint32_t NFHumanModule::GetPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo)
{
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
	if (playerId < 1000)
	{
		return 0;
	}
	return RETURN_CODE_SUCCESS;
}