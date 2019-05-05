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

class NFCHumanModule : public NFIHumanModule
{
public:
	NFCHumanModule(NFIPluginManager* p);
	virtual ~NFCHumanModule();

	virtual bool Init() override;

	virtual NFIObject* GetPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode);

	virtual  NFIObject*  GetPlayerInfo(uint64_t playerId, uint32_t& retCode);

	virtual void CreatePlayer(const NFMsg::cgaccountlogin& cgMsg);

	virtual void CreatePlayerStates(uint64_t playerId, const std::string& account);
private:
	virtual std::string GetInitFaceID();

	virtual NFIObject* LoadPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode);

	virtual NFIObject* LoadPlayerInfo(uint64_t playerId, uint32_t& retCode);

	static void CopyFromDB(NFMsg::playerinfo* pInfo, NFMsg::db_playerinfo* pDbInfo);
private:
	virtual NFIObject* CreatePlayerObject(NFMsg::db_playerinfo* pInfo);
	virtual NFIObject* GetPlayerObject(uint64_t playerId);
};
