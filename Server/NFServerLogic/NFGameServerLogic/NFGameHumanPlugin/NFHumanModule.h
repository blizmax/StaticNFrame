// -------------------------------------------------------------------------
//    @FileName         :    NFHumanModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIHumanModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFHumanDefine.h"
#include "NFComm/NFCore/NFMapEx.hpp"

class NFCHumanModule : public NFIHumanModule
{
public:
	NFCHumanModule(NFIPluginManager* p);
	virtual ~NFCHumanModule();

	virtual bool Init() override;

	virtual NFIObject* GetPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode);

	virtual  NFIObject*  GetPlayerInfo(uint64_t playerId, uint32_t& retCode);

	virtual void CreatePlayer(const NFMsg::cgaccountlogin& cgMsg);

private:
	virtual std::string GetInitFaceID();

	virtual NFIObject* LoadPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode);

	virtual NFIObject* LoadPlayerInfo(uint64_t playerId, uint32_t& retCode);
private:
	virtual NFIObject* CreatePlayerObject(NFMsg::db_playerinfo* pInfo);
	virtual NFIObject* GetPlayerObject(uint64_t playerId);

};
