// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.cpp
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#include "NFPlayerServicesModule.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFMessageDefine/NFNodeClass.h"
#include "NFMessageDefine/NFNodeClassName.h"
#include "NFComm/NFPluginModule/NFIObject.h"
#include "NFServerLogic/NFServerLogicCommon/NFPlayerDefine.h"
#include "NFComm/NFCore/NFCommon.h"

#define SAVE_PLAYER_DATA_TIMER 0
#define SAVE_PLAYER_DATA_TIMER_TIME (1 * 60 * 1000)

NFCPlayerServicesModule::NFCPlayerServicesModule(NFIPluginManager* p):NFIPlayerServicesModule(p)
{

}

NFCPlayerServicesModule::~NFCPlayerServicesModule()
{

}

bool NFCPlayerServicesModule::Init()
{
	this->SetTimer(SAVE_PLAYER_DATA_TIMER, SAVE_PLAYER_DATA_TIMER_TIME, INFINITY_CALL);
	return true;
}

void NFCPlayerServicesModule::OnTimer(uint32_t nTimerID)
{
	if (nTimerID == SAVE_PLAYER_DATA_TIMER)
	{
		SavePlayerDataToDB();
	}
}

void NFCPlayerServicesModule::SavePlayerDataToDB()
{
	std::unordered_map<uint64_t, NFIObject*>* pObjectMap = FindModule<NFIKernelModule>()->GetAllObject(NF_NODE_STRING_CLASS_NAME_PLAYER);
	if (pObjectMap == nullptr)
	{
		return;
	}

	for (auto iter = pObjectMap->begin(); iter != pObjectMap->end(); iter++)
	{
		NFIObject* pObject = iter->second;
		if (pObject->GetNodeString(NF_NODE_STRING_CLASS_NAME) == NF_NODE_STRING_CLASS_NAME_PLAYER)
		{
			NFMsg::db_playerinfo info;
			NFProtobufCommon::NFObjectToMessage(pObject, info);

			std::string strValue;
			if (info.SerializeToString(&strValue))
			{
				if (FindModule<NFINoSqlModule>()->Set("playerinfo" + NFCommon::tostr(info.userid()), strValue) == false)
				{
					NFLogError(NF_LOG_LOGIN_MODULE_LOG, info.userid(), "Nosql set playerinfo failed, pInfo:{}", info.DebugString());
				}

				if (FindModule<NFIAsyMysqlModule>()->Update(info, info.userid()) == false)
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