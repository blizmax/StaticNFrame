// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFHumanServicesModule.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFMessageDefine/NFNodeClass.h"
#include "NFMessageDefine/NFNodeClassName.h"
#include "NFComm/NFPluginModule/NFIObject.h"
#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"
#include "NFComm/NFCore/NFCommon.h"

#define SAVE_PLAYER_DATA_TIMER 0
#define SAVE_PLAYER_DATA_TIMER_TIME (1 * 60 * 1000)

NFCHumanServicesModule::NFCHumanServicesModule(NFIPluginManager* p):NFIHumanServicesModule(p)
{

}

NFCHumanServicesModule::~NFCHumanServicesModule()
{

}

bool NFCHumanServicesModule::Init()
{
	this->SetTimer(SAVE_PLAYER_DATA_TIMER, SAVE_PLAYER_DATA_TIMER_TIME, INFINITY_CALL);
	return true;
}

void NFCHumanServicesModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == SAVE_PLAYER_DATA_TIMER)
	{
		SavePlayerDataToDB();
	}
}

bool NFCHumanServicesModule::DynamicLoadPlugin()
{
	m_pKernelModule = m_pPluginManager->FindModule<NFIKernelModule>();
	m_pNosqlModule = m_pPluginManager->FindModule<NFINoSqlModule>();
	m_pAsyMysqlModule = m_pPluginManager->FindModule<NFIAsyMysqlModule>();
	return true;
}

void NFCHumanServicesModule::SavePlayerDataToDB()
{
	std::unordered_map<uint64_t, NFIObject*>& objectMap = m_pKernelModule->GetAllObject();
	for (auto iter = objectMap.begin(); iter != objectMap.end(); iter++)
	{
		NFIObject* pObject = iter->second;
		if (pObject->GetNodeString(NF_NODE_STRING_CLASS_NAME) == NF_NODE_STRING_CLASS_NAME_PLAYER)
		{
			NFMsg::db_playerinfo info;
			NFProtobufCommon::NFObjectToMessage(pObject, info);

			std::string strValue;
			if (info.SerializeToString(&strValue))
			{
				if (m_pNosqlModule->Set("playerinfo" + NFCommon::tostr(info.userid()), strValue) == false)
				{
					NFLogError(NF_LOG_LOGIN_MODULE_LOG, info.userid(), "Nosql set playerinfo failed, pInfo:{}", info.DebugString());
				}

				if (m_pAsyMysqlModule->Update(info, info.userid()) == false)
				{
					NFLogError(NF_LOG_LOGIN_MODULE_LOG, info.userid(), "Nosql set playerinfo failed, pInfo:{}", info.DebugString());
				}
			}
			else
			{
				NFLogError(NF_LOG_LOGIN_MODULE_LOG, info.userid(), "pInfo.SerializeToString error:{}", info.DebugString());
			}
		}
	}
}