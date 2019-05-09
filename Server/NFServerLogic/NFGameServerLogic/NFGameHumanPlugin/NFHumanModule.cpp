// -------------------------------------------------------------------------
//    @FileName         :    NFHumanModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFHumanModule.h"

#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"
#include "NFServerLogic/NFServerLogicCommon/NFBehaviorLogMgr.h"
#include "NFComm/NFPluginModule/NFCObject.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFServerLogic/NFServerLogicCommon/NFIGameConfigModule.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"

#include "NFMessageDefine/NFNodeClass.h"
#include "NFMessageDefine/NFNodeClassName.h"

NFCHumanModule::NFCHumanModule(NFIPluginManager* p):NFIHumanModule(p)
{

}

NFCHumanModule::~NFCHumanModule()
{

}

bool NFCHumanModule::Init()
{
	return true;
}

bool NFCHumanModule::DynamicLoadPlugin()
{
	m_pKernelModule = m_pPluginManager->FindModule<NFIKernelModule>();
	m_pMysqlModule = m_pPluginManager->FindModule<NFIMysqlModule>();
	m_pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	m_pAsyMysqlModule = m_pPluginManager->FindModule<NFIAsyMysqlModule>();
	return true;
}

NFIObject* NFCHumanModule::CreatePlayerObject(NFMsg::db_playerinfo* pInfo)
{
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "function param error, pInfo == nullptr");
		return nullptr;
	}

	NFIObject* pObject = m_pKernelModule->CreateNFObject(pInfo->userid(), NF_NODE_STRING_CLASS_NAME_PLAYER);
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

NFIObject* NFCHumanModule::GetPlayerObject(uint64_t playerId)
{
	NFIObject* pObject = m_pKernelModule->GetNFObject(playerId, NF_NODE_STRING_CLASS_NAME_PLAYER);
	if (pObject)
	{
		return pObject;
	}

	NFLogWarning(NF_LOG_LOGIN_MODULE_LOG, playerId, "Get Player Failed!, not exist player:{} where the Node 'ClassName' is player", playerId);
	return nullptr;
}

NFIObject* NFCHumanModule::LoadPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;

	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_account(account);
	bool ret = m_pMysqlModule->Query(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "ÕËºÅ²»´æÔÚ,account=" + account);
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	if (pDbInfo->password() != password)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "µÇÂ¼ÃÜÂë²»Æ¥Åä,password=" + password);
		retCode = RETURN_CODE_PASSWORD_NOT_MATCH;
		return pObject;
	}

	pObject = NFCHumanModule::CreatePlayerObject(pDbInfo);
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
		if (m_pNosqlModule->Set("account" + pDbInfo->account(), strValue) == false)
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
		if (m_pNosqlModule->Set("playerinfo" + NFCommon::tostr(pDbInfo->userid()), strValue) == false)
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

NFIObject* NFCHumanModule::GetPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;

	std::string strUserID;
	if (m_pNosqlModule->Get("account" + account, strUserID))
	{
		NFMsg::accountinfo accountInfo;
		if (accountInfo.ParseFromString(strUserID))
		{
			if (accountInfo.password() != password)
			{
				NFBehaviorLog(0, account, "player", "GetPlayerInfoByCID", RETURN_CODE_PASSWORD_NOT_MATCH, "µÇÂ¼ÃÜÂë²»Æ¥Åä,password=" + password);
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

NFIObject*  NFCHumanModule::GetPlayerInfo(uint64_t playerId, uint32_t& retCode)
{
	NFIObject* pObject = GetPlayerObject(playerId);
	if (pObject)
	{
		retCode = RETURN_CODE_SUCCESS;
		return pObject;
	}

	std::string strValue;
	if (m_pNosqlModule->Get("playerinfo" + NFCommon::tostr(playerId), strValue))
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

std::string NFCHumanModule::GetInitFaceID()
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

void NFCHumanModule::CreatePlayer(const NFMsg::cgaccountlogin& cgMsg)
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
	pDbInfo->set_lasttime(NFGetSecondTime());
	pDbInfo->set_face_1(NFCHumanModule::GetInitFaceID());
	pDbInfo->set_sex(cgMsg.sex());
	pDbInfo->set_regdate(NFDateTime::Now().GetDbTimeString());
	bool ret = m_pMysqlModule->Updata(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(0, "", "player", "LoadPlayerInfoByCID", -1, "¼ÓÔØÊý¾Ý¿âÍæ¼ÒÐÅÏ¢Ê§°Ü");
		return;
	}
}

NFIObject* NFCHumanModule::LoadPlayerInfo(uint64_t playerId, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;
	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_userid(NFCommon::tostr(playerId));
	bool ret = m_pMysqlModule->Query(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(playerId, "", "player", "LoadPlayerInfoByCID", -1, "¼ÓÔØÊý¾Ý¿âÍæ¼ÒÐÅÏ¢Ê§°Ü");
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
		if (m_pNosqlModule->Set("playerinfo" + NFCommon::tostr(pDbInfo->userid()), strValue) == false)
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