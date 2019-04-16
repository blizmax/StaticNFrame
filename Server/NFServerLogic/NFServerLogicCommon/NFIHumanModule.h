// -------------------------------------------------------------------------
//    @FileName         :    NFIHumanModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFIHumanModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIDynamicModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"

class NFIHumanModule : public NFIDynamicModule
{
public:
	virtual uint32_t GetPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo) = 0;

	virtual uint32_t GetPlayerInfo(uint32_t playerId, NFMsg::playerinfo* pInfo) = 0;

	virtual void CreatePlayer(NFMsg::playerinfo* pInfo) = 0;
};
