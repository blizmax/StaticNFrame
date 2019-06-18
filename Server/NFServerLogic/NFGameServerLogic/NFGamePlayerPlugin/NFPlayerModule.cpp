// -------------------------------------------------------------------------
//    @FileName         :    NFHumanModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFPlayerModule.h"

#include "NFServerLogic/NFServerLogicCommon/NFPlayerDefine.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"
#include "NFComm/NFPluginModule/NFCObject.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFServerLogic/NFServerLogicCommon/NFIGameConfigModule.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"

#include "NFMessageDefine/NFNodeClass.h"
#include "NFMessageDefine/NFNodeClassName.h"

NFCPlayerModule::NFCPlayerModule(NFIPluginManager* p):NFIPlayerModule(p)
{

}

NFCPlayerModule::~NFCPlayerModule()
{

}

bool NFCPlayerModule::Init()
{
	return true;
}

NFIObject* NFCPlayerModule::CreatePlayerObject(NFMsg::db_playerinfo* pInfo)
{
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "function param error, pInfo == nullptr");
		return nullptr;
	}

	NFIObject* pObject = FindModule<NFIKernelModule>()->CreateNFObject(pInfo->userid(), NF_NODE_STRING_CLASS_NAME_PLAYER);
	if (pObject)
	{
		if (NFProtobufCommon::NFObjectFromMessage(pObject, *pInfo))
		{
			NFLogInfo(NF_LOG_LOGIN_MODULE_LOG, 0, "create player success, userid:{}", pInfo->userid());
			return pObject;
		}
		else
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "NFProtobufCommon::NFObjectFromMessage create player failed, userid:{}", pInfo->userid());
			return nullptr;
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "pKernelModule->CreateNFObject create player failed, userid:{}", pInfo->userid());
		return nullptr;
	}
}

NFIObject* NFCPlayerModule::GetPlayerObject(uint64_t playerId)
{
	NFIObject* pObject = FindModule<NFIKernelModule>()->GetNFObject(playerId, NF_NODE_STRING_CLASS_NAME_PLAYER);
	if (pObject)
	{
		return pObject;
	}

	NFLogWarning(NF_LOG_LOGIN_MODULE_LOG, playerId, "Get Player Failed!, not exist player:{} where the Node 'ClassName' is player", playerId);
	return nullptr;
}

NFIObject* NFCPlayerModule::LoadPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;

	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_account(account);
	bool ret = FindModule<NFIMysqlModule>()->QueryOne(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "账号不存在,account=" + account);
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	if (pDbInfo->password() != password)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "登录密码不匹配,password=" + password);
		retCode = RETURN_CODE_PASSWORD_NOT_MATCH;
		return pObject;
	}

	pObject = NFCPlayerModule::CreatePlayerObject(pDbInfo);
	if (pObject == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "NFCHumanModule::CreatePlayerObject failed:{}", pDbInfo->DebugString());
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFMsg::accountinfo accountInfo;
	accountInfo.set_account(account);
	accountInfo.set_password(password);
	accountInfo.set_userid(pDbInfo->userid());

	std::string strValue;
	if (accountInfo.SerializeToString(&strValue))
	{
		if (FindModule<NFINoSqlModule>()->Set("account" + pDbInfo->account(), strValue) == false)
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "Nosql set account failed, pInfo:{}", accountInfo.DebugString());
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "accountInfo.SerializeToString error:{}", accountInfo.DebugString());
	}
	
	if (pDbInfo->SerializeToString(&strValue))
	{
		if (FindModule<NFINoSqlModule>()->Set("playerinfo" + NFCommon::tostr(pDbInfo->userid()), strValue) == false)
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "Nosql set playerinfo failed, pInfo:{}", pDbInfo->DebugString());
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "pInfo.SerializeToString error:{}", pDbInfo->DebugString());
	}

	retCode = RETURN_CODE_SUCCESS;
	return pObject;
}

NFIObject* NFCPlayerModule::GetPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;

	std::string strUserID;
	if (FindModule<NFINoSqlModule>()->Get("account" + account, strUserID))
	{
		NFMsg::accountinfo accountInfo;
		if (accountInfo.ParseFromString(strUserID))
		{
			if (accountInfo.password() != password)
			{
				NFBehaviorLog(0, account, "player", "GetPlayerInfoByCID", RETURN_CODE_PASSWORD_NOT_MATCH, "登录密码不匹配,password=" + password);
				retCode = RETURN_CODE_PASSWORD_NOT_MATCH;
				return pObject;
			}

			pObject = GetPlayerInfo(accountInfo.userid(), retCode);
			if (pObject)
			{
				retCode = RETURN_CODE_SUCCESS;
				return pObject;
			}
		}
	}

	return LoadPlayerInfoByCID(account, password, retCode);
}

