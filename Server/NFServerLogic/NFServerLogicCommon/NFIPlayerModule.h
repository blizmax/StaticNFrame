// -------------------------------------------------------------------------
//    @FileName         :    NFIHumanModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFIHumanModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIGameDynamicModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFPlayerDefine.h"

class NFIPlayerModule : public NFIGameDynamicModule
{
public:
	NFIPlayerModule(NFIPluginManager* p) :NFIGameDynamicModule(p)
	{

	}

	virtual ~NFIPlayerModule()
	{

	}

	virtual NFIObject* GetPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode) = 0;

	virtual  NFIObject*  GetPlayerInfo(uint64_t playerId, uint32_t& retCode) = 0;

	virtual void CreatePlayer(const NFMsg::cgaccountlogin& cgMsg) = 0;

	virtual NFIObject* GetPlayerObject(uint64_t playerId) = 0;
};
