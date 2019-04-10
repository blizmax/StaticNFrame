// -------------------------------------------------------------------------
//    @FileName         :    NFHumanModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIDynamicModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"

class NFHumanModule : public NFIDynamicModule
{
public:
	NFHumanModule(NFIPluginManager* p);
	virtual ~NFHumanModule();

	virtual bool Init() override;

	virtual uint32_t GetPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo);

	virtual uint32_t LoadPlayerInfoByCID(const std::string& account, const std::string& password, NFMsg::playerinfo* pInfo);

	virtual uint32_t GetPlayerInfo(uint32_t playerId, NFMsg::playerinfo* pInfo);
};