NFIObject*  NFCPlayerModule::GetPlayerInfo(uint64_t playerId, uint32_t& retCode)
{
	NFIObject* pObject = GetPlayerObject(playerId);
	if (pObject)
	{
		retCode = RETURN_CODE_SUCCESS;
		return pObject;
	}

	std::string strValue;
	if (FindModule<NFINoSqlModule>()->Get("playerinfo" + NFCommon::tostr(playerId), strValue))
	{
		NFMsg::db_playerinfo dbInfo;
		if (dbInfo.ParseFromString(strValue))
		{
			pObject = CreatePlayerObject(&dbInfo);
			if (pObject == nullptr)
			{
				NFLogError(NF_LOG_LOGIN_MODULE_LOG, dbInfo.userid(), "NFCHumanModule::CreatePlayerObject failed:{}", dbInfo.DebugString());
				retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
				return pObject;
			}
			retCode = RETURN_CODE_SUCCESS;
			return pObject;
		}
	}

	return LoadPlayerInfo(playerId, retCode);
}

std::string NFCPlayerModule::GetInitFaceID()
{
	std::string initFace = GetGlobalConfigObject()->GetNodeString(GAME_CONFIG_INIT_FACE);
	std::vector<std::string> vecInitFace;
	NFStringUtility::Split(vecInitFace, initFace, ",");

	if (vecInitFace.size() <= 0)
	{
		return std::string();
	}

	int index = NFRandInt(0, (int)vecInitFace.size());
	return vecInitFace[index];
}

void NFCPlayerModule::CreatePlayer(const NFMsg::cgaccountlogin& cgMsg)
{
	NFMsg::db_query_playerinfo db_playerinfo;
	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();
	pDbInfo->set_cid(cgMsg.cid());
	pDbInfo->set_account(cgMsg.account());
	pDbInfo->set_password(cgMsg.password());
	pDbInfo->set_nickname(cgMsg.nickname());
	pDbInfo->set_channel(cgMsg.channel());
	pDbInfo->set_province(cgMsg.province());
	pDbInfo->set_city(cgMsg.city());
	pDbInfo->set_bindtype(cgMsg.bindtype());
	pDbInfo->set_bindnick(cgMsg.bindnick());
	pDbInfo->set_platformid(cgMsg.platformid());
	pDbInfo->set_imei(cgMsg.imei());
	pDbInfo->set_devname(cgMsg.devname());
	pDbInfo->set_mobiletype(cgMsg.mobiletype());
	pDbInfo->set_jetton(GetGlobalConfigObject()->GetNodeInt32(GAME_CONFIG_INIT_JETTON));
	pDbInfo->set_money(GetGlobalConfigObject()->GetNodeInt32(GAME_CONFIG_INIT_MONEY));
	pDbInfo->set_lasttime((int32_t)NFGetSecondTime());
	pDbInfo->set_face_1(NFCPlayerModule::GetInitFaceID());
	pDbInfo->set_sex(cgMsg.sex());
	pDbInfo->set_regdate(NFDateTime::Now().GetDbTimeString());
	bool ret = FindModule<NFIMysqlModule>()->UpdateOne(db_playerinfo);
	if (FindModule<NFIAsyMysqlModule>()->UpdateOne(db_playerinfo, 0) == false)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG,0, "Nosql set playerinfo failed, pInfo:{}", db_playerinfo.DebugString());
	}
	if (ret == false)
	{
		NFBehaviorLog(0, "", "player", "LoadPlayerInfoByCID", -1, "加载数据库玩家信息失败");
		return;
	}
}

NFIObject* NFCPlayerModule::LoadPlayerInfo(uint64_t playerId, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;
	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_userid(NFCommon::tostr(playerId));
	bool ret = FindModule<NFIMysqlModule>()->QueryOne(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(playerId, "", "player", "LoadPlayerInfoByCID", -1, "加载数据库玩家信息失败");
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	pObject = CreatePlayerObject(pDbInfo);
	if (pObject == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "NFCHumanModule::CreatePlayerObject failed:{}", pDbInfo->DebugString());
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	std::string strValue;
	if (pDbInfo->SerializeToString(&strValue))
	{
		if (FindModule<NFINoSqlModule>()->Set("playerinfo" + NFCommon::tostr(pDbInfo->userid()), strValue) == false)
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "nosql set playerifo failed:{}", pDbInfo->DebugString());
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "pInfo.SerializeToString error:{}", pDbInfo->DebugString());
	}

	retCode = RETURN_CODE_SUCCESS;
	return pObject;
}