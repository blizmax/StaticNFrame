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
#include "NFComm/NFPluginModule/NFCObject.h"
#include "NFComm/NFCore/NFCommon.h"
#include "NFServerLogic/NFServerLogicCommon/NFIGameConfigModule.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "NFComm/NFCore/NFRandom.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFMessageDefine/NFNodeClass.h"
#include "NFMessageDefine/NFNodeClassName.h"

NFCHumanModule::NFCHumanModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
}

NFCHumanModule::~NFCHumanModule()
{

}

bool NFCHumanModule::Init()
{
	return true;
}

NFIObject* NFCHumanModule::CreatePlayerObject(NFMsg::db_playerinfo* pInfo)
{
	if (pInfo == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "function param error, pInfo == nullptr");
		return nullptr;
	}

	NFIKernelModule* pKernelModule = m_pPluginManager->FindModule<NFIKernelModule>();
	NFIObject* pObject = pKernelModule->CreateNFObject(pInfo->userid(), NF_NODE_STRING_CLASS_NAME_PLAYER);
	if (pObject)
	{
		if (NFProtobufCommon::NFObjectFromMessage(pObject, *pInfo))
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "create player success, userid:{}", pInfo->userid());
			return nullptr;
		}
		else
		{
			NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "create player failed, userid:{}", pInfo->userid());
			return nullptr;
		}
	}
	else
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, 0, "create player failed, userid:{}", pInfo->userid());
		return nullptr;
	}
}

NFIObject* NFCHumanModule::GetPlayerObject(uint64_t playerId)
{
	NFIKernelModule* pKernelModule = m_pPluginManager->FindModule<NFIKernelModule>();
	NFIObject* pObject = pKernelModule->GetNFObject(playerId, NF_NODE_STRING_CLASS_NAME_PLAYER);
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
	NFIMysqlModule* pMysqlModule = m_pPluginManager->FindModule<NFIMysqlModule>();
	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_account(account);
	bool ret = pMysqlModule->Query(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "�˺Ų�����,account=" + account);
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	if (pDbInfo->password() != password)
	{
		NFBehaviorLog(0, account, "player", "LoadPlayerInfoByCID", -1, "��¼���벻ƥ��,password=" + password);
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
		if (pNosqlDriver->Set("account" + pDbInfo->account(), strValue) == false)
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
		if (pNosqlDriver->Set("playerinfo" + NFCommon::tostr(pDbInfo->userid()), strValue) == false)
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
				NFBehaviorLog(0, account, "player", "GetPlayerInfoByCID", RETURN_CODE_PASSWORD_NOT_MATCH, "��¼���벻ƥ��,password=" + password);
				retCode = RETURN_CODE_PASSWORD_NOT_MATCH;
				return pObject;
			}

			pObject = GetPlayerInfo(accountInfo.userid(), retCode);
			if (pObject)
			{
				retCode = RETURN_CODE_SUCCESS;
				return pObject;
			}

			retCode = RETURN_CODE_SUCCESS;
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

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	std::string strValue;
	if (pNosqlDriver->Get("playerinfo" + NFCommon::tostr(playerId), strValue))
	{
		NFMsg::db_playerinfo dbInfo;
		if (dbInfo.ParseFromString(strValue))
		{
			pObject = NFCHumanModule::CreatePlayerObject(&dbInfo);
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
	NFIMysqlModule* pMysqlModule = m_pPluginManager->FindModule<NFIMysqlModule>();
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
	bool ret = pMysqlModule->Updata(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(0, "", "player", "LoadPlayerInfoByCID", -1, "�������ݿ������Ϣʧ��");
		return;
	}
}

void NFCHumanModule::CreatePlayerStates(uint64_t playerId, const std::string& account)
{

}

NFIObject* NFCHumanModule::LoadPlayerInfo(uint64_t playerId, uint32_t& retCode)
{
	NFIObject* pObject = nullptr;
	NFIMysqlModule* pMysqlModule = m_pPluginManager->FindModule<NFIMysqlModule>();
	NFMsg::db_query_playerinfo db_playerinfo;
	db_playerinfo.mutable_db_cond()->set_userid(NFCommon::tostr(playerId));
	bool ret = pMysqlModule->Query(db_playerinfo);
	if (ret == false)
	{
		NFBehaviorLog(playerId, "", "player", "LoadPlayerInfoByCID", -1, "�������ݿ������Ϣʧ��");
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFMsg::db_playerinfo* pDbInfo = db_playerinfo.mutable_db_fields();

	pObject = NFCHumanModule::CreatePlayerObject(pDbInfo);
	if (pObject == nullptr)
	{
		NFLogError(NF_LOG_LOGIN_MODULE_LOG, pDbInfo->userid(), "NFCHumanModule::CreatePlayerObject failed:{}", pDbInfo->DebugString());
		retCode = RETURN_CODE_ACCOUNT_NO_EXIST;
		return pObject;
	}

	NFINoSqlModule* pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	NF_SHARE_PTR<NFINoSqlDriver> pNosqlDriver = pNosqlModule->GetDriverBySuitConsistent();

	std::string strValue;
	if (pDbInfo->SerializeToString(&strValue))
	{
		if (pNosqlDriver->Set("playerinfo" + NFCommon::tostr(pDbInfo->userid()), strValue) == false)
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